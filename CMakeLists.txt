# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (sphereGenerator)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenGL REQUIRED)
#find_package(SOIL REQUIRED)


# Compile external dependencies
add_subdirectory (external)

#link_directories(/usr/local/lib)

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
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
	playground/tutorial07.cpp
	playground/sphereGenerator.cpp
	playground/stdafx.cpp
	playground/stdafx.h
	playground/targetver.h
	playground/Triangle.cpp
	playground/Triangle.h
	playground/TriangleTesselation.cpp
	playground/TriangleTesselation.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	playground/TransformVertexShader.vertexshader
	playground/TextureFragmentShader.fragmentshader
	#playground/SimpleFragmentShader.fragmentshader
	#playground/SimpleVertexShader.vertexshader
		playground/ModelObject.cpp playground/ModelObject.h playground/Camera.cpp playground/Camera.h)
#libSOIL.a for loading textures
target_link_libraries(playground
	${ALL_LIBS} libSOIL.a
)

# Xcode and Visual working directories
#set_target_properties(playground PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/playground/")
#create_target_launcher(playground WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/playground/")





