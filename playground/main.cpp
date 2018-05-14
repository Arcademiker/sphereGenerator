//todo:
//class shader
//class model .buffer model.draw
//class fenser, steuerung
//tesselate only visible area in specific range?
//--capsule: texture load bind, matrices, attribute pointer
//--cam controller check




// Include standard headers
#include <cstdio>
//#include <stdlib.h>
//#include <vector>

//sphereGenerator includes:
#include "TriangleTesselation.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

#include <common/shader.hpp>
//#include <common/texture.hpp>
#include <common/controls.hpp>
//#include <common/objloader.hpp>

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

#include <iostream>
//#include <GL/glxew.h> //for vsync
//#include <GL/glx.h>
#include "SOIL.h"
#include "Camera.h"
#include "Graph.h"

void loadImage_SOIL(GLuint* textures,const char* imagepath, unsigned int texIndex);
void updateVPMatFromInput(CCamera* pCamera, float speed);

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
    int resWidth = 1920;
    int resHeight = 1080;
	window = glfwCreateWindow( resWidth, resHeight, "Tutorial 07 - Model Loading", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible."
                " Try the 2.1 version of the tutorials.\n" );
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

    /*
    //activating vsync
    Display *dpy = glXGetCurrentDisplay();
    GLXDrawable drawable = glXGetCurrentDrawable();
    const int interval = 1;
    if (drawable) {
        glXSwapIntervalEXT(dpy, drawable, interval);
    }
    */
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    ///controller cam-->
	/*
    float horizontalAngle = 0;//-0.5f*3.14f;
    // Initial vertical angle : none
    float verticalAngle = 0;//3.14f+0.5f;//2*3.14f;
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
    );
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.1415f/2.0f),
            0,
            cos(horizontalAngle - 3.1415f/2.0f)
    );
    glm::vec3 up = glm::cross( right, direction );
    float aspectRatio = static_cast<float>(resWidth)/static_cast<float>(resHeight);
    CCamera pCamera(glm::radians(45.0f), aspectRatio,0.001f, 5.0f, glm::vec3( 0, 0, 0 ),direction,up );
	*/
    ///<--cam

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, resWidth/2, resHeight/2);

	// Light background
	//glClearColor(0.9f, 1.0f, 0.9f, 0.0f);
	// Dark background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);



	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glDisable(GL_CULL_FACE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../playground/TransformVertexShader.vertexshader", "../playground/TextureFragmentShader.fragmentshader" );

    // Get a handle for our "P" uniform
    //GLuint MatrixID0 = glGetUniformLocation(programID, "M");
    // Get a handle for our "MV" uniform
    GLuint MatrixID1 = glGetUniformLocation(programID, "MV");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID2 = glGetUniformLocation(programID, "MVP");

	GLuint MatrixID3 = glGetUniformLocation(programID, "ViewMatrix");



    GLuint textures[5];
    glGenTextures(5, textures);
    // Load the heightmap
    //loadImage_SOIL(textures,"../playground/marsheight.png",0);
    loadImage_SOIL(textures,"../playground/earthHeightmap8k.png",0);
	//loadImage_SOIL(textures,"../playground/marsheightwater.png",0);

	// Get a handle for our "myTextureSampler" uniform
	GLuint HeightmapID  = glGetUniformLocation(programID, "myHeightmap");

    // Load the texture
    //loadImage_SOIL(textures,"../playground/marstexture.jpg",1);
    //loadImage_SOIL(textures,"../playground/earthDiffuse8kice.jpg",1);
    loadImage_SOIL(textures,"../playground/earth16kiceblue.jpg",1);
	//loadImage_SOIL(textures,"../playground/marstexturemareblue2.jpg",1);
    GLuint TextureID  = glGetUniformLocation(programID, "myTexture");


    //load normalmap
    loadImage_SOIL(textures,"../playground/earthNormal8k.png",2);
	//loadImage_SOIL(textures,"../playground/normalmars4.png",2);
    GLuint NormalID  = glGetUniformLocation(programID, "myNormal");

	//load specular
	loadImage_SOIL(textures,"../playground/earthSpecular8kfinal.png",3);
	//loadImage_SOIL(textures,"../playground/marsspecular.png",3);
	GLuint SpecularID  = glGetUniformLocation(programID, "mySpecular");

    //load nighttexture
    loadImage_SOIL(textures,"../playground/earth16cities4.jpg",4);
    GLuint Texture2ID  = glGetUniformLocation(programID, "myTexture2");

    /// generate sphere object:
    CTriangleTesselation TriangleTesselation(0.5f,0);
    //TriangleTesselation.Tesselate(1);
    const std::vector<CTriangle>* triangles = TriangleTesselation.GetTriangleList();

    std::cout << "shown faces: " << triangles->size();

    ///test Graph
    //TriangleTesselation.GetGraph()->printGraph();

    //std::vector<CTriangle> tmp = *triangles;
    //std::cout << tmp.at(0).GetPoint1()->fY << std::endl;

    // Read our .obj file
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec2> uvs;
	//std::vector<glm::vec3> normals; // Won't be used at the moment.
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

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


    glm::mat4 ModelMatrix = glm::mat4();
    glm::mat4 ViewMatrix = glm::mat4();
    glm::mat4 ProjectionMatrix = glm::mat4();
	//ModelMatrix = glm::rotate( ModelMatrix,3.141592f,glm::vec3(0.0f,0.0f,1.0f));
    ///camera ini

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		ViewMatrix =  getViewMatrix();
		ProjectionMatrix = getProjectionMatrix();

		//controller
		///updateVPMatFromInput(&pCamera,0.004f);
		///ProjectionMatrix = pCamera.GetProjectionMatrix();
		///ViewMatrix = pCamera.GetViewMatrix();
        //mat4 rotation;
        //rotation = glm::rotate(2.0f, vec3(0,1,0));

        ///ModelMatrix = glm::rotate( ModelMatrix,0.001f,glm::vec3(0.0f,1.0f,0.0f));

        //glm::rotate(0.1f,glm::vec3(0,1,0));
        glm::mat4 MV =  ViewMatrix * ModelMatrix;
		glm::mat4 MVP = ProjectionMatrix * MV ;



		// Send our transformation to the currently bound shader,
        //glUniformMatrix4fv(MatrixID0, 1, GL_FALSE, &ModelMatrix[0][0]);

        glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &MV[0][0]);
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);

		glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &ViewMatrix[0][0]);

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

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		// Set our "myTexture" sampler to use Texture Unit 0
		glUniform1i(SpecularID, 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textures[4]);
        // Set our "myTexture" sampler to use Texture Unit 0
        glUniform1i(Texture2ID, 4);


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
		pAttributPointer = reinterpret_cast<int*>(sizeof(float)*5);
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

    //glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glGenerateMipmap(GL_TEXTURE_2D);
}



