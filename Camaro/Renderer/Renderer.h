#pragma once

#include <System/God.h>

#ifdef USE_DXRENDERER

#include <System/Threading/Thread.h>
#include "DX/DXRenderer.h"
class DXDebugRenderer;

extern uint* renderThreadID;

class CRenderer : public CThread
{
public:
	CRenderer()
		: CThread("RenderThread") 
	{
		m_dxRenderer = new DXRenderer();
		renderThreadID = &m_threadID;
	}

	~CRenderer()
	{
		ForceSafeShutdownThread();
		SAFE_DELETE(m_dxRenderer); 
	}

	bool Init() { return m_dxRenderer->Init(); }
	bool Render() { return m_dxRenderer->Render(); }

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return m_dxRenderer->GetVertexBuffer(); }
	LPDIRECT3D9 GetD3D() const { return m_dxRenderer->GetD3D(); }
	LPDIRECT3DDEVICE9 GetD3DDevice() const { return m_dxRenderer->GetD3DDevice(); }
	LPD3DXMATRIX GetViewMatrix() const { return m_dxRenderer->GetViewMatrix(); }
	LPD3DXMATRIX GetProjMatrix() const { return m_dxRenderer->GetProjMatrix(); }
	LPD3DXMATRIX GetWorldMatrix() const { return m_dxRenderer->GetWorldMatrix(); }
	D3DXMATRIX GetWorldViewProjMatrix() const	{ return m_dxRenderer->GetWorldViewProjMatrix(); }
	void SetViewMatrix(const D3DXMATRIX& mat) { m_dxRenderer->SetViewMatrix(mat); }
	void SetProjMatrix(const D3DXMATRIX& mat) { m_dxRenderer->SetProjMatrix(mat); }
	void SetWorldMatrix(const D3DXMATRIX& mat) { m_dxRenderer->SetWorldMatrix(mat); }

	D3DXVECTOR3 GetCameraPosition() { return m_dxRenderer->GetCameraPosition(); }

	DXDebugRenderer* GetDebugRenderer() const { return m_dxRenderer->GetDebugRenderer(); }
	DXRenderQueue* GetRenderQueue() const { return m_dxRenderer->GetRenderQueue(); }

private:
	virtual bool RunLoop() { return Render(); }

	void SetCameraPosition(D3DXVECTOR3 camPos) { m_dxRenderer->SetCameraPosition(camPos); }

	DXRenderer* m_dxRenderer;
};

#else

class CRenderer
{
public:
	CRenderer() { ; }
	~CRenderer() { ; }

	bool Init() { LogInfo("Non-Renderer implementation has been initialized"); return true; }
	void Render() { ; }
};

#endif