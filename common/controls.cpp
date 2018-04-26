// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 3 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 2*3.14f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 1.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.001f, 5.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


/*glfwPollEvents();
        int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
        if (1 == present)
        {
            glm::vec3 modelScale = (vecMODELS[0]->getMaxValues() - vecMODELS[0]->getMinValues());
            float fMoveSpeed = std::max(modelScale.x, modelScale.y);
            fMoveSpeed = std::max(fMoveSpeed, modelScale.z);
            fMoveSpeed = fMoveSpeed * static_cast<float>(dTimeDiff) / 10.0f;


            int axisCount;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
            //std::cout << axisCount << std::endl;
            if (abs(axes[0]) > 0.01f) { pCamera->Translate(glm::vec3(-1.0f * axes[0] * fMoveSpeed, 0.0f, 0.0f)); }
            if (abs(axes[1]) > 0.01f) { pCamera->Translate(glm::vec3(0.0f, 0.0f, 1.0f * axes[1] * fMoveSpeed)); }
            if (abs(axes[2]) > 0.01f) { pCamera->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f * axes[2] * fMoveSpeed * 10.0f); }
            if (abs(axes[3]) > 0.01f) { pCamera->AddPitch(-0.5f * axes[3] * fMoveSpeed * 10.0f); }
            if (abs(axes[4] + 1.0f) > 0.01f) { pCamera->Translate(glm::vec3(0.0f, 1.0f * (axes[4] + 1.0f) * fMoveSpeed, 0.0f)); }
            if (abs(axes[5] + 1.0f) > 0.01f) { pCamera->Translate(glm::vec3(0.0f, -1.0f * (axes[5] + 1.0f) * fMoveSpeed, 0.0f)); }
        }
*/