#include "DXRenderQueue.h"

#ifdef USE_DXRENDERER

#include "Physics/PhysicsCommon.h"
#include "Renderer/Renderer.h"
#include "Renderer/DX/DXDebugRenderer.h"

#define RENDER_QUEUE_SIZE 1024 * 1024	// 1 MB

DXRenderQueue::DXRenderQueue()
{
	m_commandQueueHeadCQ1 = new byte[RENDER_QUEUE_SIZE];
	m_commandQueueHeadCQ2 = new byte[RENDER_QUEUE_SIZE];

	m_commandQueueIsSwapped = false;	// initial command queue is CQ1
	SwapCommandQueue();	// initialize CQ2
	SwapCommandQueue();	// initialize CQ1

	m_allocatedCopies.clear();
	m_commandSizeCheck = 0;
	m_commandSize = new uint[eRC_Count];
	m_isRenderThread = false;
	m_IDPool = 0;
	InitCommandSizes();

	m_debugRenderer = GOD::m_renderer->GetDebugRenderer();

	if (!m_debugRenderer)
	{
		LogError("DXRenderQueue: DebugRenderer has not been initialized yet");
	}
}

DXRenderQueue::~DXRenderQueue()
{
	// remove copies which are still in queue
	TAllocatedCopiesVector::iterator copiesIt;
	for (copiesIt = m_allocatedCopies.begin(); copiesIt != m_allocatedCopies.end(); ++copiesIt)
		free(*copiesIt);

	SAFE_DELETE_ARRAY(m_commandSize);
	SAFE_DELETE_ARRAY(m_commandQueueHeadCQ1);
	SAFE_DELETE_ARRAY(m_commandQueueHeadCQ2);
}

void DXRenderQueue::InitCommandSizes()
{
		m_commandSize[eRC_EndOfCommandQueue] = 0;
		m_commandSize[eRC_DrawPolygon] = sizeof(b2Vec2*) + sizeof(int) + sizeof(b2Color);
		m_commandSize[eRC_DrawSolidPolygon] = sizeof(b2Vec2*) + sizeof(int) + sizeof(b2Color);
		m_commandSize[eRC_DrawCircle] = sizeof(b2Vec2) + sizeof(float) + sizeof(b2Color);
		m_commandSize[eRC_DrawSolidCircle] = 2 * sizeof(b2Vec2) + sizeof(float) + sizeof(b2Color);
		m_commandSize[eRC_DrawSegment] = 2 * sizeof(b2Vec2) + sizeof(b2Color);
		m_commandSize[eRC_DrawTransform] = sizeof(b2Transform);
}

void DXRenderQueue::AdvancePointer(uint bytes)
{
	m_commandQueueIterator += bytes;
}

template <typename T> void* DXRenderQueue::MakeCopy(T* ptr, int count)
{
	int size = count * sizeof(T);
	void* copyOfArray = (void*) malloc(size);
	memcpy(copyOfArray, (void*)ptr, size);

	// keep reference in table for cleanup
	m_allocatedCopies.push_back(copyOfArray);

	return copyOfArray;
}

void DXRenderQueue::RemoveCopy(void* ptr)
{
	TAllocatedCopiesVector::iterator allocatedCopiesIt;

	for (allocatedCopiesIt = m_allocatedCopies.begin(); allocatedCopiesIt != m_allocatedCopies.end();)
	{
		if (*allocatedCopiesIt == ptr)
		{
			free(ptr);
			allocatedCopiesIt = m_allocatedCopies.erase(allocatedCopiesIt);
		}
		else
			++allocatedCopiesIt;
	}
}

void DXRenderQueue::QueueCommandBegin(ERenderCommand command)
{
	int queueSizeInBytes = m_commandQueueIterator - m_selectedCommandQueueHead;

	if ( queueSizeInBytes > 0.9f * (float)RENDER_QUEUE_SIZE)
		LogWarning("More than 90%% of Render Queue is in use - consider resizing");

	AddCommand(command);
	m_commandSizeCheck = m_commandSize[command];
}

void DXRenderQueue::QueueCommandEnd()
{
	if (m_commandSizeCheck == 0)
		return;
	else if (m_commandSizeCheck > 0)
		LogFatalError("Command Queue - Too less parameters have been passed");
	else if (m_commandSizeCheck < 0)
		LogFatalError("Command Queue - Too many parameters have been passed");
}

void DXRenderQueue::AddCommand(ERenderCommand command)
{
	// each queued command gets an ID
	*(uint*)m_commandQueueIterator = m_IDPool++;
	AdvancePointer(sizeof(uint));

	*(ERenderCommand*)m_commandQueueIterator = command;
	AdvancePointer(sizeof(ERenderCommand));
}

template <typename T> void DXRenderQueue::AddValue(T val)
{
	*(T*)m_commandQueueIterator = val;

	m_commandSizeCheck -= sizeof(T);
	AdvancePointer(sizeof(T));
}

template <typename T> void DXRenderQueue::AddPointer(T* ptr)
{
	*(T**)m_commandQueueIterator = ptr;

	m_commandSizeCheck -= sizeof(T*);
	AdvancePointer(sizeof(T*));
}

void DXRenderQueue::SwapCommandQueue()
{
	if (m_commandQueueIsSwapped)
	{
		// CQ2 is still active, so we can prepare CQ1
		memset(m_commandQueueHeadCQ1, 0, RENDER_QUEUE_SIZE);
		m_commandQueueIterator = m_commandQueueHeadCQ1;
		m_selectedCommandQueueHead = m_commandQueueHeadCQ1;
	}
	else
	{
		// CQ1 is still active, so we can prepare CQ2
		memset(m_commandQueueHeadCQ2, 0, RENDER_QUEUE_SIZE);
		m_commandQueueIterator = m_commandQueueHeadCQ2;
		m_selectedCommandQueueHead = m_commandQueueHeadCQ2;
	}

	m_commandSizeCheck = 0;
	m_IDPool = 0;
	m_commandQueueIsSwapped = !m_commandQueueIsSwapped;
	m_isRenderThread = false;
}

