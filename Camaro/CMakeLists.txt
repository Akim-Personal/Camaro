cmake_minimum_required (VERSION 2.6)
project (Camaro CXX C)

set(CAMARO_COMMON
	Common/Common.cpp
	Common/Common.h
	Common/ProjectDefines.h
	Common/String.h
)

set(CAMARO_GAME
	Game/Instances/Bunny.cpp
	Game/Instances/Bunny.h
	Game/Instances/Dynamic.cpp
	Game/Instances/Dynamic.h
	Game/Instances/Instance.h
	Game/Instances/InstanceProperties.h
	Game/Instances/Solid.cpp
	Game/Instances/Solid.h
	Game/Instances/TestObject.cpp
	Game/Instances/TestObject.h
	Game/InstanceManager.cpp
	Game/InstanceManager.h
	Game/World.cpp
	Game/World.h
)

set(CAMARO_INPUT
	Input/DX/DXGamepadInput.cpp
	Input/DX/DXGamepadInput.h
	Input/DX/DXKeyboardInput.cpp
	Input/DX/DXKeyboardInput.h
	Input/InputDevice.h
	Input/InputManager.cpp
	Input/InputManager.h	
)

set(CAMARO_LAUNCHER
	Launcher/Main.cpp
)

set(CAMARO_PHYSICS
	Physics/Physics.cpp
	Physics/Physics.h
	Physics/PhysicsCommon.h
	Physics/PhysicsModifier.cpp
	Physics/PhysicsModifier.h
	Physics/PhysicsProxy.cpp
	Physics/PhysicsProxy.h
	Physics/PhysicsRenderer.cpp
	Physics/PhysicsRenderer.h
)

set(CAMARO_RENDERER
	Renderer/DX/DXDebugRenderer.cpp
	Renderer/DX/DXDebugRenderer.h
	Renderer/DX/DXRenderer.cpp
	Renderer/DX/DXRenderer.h
	Renderer/DX/DXRenderProxy.cpp
	Renderer/DX/DXRenderProxy.h
	Renderer/DX/DXRenderQueue.cpp
	Renderer/DX/DXRenderQueue.h
	Renderer/Renderer.h
)

set(CAMARO_SYSTEM
	System/Memory/MemoryAllocator.cpp
	System/Memory/MemoryAllocator.h
	System/Threading/Thread.cpp
	System/Threading/Thread.h
	System/Clock.cpp
	System/Clock.h
	System/God.cpp
	System/God.h
	System/Log.cpp
	System/Log.h
)

source_group(Common FILES ${CAMARO_COMMON})
source_group(Game FILES ${CAMARO_GAME})
source_group(Input FILES ${CAMARO_INPUT})
source_group(Launcher FILES ${CAMARO_LAUNCHER})
source_group(Physics FILES ${CAMARO_PHYSICS})
source_group(Renderer FILES ${CAMARO_RENDERER})
source_group(System FILES ${CAMARO_SYSTEM})

set(CAMARO_ALL
	${CAMARO_COMMON}
	${CAMARO_GAME}
	${CAMARO_INPUT}
	${CAMARO_LAUNCHER}
	${CAMARO_PHYSICS}
	${CAMARO_RENDERER}
	${CAMARO_SYSTEM}
)

include_directories(
	./
	$ENV{DXSDK_DIR}Include
	${CMAKE_SOURCE_DIR}/ThirdParty/Box2D/Box2D
)

link_directories(
	$ENV{DXSDK_DIR}Lib/x64
	${CMAKE_BINARY_DIR}/ThirdParty/Box2D/Box2D/Box2D/
)

link_libraries(
	Box2D.lib
)

add_executable(Camaro ${CAMARO_ALL})

add_dependencies(Camaro Box2D)

set_target_properties(Camaro PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(Camaro PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
set_target_properties(Camaro PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(Camaro PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
