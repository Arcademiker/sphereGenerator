# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (sphereGenerator)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenGL REQUIRED)
#find_package(SOIL REQUIRED)


# Compile external dependencies
add_subdirectory (external)

#windows path to SOIL
#link_directories("c:/soil-master/build")

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


# User playground
add_executable(playground
	playground/main.cpp
	playground/sphereGenerator.cpp
	playground/Triangle.cpp
	playground/Triangle.h
	playground/TriangleTesselation.cpp
	playground/TriangleTesselation.cpp
	playground/TransformVertexShader.vertexshader
	playground/TextureFragmentShader.fragmentshader
	playground/ModelObject.cpp
	playground/ModelObject.h
	playground/Camera.cpp
	playground/Camera.h
    common/shader.cpp
    common/shader.hpp
    common/controls.cpp
    common/controls.hpp playground/Graph.cpp playground/Graph.h playground/AStar.cpp playground/AStar.h playground/ExplorationAgenda.cpp playground/ExplorationAgenda.h)

#libSOIL.a for loading textures
target_link_libraries(playground
	${ALL_LIBS} libSOIL.a
)



# Xcode and Visual working directories
#set_target_properties(playground PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/playground/")
#create_target_launcher(playground WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/playground/")





