#include "PhysicsRenderer.h"

#if defined(USE_BOX2D) && defined(USE_DXRENDERER)

#include "Renderer/Renderer.h"
#include "Renderer/DX/DXRenderQueue.h"

CPhysicsRenderer::CPhysicsRenderer()
	: b2Draw()
{
	m_renderQueue = NULL;
}

bool CPhysicsRenderer::Init()
{
	if (GOD::m_renderer)
	{
		m_renderQueue = GOD::m_renderer->GetRenderQueue();

		if (!m_renderQueue)
		{
			LogError("CPhysicsRenderer: RenderQueue has not been initialized yet");
			return false;
		}
	}
	else
	{
		LogError("CPhysicsRenderer: Render System has not been initialized yet");
		return false;
	}

	return true;
}

void CPhysicsRenderer::DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	m_renderQueue->RC_DrawPolygon(vertices, vertexCount, color);
}

void CPhysicsRenderer::DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	m_renderQueue->RC_DrawSolidPolygon(vertices, vertexCount, color);
}

void CPhysicsRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	m_renderQueue->RC_DrawCircle(center, radius, color);
}

void CPhysicsRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	m_renderQueue->RC_DrawSolidCircle(center, radius, axis, color);
}

void CPhysicsRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_renderQueue->RC_DrawSegment(p1, p2, color);
}

void CPhysicsRenderer::DrawTransform(const b2Transform& xf)
{
	m_renderQueue->RC_DrawTransform(xf);
}

void CPhysicsRenderer::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	/*not implemented*/
}

#endif