void updateVPMatFromInput(CCamera* pCamera, float speed) {
	glfwPollEvents();
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (1 == present)
	{
		//glm::vec3 modelScale = (vecMODELS[0]->getMaxValues() - vecMODELS[0]->getMinValues());
		float fMoveSpeed = speed;//std::max(modelScale.x, modelScale.y);
		//fMoveSpeed = std::max(fMoveSpeed, modelScale.z);
		//fMoveSpeed = fMoveSpeed * static_cast<float>(dTimeDiff) / 10.0f;


		int axisCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
		//std::cout << axisCount << std::endl;
		//if (abs(axes[0]) > 0.1f) { pCamera->Translate(glm::vec3(-1.0f * axes[0] * fMoveSpeed, 0.0f, 0.0f)); }
		//if (abs(axes[1]) > 0.1f) { pCamera->Translate(glm::vec3(0.0f, 0.0f, 1.0f * axes[1] * fMoveSpeed)); }
		//if (abs(axes[4]) > 0.1f) { pCamera->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f * axes[4] * fMoveSpeed * 10.0f); }
		//if (abs(axes[3]) > 0.1f) { pCamera->AddPitch(-0.5f * axes[3] * fMoveSpeed * 100.0f); }
		//if (abs(axes[2] + 1.0f) > 0.1f) { pCamera->Translate(glm::vec3(0.0f, 1.0f * (axes[2] + 1.0f) * fMoveSpeed, 0.0f)); }
		//if (abs(axes[5] + 1.0f) > 0.1f) { pCamera->Translate(glm::vec3(0.0f, -1.0f * (axes[5] + 1.0f) * fMoveSpeed, 0.0f)); }

		if (fabsf(axes[0]) > 0.1f) { pCamera->Translate(glm::vec3(-1.0f * axes[0] * fMoveSpeed, 0.0f, 0.0f)); }
		if (fabsf(axes[1]) > 0.1f) { pCamera->Translate(glm::vec3(0.0f, 1.0f * axes[1] * fMoveSpeed, 0.0f )); }
		if (fabsf(axes[4]) > 0.1f) { pCamera->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f * axes[4] * fMoveSpeed * 10.0f); }
		if (fabsf(axes[3]) > 0.1f) { pCamera->AddPitch(axes[3] * fMoveSpeed * 180.0f); }
		if (fabsf(axes[2] + 1.0f) > 0.1f) { pCamera->Translate(glm::vec3(0.0f,0.0f,-1.0f * (axes[2] + 1.0f) * fMoveSpeed)); }
		if (fabsf(axes[5] + 1.0f) > 0.1f) { pCamera->Translate(glm::vec3(0.0f,0.0f,1.0f * (axes[5] + 1.0f) * fMoveSpeed)); }
	}
}