#ifndef _DXRENDERER_H_
#define _DXRENDERER_H_

#include "System/God.h"

#ifdef USE_DXRENDERER

class DXDebugRenderer;
class DXRenderQueue;

class DXRenderer
{
public:
	DXRenderer();
	~DXRenderer() { CleanUp(); }

	bool Init();
	void Render();

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return m_vertexBuffer; }
	LPDIRECT3D9 GetD3D() const { return m_dxD3D; }
	LPDIRECT3DDEVICE9 GetD3DDevice() const { return m_dxD3DDevice; }
	LPD3DXMATRIX GetViewMatrix() const { return m_viewMatrix; }
	LPD3DXMATRIX GetProjMatrix() const { return m_projMatrix; }
	LPD3DXMATRIX GetWorldMatrix() const { return m_worldMatrix; }
	D3DXMATRIX GetWorldViewProjMatrix() const	{ return *GetWorldMatrix() * *GetViewMatrix() * *GetProjMatrix(); }
	void SetViewMatrix(const D3DXMATRIX& mat) { *m_viewMatrix = mat; }
	void SetProjMatrix(const D3DXMATRIX& mat) { *m_projMatrix = mat; }
	void SetWorldMatrix(const D3DXMATRIX& mat) { *m_worldMatrix = mat; }

	D3DXVECTOR3 GetCameraPosition() const { return m_Camera; }
	void SetCameraPosition(D3DXVECTOR3 camPos) { m_Camera = camPos; UpdateCamera(); }

	DXDebugRenderer* GetDebugRenderer() const { return m_debugRenderer; }
	DXRenderQueue* GetRenderQueue() const { return m_renderQueue; }


private:
	void CleanUp();

	void InitializeMatrices();
	void UpdateCamera();

	HRESULT SetupD3D();

	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;

	D3DXVECTOR3 m_Camera;
	D3DXVECTOR3 m_Lookat;
	D3DXVECTOR3 m_UpVector;

	LPDIRECT3D9					m_dxD3D;
	LPDIRECT3DDEVICE9		m_dxD3DDevice;

	LPD3DXMATRIX m_viewMatrix;
	LPD3DXMATRIX m_projMatrix;
	LPD3DXMATRIX m_worldMatrix;

	DXDebugRenderer* m_debugRenderer;
	DXRenderQueue* m_renderQueue;
};

#endif

#endif // _DXRENDERER_H_