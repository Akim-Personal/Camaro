#include "DXDebugRenderer.h"

#ifdef USE_DXRENDERER

#include <Game/World.h>
#include <Renderer/Renderer.h>
#include <Physics/PhysicsCommon.h>

#define Z_PLANE 0.0f

DXDebugRenderer::DXDebugRenderer()
{
	m_dxD3DDevice = GOD::m_renderer->GetD3DDevice();
}

DXDebugRenderer::~DXDebugRenderer()
{}

void DXDebugRenderer::SetObjectColor(SPositionColorVertex* pVertices, int vertexCount, const b2Color& color)
{
	DWORD alpha50 = 0x77000000;
	for (int i = 0; i < vertexCount; ++i)
	{
		pVertices[i].color = alpha50 + color.GetAsDWORD();
	}
}

void DXDebugRenderer::ReverseVertexOrder(const b2Vec2* src, SPositionColorVertex* dst, int count)
{
	for (int i = 0; i < count; ++i)
	{
		dst[i].x = src[(count - 1) - i].x;
		dst[i].y = src[(count - 1) - i].y;
		dst[i].z = Z_PLANE;
	}
}

void DXDebugRenderer::DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color&)
{
	// Draw outer line
	LPD3DXLINE lineSprite;
	D3DXVECTOR3* points = new D3DXVECTOR3[vertexCount + 1];

	for (int i = 0; i < vertexCount; ++i)
	{
		points[i] = D3DXVECTOR3(vertices[i].x, vertices[i].y, Z_PLANE);
	}

	points[vertexCount] = D3DXVECTOR3(vertices[0].x, vertices[0].y, Z_PLANE);

	D3DXCreateLine(m_dxD3DDevice, &lineSprite);
	if (lineSprite)
	{
		//lineSprite->SetAntialias(TRUE);
		lineSprite->SetWidth(1.0f);
		lineSprite->Begin();
		const D3DXMATRIX transformMatrix = GOD::m_renderer->GetWorldViewProjMatrix();
		lineSprite->DrawTransform(points, vertexCount + 1, &transformMatrix , 0xff000000);
		lineSprite->End();
		lineSprite->Release();
	}

	SAFE_DELETE_ARRAY(points);
}

void DXDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	SPositionColorVertex* pVertices;

	if (FAILED(GOD::m_renderer->GetVertexBuffer()->Lock(0, 0, (void**)&pVertices, 0)))
	{
		LogError("DrawSolidPolygon failed to lock vertex buffer - skipped drawing");
		return;
	}

	SetObjectColor(pVertices, vertexCount, color);
	ReverseVertexOrder(vertices, pVertices, vertexCount);

	GOD::m_renderer->GetVertexBuffer()->Unlock();

	m_dxD3DDevice->SetStreamSource(0, GOD::m_renderer->GetVertexBuffer(), 0, sizeof(SPositionColorVertex));
	m_dxD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_dxD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// Draw outer line
	LPD3DXLINE lineSprite;
	D3DXVECTOR3* points = new D3DXVECTOR3[vertexCount + 1];

	for (int i = 0; i < vertexCount; ++i)
	{
		points[i] = D3DXVECTOR3(vertices[i].x, vertices[i].y, Z_PLANE);
	}

	points[vertexCount] = D3DXVECTOR3(vertices[0].x, vertices[0].y, Z_PLANE);

	D3DXCreateLine(m_dxD3DDevice, &lineSprite);
	if (lineSprite)
	{
		//lineSprite->SetAntialias(TRUE);
		lineSprite->SetWidth(1.0f);
		lineSprite->Begin();
		const D3DXMATRIX transformMatrix = GOD::m_renderer->GetWorldViewProjMatrix();
		lineSprite->DrawTransform(points, vertexCount + 1, &transformMatrix , 0xff000000);
		lineSprite->End();
		lineSprite->Release();
	}

	SAFE_DELETE_ARRAY(points);
}

void DXDebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color&)
{
	// Draw outer line
	LPD3DXLINE lineSprite;
	const int segments = 32;
	const int pointCount = segments + 1;
	D3DXVECTOR3* points = new D3DXVECTOR3[pointCount];

	for (int i = 0; i < pointCount; ++i)
	{
		float angle = (2 * b2_pi) - (2 * b2_pi / (float)segments * i);
		points[i].x = center.x + radius * cos(angle);
		points[i].y = center.y + radius * sin(angle);
		points[i].z = Z_PLANE;
	}

	points[pointCount - 1] = points[0];

	D3DXCreateLine(m_dxD3DDevice, &lineSprite);
	if (lineSprite)
	{
		//lineSprite->SetAntialias(TRUE);
		lineSprite->SetWidth(1.0f);
		lineSprite->Begin();
		const D3DXMATRIX transformMatrix = GOD::m_renderer->GetWorldViewProjMatrix();
		lineSprite->DrawTransform(points, pointCount, &transformMatrix , 0xff000000);
		lineSprite->End();
		lineSprite->Release();
	}

	SAFE_DELETE_ARRAY(points);
}

void DXDebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis,
	const b2Color& color, int segmentsCount, bool noOutline)
{
	SPositionColorVertex* pVertices;
	const int vertexCount = segmentsCount + 2;

	if (FAILED(GOD::m_renderer->GetVertexBuffer()->Lock(0, 0, (void**)&pVertices, 0)))
	{
		LogError("DrawSolidCircle failed to lock vertex buffer - skipped drawing");
		return;
	}

	SetObjectColor(pVertices, vertexCount, color);

	pVertices[0].x = center.x;
	pVertices[0].y = center.y;
	pVertices[0].z = Z_PLANE;

	for (int i = 1; i < vertexCount - 1; ++i)
	{
		float angle = (2 * b2_pi) - (2 * b2_pi / (float)segmentsCount * i);
		pVertices[i].x = center.x + radius * cos(angle);
		pVertices[i].y = center.y + radius * sin(angle);
		pVertices[i].z = Z_PLANE;
	}

	pVertices[vertexCount - 1] = pVertices[1];

	GOD::m_renderer->GetVertexBuffer()->Unlock();

	m_dxD3DDevice->SetStreamSource(0, GOD::m_renderer->GetVertexBuffer(), 0, sizeof(SPositionColorVertex));
	m_dxD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_dxD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, segmentsCount);

	if (!noOutline)
	{
		// Draw outer line
		LPD3DXLINE lineSprite;
		const int pointCount = segmentsCount + 1;
		D3DXVECTOR3* points = new D3DXVECTOR3[pointCount];

		for (int i = 0; i < pointCount; ++i)
		{
			points[i] = D3DXVECTOR3(pVertices[i + 1].x, pVertices[i + 1].y, Z_PLANE);
		}

		D3DXCreateLine(m_dxD3DDevice, &lineSprite);
		if (lineSprite)
		{
			//lineSprite->SetAntialias(TRUE);
			lineSprite->SetWidth(1.0f);
			lineSprite->Begin();
			const D3DXMATRIX transformMatrix = GOD::m_renderer->GetWorldViewProjMatrix();
			lineSprite->DrawTransform(points, pointCount, &transformMatrix , 0xff000000);
			points[0].x = center.x;
			points[0].y = center.y;
			points[1].x = center.x + radius * axis.x;
			points[1].y = center.y + radius * axis.y;
			lineSprite->DrawTransform(points, 2, &transformMatrix , 0xff000000);
			lineSprite->End();
			lineSprite->Release();
		}

		SAFE_DELETE_ARRAY(points);
	}
}

void DXDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	LPD3DXLINE lineSprite;
	D3DXVECTOR3 points[2];

	D3DXCreateLine(m_dxD3DDevice, &lineSprite);
	if (lineSprite)
	{
		//lineSprite->SetAntialias(TRUE);
		lineSprite->SetWidth(1.0f);
		lineSprite->Begin();
		const D3DXMATRIX transformMatrix = GOD::m_renderer->GetWorldViewProjMatrix();
		points[0].x = p1.x;
		points[0].y = p1.y;
		points[0].z = Z_PLANE;
		points[1].x = p2.x;
		points[1].y = p2.y;
		points[1].z = Z_PLANE;
		lineSprite->DrawTransform(points, 2, &transformMatrix , color.GetAsDWORD());
		lineSprite->End();
		lineSprite->Release();
	}
}

void DXDebugRenderer::DrawTransform(const b2Transform& xf)
{
	LPD3DXLINE lineSprite;
	D3DXVECTOR3 points[2];

	D3DXCreateLine(m_dxD3DDevice, &lineSprite);
	if (lineSprite)
	{
		//lineSprite->SetAntialias(TRUE);
		lineSprite->SetWidth(1.0f);
		lineSprite->Begin();
		const D3DXMATRIX transformMatrix = GOD::m_renderer->GetWorldViewProjMatrix();
		const float k_axisScale = 0.5f;
		points[0].x = xf.p.x;
		points[0].y = xf.p.y;
		points[0].z = Z_PLANE;
		points[1].x = xf.p.x + k_axisScale * xf.q.GetXAxis().x;
		points[1].y = xf.p.y + k_axisScale * xf.q.GetXAxis().y;
		points[1].z = Z_PLANE;
		lineSprite->DrawTransform(points, 2, &transformMatrix , 0xffff0000);
		points[1].x = xf.p.x + k_axisScale * xf.q.GetYAxis().x;
		points[1].y = xf.p.y + k_axisScale * xf.q.GetYAxis().y;
		lineSprite->DrawTransform(points, 2, &transformMatrix , 0xff00ff00);
		lineSprite->End();
		lineSprite->Release();
	}
}

//void DXDebugRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
//{
//	b2Vec2 axisDummy = b2Vec2(0.0f, 0.0f);
//	DrawSolidCircle(p, 2 * size, axisDummy, color, 8, true);
//}
//
//void DXDebugRenderer::DrawString(int, int, const char* string, ...)
//{
//	char buffer[256];
//	va_list arg;
//	va_start(arg, string);
//	vsprintf_s(buffer, string, arg);
//	va_end(arg);
//
//	HDC hdc = CreateCompatibleDC(NULL);
//	HFONT font = CreateFont(0, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Calibri");
//	SelectObject(hdc, font);
//
//	ID3DXMesh *mesh;
//	D3DXCreateText(m_dxD3DDevice, hdc, buffer, 0.01f, 0.000001f, &mesh, NULL, NULL);
//
//	LPD3DXMATRIX worldMat = GOD::m_DXRenderer->GetWorldMatrix();
//	D3DXMatrixTranslation(worldMat, (float)x, (float)y, Z_PLANE);
//	m_dxD3DDevice->SetTransform(D3DTS_WORLD, worldMat);
//
//	mesh->DrawSubset(0);
//	mesh->Release();
//
//	D3DXMatrixIdentity(worldMat);
//	m_dxD3DDevice->SetTransform(D3DTS_WORLD, worldMat);
//
//	if (font) DeleteObject(font);
//	if (hdc) DeleteDC(hdc);
//}

#endif