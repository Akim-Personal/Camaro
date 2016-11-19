cmake_minimum_required (VERSION 2.6)
project (Camaro CXX C)

include(CMakeFileList.cmake)

##################################################################
# Generate VS filter list
foreach(source IN LISTS camaro_files)
    get_filename_component(source_path ${source} DIRECTORY)
    string(REPLACE "/" "\\" source_path_msvc ${source_path})
    source_group(${source_path_msvc} FILES ${source})
endforeach()

##################################################################
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

##################################################################
# Create Project
add_executable(Camaro ${camaro_files})

# Build order
add_dependencies(Camaro Box2D)

##################################################################
# Project properties
set_target_properties(Camaro PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(Camaro PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
set_target_properties(Camaro PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(Camaro PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
