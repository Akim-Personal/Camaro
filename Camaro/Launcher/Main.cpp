#include "System/God.h"

#ifdef USE_DXRENDERER

// ---------------- DirectX Main ----------------

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//case WM_CLOSE:
	//	{
	//		if (MessageBox(hWnd, "Do you want to quit?", GAME_NAME, MB_OKCANCEL) == IDOK)
	//			DestroyWindow(hWnd);
	//		return 0;
	//	}
	case WM_DESTROY:
		{
			GOD::CleanUp();
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	//Register windows class
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL, GAME_NAME, NULL};
	RegisterClassEx(&wc);

	// Create application window
	GOD::m_hWnd = CreateWindow(GAME_NAME, GAME_NAME,
		WS_OVERLAPPEDWINDOW, 200, 200, 800, 600, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	GOD::m_hInst = hInst;
	GOD::CreateUniverse();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Game Loop
			if (!GOD::Progress())
			{
				DestroyWindow(GOD::m_hWnd);
				break;
			}
		}
	}

	UnregisterClass(GAME_NAME, wc.hInstance);
	return 0;
}

#elif USE_GL

// ---------------- OpenGL Main ----------------

#else

// ---------------- NoRender Main ----------------

#include <conio.h>

int main()
{
	GOD::CreateUniverse();
	while (!_kbhit() && GOD::Progress());
	GOD::CleanUp();

	return 0;
}

#endif