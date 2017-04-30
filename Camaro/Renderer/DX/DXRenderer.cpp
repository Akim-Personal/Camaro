#include "DXRenderer.h"

#ifdef USE_DXRENDERER

#ifdef USE_BOX2D
#	include <Physics/Physics.h>
#	include <Physics/PhysicsRenderer.h>
#endif

#include "DXDebugRenderer.h"
#include "DXRenderQueue.h"

uint* renderThreadID = nullptr;

DXRenderer::DXRenderer()
{
	m_dxD3D = NULL;
	m_dxD3DDevice = NULL;
	m_vertexBuffer = NULL;
	m_debugRenderer = NULL;
	m_renderQueue = NULL;
}

bool DXRenderer::Init()
{
	// Initialize Direct3D
	if (SUCCEEDED(SetupD3D()))
	{
		InitializeMatrices();

		if (FAILED(GetD3DDevice()->CreateVertexBuffer(8 * 1024 * 1024, 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_vertexBuffer, NULL)))
			LogFatalError("Failed to initialize Vertex buffer");

		m_debugRenderer = new DXDebugRenderer();
		m_renderQueue = new DXRenderQueue();

#ifdef USE_BOX2D
		GOD::m_physics->GetPhysicsRenderer()->Init();
#endif

		// Show the window
		ShowWindow(GOD::m_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(GOD::m_hWnd);
		return true;
	}

	LogFatalError("Could not initialize DXRenderer");
	return false;
}

HRESULT DXRenderer::SetupD3D()
{
	// Create the D3D object, return failure if this can't be done.
	if (NULL == (m_dxD3D = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // disable vsync

	// Anti-Aliasing Check
	//if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL, D3DFMT_R8G8B8, FALSE, D3DMULTISAMPLE_4_SAMPLES, &blab)))
	//{
	//	d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	//}
	//else if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, FALSE, D3DMULTISAMPLE_2_SAMPLES, NULL)))
	//{
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	//}

	// Create the D3DDevice
	if (FAILED(m_dxD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GOD::m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_dxD3DDevice)))
	{
		return E_FAIL;
	}

	m_dxD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_dxD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Alpha
	m_dxD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_dxD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_dxD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	//m_dxD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//m_dxD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)50);
	//m_dxD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, 1);

	return S_OK;
}

void DXRenderer::CleanUp()
{
	SAFE_DELETE(m_renderQueue);
	SAFE_DELETE(m_debugRenderer);

	SAFE_RELEASE(m_vertexBuffer);

	SAFE_DELETE(m_worldMatrix);
	SAFE_DELETE(m_projMatrix);
	SAFE_DELETE(m_viewMatrix);

	SAFE_RELEASE(m_dxD3DDevice);
	SAFE_RELEASE(m_dxD3D);
}

void DXRenderer::InitializeMatrices()
{
	m_Camera = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	m_Lookat = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_viewMatrix = new D3DXMATRIX;
	m_projMatrix = new D3DXMATRIX;
	m_worldMatrix = new D3DXMATRIX;

	D3DXMatrixPerspectiveFovLH(m_projMatrix, D3DX_PI/4, 1.25f, 1.0f, 1000.0f);
	//D3DXMatrixOrthoOffCenterLH(m_projMatrix, -30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
	m_dxD3DDevice->SetTransform(D3DTS_PROJECTION, m_projMatrix);

	D3DXMatrixIdentity(m_worldMatrix);
	m_dxD3DDevice->SetTransform(D3DTS_WORLD, m_worldMatrix);
}

void DXRenderer::UpdateCamera()
{
	D3DXMatrixLookAtLH(m_viewMatrix, &m_Camera, &m_Lookat, &m_UpVector);
	m_dxD3DDevice->SetTransform(D3DTS_VIEW, m_viewMatrix);
}

bool DXRenderer::Render()
{
	// Clear the backbuffer
	m_dxD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(170, 170, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(m_dxD3DDevice->BeginScene()))
	{
		UpdateCamera();
		
#ifdef USE_BOX2D
		GOD::m_physics->GetPhysicsWorld()->DrawDebugData();
#endif

		m_renderQueue->ExecuteQueue();

		// End the scene.
		m_dxD3DDevice->EndScene();	
	}

	// Present the backbuffer to the display.
	m_dxD3DDevice->Present(NULL, NULL, NULL, NULL);

	return true;
}

#endif