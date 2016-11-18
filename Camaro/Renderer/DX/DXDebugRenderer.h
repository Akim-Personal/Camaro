#pragma once

#include <System/God.h>

#ifdef USE_DXRENDERER

struct b2Vec2;
struct b2Color;
struct b2AABB;
struct b2Transform;

class DXDebugRenderer
{
public:
	DXDebugRenderer();
	~DXDebugRenderer();

	void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis,
		const b2Color& color, int segmentsCount = 32, bool noOutline = false);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
	//void DrawPoint(const b2Vec2& p, float size, const b2Color& color);
	//void DrawString(int x, int y, const char* string, ...);

private:
	struct SPositionColorVertex
	{
		float x, y, z;
		DWORD color;
	};

	void SetObjectColor(SPositionColorVertex* pVertices, int vertexCount, const b2Color& color);
	void ReverseVertexOrder(const b2Vec2* src, SPositionColorVertex* dst, int count);

	LPDIRECT3DDEVICE9 m_dxD3DDevice;
};

#endif