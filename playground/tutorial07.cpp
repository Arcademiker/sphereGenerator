//todo:
//class shader
//class model .buffer model.draw
//class fenser, steuerung
//add noise
//tesselate only visible area in specific range?




// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//sphereGenerator includes:
#include "stdafx.h"
#include "TriangleTesselation.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include "SOIL.h"

void loadImage_SOIL(GLuint* textures,const char* imagepath, unsigned int texIndex);


int main( )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    int resolutionWidth = 1600;
    int resolutionHeight = 1200;
	window = glfwCreateWindow( resolutionWidth, resolutionHeight, "Tutorial 07 - Model Loading", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, resolutionWidth/2, resolutionHeight/2);

	// Light background
	//glClearColor(0.9f, 1.0f, 0.9f, 0.0f);
	// Dark background
	glClearColor(0.1f, 0.0f, 0.1f, 0.0f);



	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../playground/TransformVertexShader.vertexshader", "../playground/TextureFragmentShader.fragmentshader" );

    // Get a handle for our "P" uniform
    //GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    // Get a handle for our "MV" uniform
    GLuint MatrixID1 = glGetUniformLocation(programID, "MV");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID2 = glGetUniformLocation(programID, "MVP");



    GLuint textures[2];
    glGenTextures(2, textures);
    // Load the heightmap
    //loadImage_SOIL(textures,"../playground/marsheight.png",0);
    loadImage_SOIL(textures,"../playground/earthHeightmap8k.png",0);

	// Get a handle for our "myTextureSampler" uniform
	GLuint HeightmapID  = glGetUniformLocation(programID, "myHeightmap");

    // Load the texture
    //loadImage_SOIL(textures,"../playground/marstexture.jpg",1);
    loadImage_SOIL(textures,"../playground/earthDiffuse8k.jpg",1);
    GLuint TextureID  = glGetUniformLocation(programID, "myTexture");


    //load normalmap
    loadImage_SOIL(textures,"../playground/earthNormal8k.png",2);
    GLuint NormalID  = glGetUniformLocation(programID, "myNormal");

    /// generate sphere object:
    CTriangleTesselation TriangleTesselation(0.5f);
    TriangleTesselation.Tesselate(5);
    const std::vector<CTriangle>* triangles = TriangleTesselation.GetTriangleList();

    std::cout << "shown faces: " << triangles->size() ;
    //std::vector<CTriangle> tmp = *triangles;
    //std::cout << tmp.at(0).GetPoint1()->fY << std::endl;

    // Read our .obj file
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	//bool res = loadOBJ("../playground/cube.obj", vertices, uvs, normals);
    ///bool res = loadOBJ(triangles, vertices, uvs, normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, triangles->size() * sizeof(CTriangle), &triangles->at(0), GL_STATIC_DRAW);

	//GLuint uvbuffer;
	//glGenBuffers(1, &uvbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, triangles->size() * sizeof(CTriangle), &triangles->at(0), GL_STATIC_DRAW);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		//glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glm::mat4 MV =  ViewMatrix ;//* ModelMatrix;
		glm::mat4 MVP = ProjectionMatrix * MV ;

		// Send our transformation to the currently bound shader,
        glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &MV[0][0]);
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		// Set our "myHeightmap" sampler to use Texture Unit 0
		glUniform1i(HeightmapID, 0);
        //glUniform1i( glGetUniformLocation( programID, "myTextureSampler" ), 0 );
        // Bind our texture in Texture Unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        // Set our "myTexture" sampler to use Texture Unit 0
        glUniform1i(TextureID, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		// Set our "myTexture" sampler to use Texture Unit 0
		glUniform1i(NormalID, 2);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(CTriangle::SPoint3D),    // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
        int* pAttributPointer = reinterpret_cast<int*>(sizeof(float)*3);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
            sizeof(CTriangle::SPoint3D),                  // stride
            pAttributPointer                  // array buffer offset
		);
        // 2nd attribute buffer : normalen
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            2,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_TRUE,           // normalized?
            sizeof(CTriangle::SPoint3D),    // stride
            (void*)0            // array buffer offset
        );
        // 2nd attribute buffer : tangenten
		pAttributPointer = reinterpret_cast<int*>(sizeof(float)*6);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
				3,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_TRUE,           // normalized?
				sizeof(CTriangle::SPoint3D),    // stride
				pAttributPointer           // array buffer offset
		);


		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, triangles->size()* 3 * 10 );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, textures);
	glDeleteVertexArrays(1, &VertexArrayID);
    //SOIL_free_image_data(TextureID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}



void loadImage_SOIL(GLuint* textures,const char* imagepath, unsigned int texIndex) {

    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, textures[texIndex]);

    image = SOIL_load_image(imagepath, &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (0 == image)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }
    SOIL_free_image_data(image);
    //glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glGenerateMipmap(GL_TEXTURE_2D);
}

/*
    GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    Gluint texture = SOIL_load_OGL_texture
            (
                    "earthheightmap.png",
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT
            );
 */

/*//GLuint Texture = loadDDS("../playground/uvmap.DDS");
 *
 *
 * 	// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(float)*5,    // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
            sizeof(float)*2,                  // stride
            (void*)0                          // array buffer offset
		);

 */

/*
GLuint loadImage_SOIL(const char * imagepath){

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Read the file, call SOIL with the right parameters
    int width, height;
    unsigned char* image =
            SOIL_load_image(imagepath, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if (0 == image)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }

	// Nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}
 */