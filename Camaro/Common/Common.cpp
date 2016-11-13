#include "Common.h"

#ifdef USE_DXRENDERER
#	ifdef _DEBUG
#		pragma comment(lib, "d3dx9d.lib")
#	else
#		pragma comment(lib, "d3dx9.lib")
#	endif
#	pragma comment(lib, "d3d9.lib")
#	ifdef USE_DIRECTINPUT
#		pragma comment(lib, "dxguid.lib")
#		pragma comment(lib, "dinput8.lib")
#	endif
#endif