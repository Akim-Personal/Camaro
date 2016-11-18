#pragma once

#define GAME_NAME "Camaro"

typedef signed char schar;
typedef unsigned char uchar;
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef uint32 uint;
typedef long long int64;
typedef unsigned long long uint64;

#define SAFE_DELETE(p)  { if (p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)  { if (p) { delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }

#define ASSERT(a) { if (!(a)) { LogAssertViolation("Expression: ("#a##")"); } }

#define USE_THREADING

#define USE_DXRENDERER
#ifdef USE_DXRENDERER
#	define USE_DIRECTINPUT
#endif
#define USE_XINPUT
#define USE_BOX2D

#pragma warning(disable: 4100)	// The formal parameter is not referenced in the body of the function.
#pragma warning(disable: 4189)	// A variable is declared and initialized but not used. 