#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> //for matrix transformation functions

#include "glslprogram.h"
#include "TextureManager.h"
#include "Config.h"
#include "Joystick.h"
#include "Block.h"
#include <iostream>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>

//this macro helps calculate offsets for VBO stuff
//Pass i as the number of bytes for the offset, so be sure to use sizeof() 
//to help calculate bytes accurately.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//lighting
struct Light
{
	glm::vec3 position;
	glm::vec3 colour;
	float power;
};

std::vector<Block*> blockList;

GLFWwindow* window = NULL;

Configuration configuration;

Joystick joystick;

glm::vec3 cameraPosition;
float cameraPitch = 0, cameraYaw = 0;
glm::vec4 cameraDir;
float scalingMovement = 6.f;
float scalingRotation = 2.f;

// from https://www.3dgep.com/understanding-the-view-matrix/
// Pitch should be in the range of [-90 ... 90] degrees and yaw
// should be in the range of [0 ... 360] degrees.
glm::mat4 FPSViewRH(glm::vec3 eye, float pitch, float yaw)
{
	float cosPitch = cos(pitch);
	float sinPitch = sin(pitch);
	float cosYaw = cos(yaw);
	float sinYaw = sin(yaw);

	glm::vec3 xaxis = glm::vec3(cosYaw, 0, -sinYaw);
	glm::vec3 yaxis = glm::vec3(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
	glm::vec3 zaxis = glm::vec3(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	glm::mat4 viewMatrix = glm::mat4(
		glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0),
		glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0),
		glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0),
		glm::vec4(-glm::dot(xaxis, eye), -glm::dot(yaxis, eye), -glm::dot(zaxis, eye), 1)
	);

	return viewMatrix;
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	if (!LoadConfig(configuration))
	{
		std::cout << "ERROR reading the config file!\n\n";
	}
	else
		DisplayConfig(configuration);

	glfwSetErrorCallback(error_callback);

	if(!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(800, 600, "Context example", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);// now we have an OpenGL context for this thread.

	//use GLEW to initialiaze modern opengl functionality supported by the GPU drivers
	glewInit();

	//joystick stuff
	joystick.Init();

	//Make Texture Manager
	TextureManager texManager;

	//load shaders
	GLSLProgram shaders;
	shaders.compileShaderFromFile("vertex.glsl", GLSLShader::GLSLShaderType::VERTEX);
	shaders.compileShaderFromFile("fragment.glsl", GLSLShader::GLSLShaderType::FRAGMENT);

	//bind attributes for the shader layout BEFORE linking the shaders!
	//We only need to do this if we can't use the layout command from within the shader code.
	shaders.bindAttribLocation(0, "position");
	shaders.bindAttribLocation(1, "normal");
	shaders.bindAttribLocation(2, "texUV");

	shaders.link();
	shaders.use();

	//MESH LOAD
	//mesh = new Mesh(&texManager, &shaders, "skull.s3d");
	
	blockList.push_back(new Block(&shaders, vec3(0.f, 0.f, -10.f), vec3(1.f, 1.f, 1.f),
		vec3(1.f, 0.f, 0.f)));

	blockList.push_back(new Block(&shaders, vec3(0.f, -2.f, -10.f), vec3(2.f, 2.f, 1.f),
		vec3(0.f, 0.f, 1.f)));

	glfwSetKeyCallback(window, key_callback);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//turn on single-sided polygons
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	//Primitive restart setup for triangle strips
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF); //maximun value for a USHORT

	//view matrix to transform the camera
	glm::mat4 viewMatrix;
	//projection matrix to project the scene onto the monitor
	glm::mat4 projectionMatrix;

	//...and an accumulator for rotatation:
	float angle = 0.f;

	//timer vars
	double currentTime = glfwGetTime();
	double previousTime = currentTime;
	float timePassed;

	cameraPosition = glm::vec3(0, 0, 0);

	//location of the mesh
	//glm::vec3 meshPosition(0.0f, 0.0f, -3.f);
	//mesh->position = meshPosition;
	
	
	//lights
	std::vector<Light> lights;
	Light light1;
	
	//light #1
	light1.position = glm::vec3(-30, -20, 0);
	light1.colour = glm::vec3(0.1f, 0.2f, 0.5f);
	light1.power = 1800.f;
	lights.push_back(light1);

	//light #2
	light1.position = glm::vec3(30, 20, 50);
	light1.colour = glm::vec3(0.9f, 0.8f, 0.7f);
	light1.power = 9000.f;
	lights.push_back(light1);
	std::vector<Block> blocklist;

	// args: vec3 for position, vec3 for height, width, depth, vec3 for RGB colour
	Block temp(
		vec3(1.0f, 3.5f, 1.5f),
		vec3(0.2f, 1.0f, 0.2f),
		vec3(1.0f, 0.0f, 0.5f)
	);


	temp.colour = vec3(1.0f, 0.0f, 0.5f);

	blocklist.push_back(temp);

	
	while(!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		timePassed = static_cast<float>(currentTime - previousTime);
		if (timePassed > 0) previousTime = currentTime;

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update projection (needed if window size can be modified)
		projectionMatrix = glm::perspective(45.0f, ratio, 0.1f, 10000.f);

		//send the matrix to the shader
		shaders.setUniform("projectionMatrix", projectionMatrix);

		joystick.DoJoystick(); //handle joystick polling
							   //process joystick values
		joystick.ProcessJoystickAxis(joystick.joystickPositionAxis1, configuration);
		joystick.ProcessJoystickAxis(joystick.joystickPositionAxis2, configuration);
		joystick.ProcessJoystickAxis(joystick.joystickPositionAxis4, configuration);
		joystick.ProcessJoystickAxis(joystick.joystickPositionAxis3, configuration);


		cameraYaw += joystick.joystickPositionAxis3 * scalingRotation * timePassed * configuration.axis_yaw;
		cameraPitch += joystick.joystickPositionAxis4 * scalingRotation * timePassed * configuration.axis_pitch;

		//move ahead
		cameraDir = glm::vec4(0, 0, -1, 0);
		viewMatrix = glm::mat4(1.f);
		viewMatrix = glm::rotate(viewMatrix, cameraYaw, glm::vec3(0, 1.f, 0));
		viewMatrix = glm::rotate(viewMatrix, cameraPitch, glm::vec3(1.f, 0, 0));
		cameraDir = viewMatrix * cameraDir;

		cameraPosition -= glm::vec3(cameraDir) * scalingMovement * joystick.joystickPositionAxis2 * (float)timePassed * configuration.axis_move;

		//strafe
		cameraDir = glm::vec4(1, 0, 0, 0);
		viewMatrix = glm::mat4(1.f);
		//viewMatrix = glm::rotate(viewMatrix, cameraPitch, glm::vec3(1.f, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, cameraYaw, glm::vec3(0, 1.f, 0));
		cameraDir = viewMatrix * cameraDir;

		cameraPosition += glm::vec3(cameraDir) * scalingMovement * joystick.joystickPositionAxis1 * (float)timePassed * configuration.axis_strafe;


		//update camera position via the view matrix
		viewMatrix = FPSViewRH(cameraPosition, cameraPitch, cameraYaw);

		//send the matrix to the shader
		shaders.setUniform("viewMatrix", viewMatrix);

		//update angle
		angle += timePassed * 1;
		//mesh->angle = angle;
		//mesh->Update(timePassed);

		//lighting
		shaders.setUniform("numLights", (int)lights.size());
		for (int i = 0; i < lights.size(); ++i)
		{
			std::stringstream ss;
			ss << "lightArray[" << i << "].";
			std:string lightString;

			//light position	
			lightString = ss.str() + "position";
			shaders.setUniform(lightString.c_str(), lights[i].position);
			//light colour
			lightString = ss.str() + "colour";
			shaders.setUniform(lightString.c_str(), lights[i].colour);
			//light power
			lightString = ss.str() + "power";
			shaders.setUniform(lightString.c_str(), lights[i].power);
		}
		//material shinyness
		shaders.setUniform("shinyness", 10.0f);
		//material specular "power" : increasing this focuses the specular reflection size
		shaders.setUniform("specularPower", 200.f);
		
		for(auto &B: blockList)
			B->Draw();
		
		

		glfwSwapBuffers(window); //display the graphics buffer to the screen
		glfwPollEvents(); //prime the message pump that GLFW uses for input events
	}

	for (auto& B : blockList)
		delete B;

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}