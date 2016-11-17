#ifndef _CPHYSICSRENDERER_H_
#define _CPHYSICSRENDERER_H_

#include "System/God.h"

#if defined(USE_BOX2D) && defined(USE_DXRENDERER)

#include "PhysicsCommon.h"

class DXRenderQueue;

class CPhysicsRenderer : public b2Draw
{
public:
	CPhysicsRenderer();

	bool Init();

	virtual void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	virtual void DrawTransform(const b2Transform& xf);
	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

private:
	DXRenderQueue* m_renderQueue;
};

#endif

#endif // _CPHYSICSRENDERER_H_
