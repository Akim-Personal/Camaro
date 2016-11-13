#ifndef _DXRENDERQUEUE_H_
#define _DXRENDERQUEUE_H_

#include "System/God.h"

#ifdef USE_DXRENDERER

struct b2Vec2;
struct b2Color;
struct b2Transform;
struct b2AABB;
class DXDebugRenderer;

enum ERenderCommand
{
	eRC_EndOfCommandQueue = 0,
	eRC_DrawPolygon,
	eRC_DrawSolidPolygon,
	eRC_DrawCircle,
	eRC_DrawSolidCircle,
	eRC_DrawSegment,
	eRC_DrawTransform,
	eRC_Count
};

class DXRenderQueue
{
private:
	typedef std::vector<void*> TAllocatedCopiesVector;

public:
	DXRenderQueue();
	~DXRenderQueue();

	void ExecuteQueue();

private:
	void InitCommandSizes();
	void AdvancePointer(uint bytes);
	template <typename T> void* MakeCopy(T* ptr, int count);
	void RemoveCopy(void* ptr);

	// write to queue functions
	void QueueCommandBegin(ERenderCommand command);
	void QueueCommandEnd();
	void AddCommand(ERenderCommand command);
	template <typename T> void AddValue(T val);
	template <typename T> void AddPointer(T* ptr);

	// read from queue functions
	void SwapCommandQueue();
	template <typename T> T* ExtractParameter();
	bool RunCommand();

	// double-buffered
	bool m_commandQueueIsSwapped;
	byte* m_selectedCommandQueueHead;
	byte* m_commandQueueHeadCQ1;
	byte* m_commandQueueHeadCQ2;

	byte* m_commandQueueIterator;
	uint* m_commandSize;
	uint m_commandSizeCheck;

	uint m_IDPool;
	TAllocatedCopiesVector m_allocatedCopies;

	bool m_isRenderThread; // for non-threading test - should be removed when threading works

	DXDebugRenderer* m_debugRenderer;

public:
	// -------------------------------- RENDER COMMANDS --------------------------------
	void RC_DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
	void RC_DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
	void RC_DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
	void RC_DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
	void RC_DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void RC_DrawTransform(const b2Transform& xf);
	//void RC_DrawPoint(const b2Vec2& p, float size, const b2Color& color);
	//void RC_DrawString(int x, int y, const char* string, ...);
};

#endif

#endif // _DXRENDERQUEUE_H_