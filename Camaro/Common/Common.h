#pragma once

#include "ProjectDefines.h"

#if defined(USE_DXRENDERER) || defined(USE_XINPUT)
#	define STRICT
#	define D3D_DEBUG_INFO
#	include <d3dx9.h>
#endif

#ifdef USE_DIRECTINPUT
#	define DIRECTINPUT_VERSION 0x0800
#	include <dinput.h>
#endif

#ifdef USE_XINPUT
#	include <XInput.h>
#endif

#include <Windows.h>
#include <string>
#include <list>
#include <map>
#include <vector>

#include "String.h"