template <typename T> T* DXRenderQueue::ExtractParameter()
{
	T* parameter = (T*)m_commandQueueIterator;
	AdvancePointer(sizeof(T));
	return parameter;
}

void DXRenderQueue::ExecuteQueue()
{
	m_isRenderThread = true;
	m_commandQueueIterator = m_selectedCommandQueueHead;

	do
	{
		// get ID of current queued command and move pointer to its start
		uint id = *(uint*)m_commandQueueIterator;
		AdvancePointer(sizeof(uint));
		//LogInfo("RQ: Queued command with ID #%d is going to be processed now", id);
	}
	while (RunCommand());

	SwapCommandQueue();
}

// -------------------------------- RENDER COMMAND PARSING --------------------------------

bool DXRenderQueue::RunCommand()
{
	ERenderCommand* currentCommand = ExtractParameter<ERenderCommand>();
	switch (*currentCommand)
	{
	case eRC_EndOfCommandQueue: return false;
		break;
	case eRC_DrawPolygon:
		{
			const b2Vec2** verticesCopy = ExtractParameter<const b2Vec2*>();
			int vertexCount = *(ExtractParameter<int>());
			const b2Color* color = ExtractParameter<const b2Color>();
			RC_DrawPolygon(*verticesCopy, vertexCount, *color);
			RemoveCopy((void*)*verticesCopy);
			break;
		}
	case eRC_DrawSolidPolygon:
		{
			const b2Vec2** verticesCopy = ExtractParameter<const b2Vec2*>();
			int vertexCount = *(ExtractParameter<int>());
			const b2Color* color = ExtractParameter<const b2Color>();
			RC_DrawSolidPolygon(*verticesCopy, vertexCount, *color);
			RemoveCopy((void*)*verticesCopy);
			break;
		}
	case eRC_DrawCircle:
		{
			const b2Vec2* center = ExtractParameter<const b2Vec2>();
			float radius = *(ExtractParameter<float>());
			const b2Color* color = ExtractParameter<const b2Color>();
			RC_DrawCircle(*center, radius, *color);
			break;
		}
	case eRC_DrawSolidCircle:
		{
			const b2Vec2* center = ExtractParameter<const b2Vec2>();
			float radius = *(ExtractParameter<float>());
			const b2Vec2* axis = ExtractParameter<const b2Vec2>();
			const b2Color* color = ExtractParameter<const b2Color>();
			RC_DrawSolidCircle(*center, radius, *axis, *color);
			break;
		}
	case eRC_DrawSegment:
		{
			const b2Vec2* p1 = ExtractParameter<const b2Vec2>();
			const b2Vec2* p2 = ExtractParameter<const b2Vec2>();
			const b2Color* color = ExtractParameter<const b2Color>();
			RC_DrawSegment(*p1, *p2, *color);
			break;
		}
	case eRC_DrawTransform:
		{
			const b2Transform* xf = ExtractParameter<const b2Transform>();
			RC_DrawTransform(*xf);
			break;
		}
	default: LogFatalError("Render Queue - Command has not been defined");
		break;
	}

	return true;
}

// -------------------------------- RENDER COMMAND IMPLEMENTATIONS --------------------------------

void DXRenderQueue::RC_DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	if (m_isRenderThread)
		m_debugRenderer->DrawPolygon(vertices, vertexCount, color);
	else
	{
		QueueCommandBegin(eRC_DrawPolygon);
		const b2Vec2* verticesCopy = (const b2Vec2*)MakeCopy(vertices, vertexCount);
		AddPointer(verticesCopy);
		AddValue(vertexCount);
		AddValue(color);
		QueueCommandEnd();
	}	
}

void DXRenderQueue::RC_DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	if (m_isRenderThread)
		m_debugRenderer->DrawSolidPolygon(vertices, vertexCount, color);
	else
	{
		QueueCommandBegin(eRC_DrawSolidPolygon);
		const b2Vec2* verticesCopy = (const b2Vec2*)MakeCopy(vertices, vertexCount);
		AddPointer(verticesCopy);
		AddValue(vertexCount);
		AddValue(color);
		QueueCommandEnd();
	}	
}

void DXRenderQueue::RC_DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	if (m_isRenderThread)
		m_debugRenderer->DrawCircle(center, radius, color);
	else
	{
		QueueCommandBegin(eRC_DrawCircle);
		AddValue(center);
		AddValue(radius);
		AddValue(color);
		QueueCommandEnd();
	}
}

void DXRenderQueue::RC_DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	if (m_isRenderThread)
		m_debugRenderer->DrawSolidCircle(center, radius, axis, color);
	else
	{
		QueueCommandBegin(eRC_DrawSolidCircle);
		AddValue(center);
		AddValue(radius);
		AddValue(axis);
		AddValue(color);
		QueueCommandEnd();
	}
}

void DXRenderQueue::RC_DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	if (m_isRenderThread)
		m_debugRenderer->DrawSegment(p1, p2, color);
	else
	{
		QueueCommandBegin(eRC_DrawSegment);
		AddValue(p1);
		AddValue(p2);
		AddValue(color);
		QueueCommandEnd();
	}
}

void DXRenderQueue::RC_DrawTransform(const b2Transform& xf)
{
	if (m_isRenderThread)
		m_debugRenderer->DrawTransform(xf);
	else
	{
		QueueCommandBegin(eRC_DrawTransform);
		AddValue(xf);
		QueueCommandEnd();
	}
}

#endif