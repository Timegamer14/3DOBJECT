#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> //for matrix transformation functions

#include "glslprogram.h"
#include "TextureManager.h"
#include "Cube.h"

#include <stdlib.h>
#include <stdio.h>

//this macro helps calculate offsets for VBO stuff
//Pass i as the number of bytes for the offset, so be sure to use sizeof() 
//to help calculate bytes accurately.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

std::string fileName = "cube.png";

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(800, 600, "Context example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);// now we have an OpenGL context for this thread.

	//use GLEW to initialiaze modern opengl functionality supported by the GPU drivers
	glewInit();

	//Make Texture Manager
	TextureManager texManager;

	//load shaders
	GLSLProgram shaders;
	shaders.compileShaderFromFile("vertex.glsl", GLSLShader::GLSLShaderType::VERTEX);
	shaders.compileShaderFromFile("fragment.glsl", GLSLShader::GLSLShaderType::FRAGMENT);

	//bind attributes for the shader layout BEFORE linking the shaders!
	//We only need to do this if we can't use the layout command from within the shader code.
	shaders.bindAttribLocation(0, "position");
	shaders.bindAttribLocation(1, "texUV");

	shaders.link();
	shaders.use();
	Cube cube(fileName, &texManager, &shaders);

	glfwSetKeyCallback(window, key_callback);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//Primitive restart setup for triangle strips
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF); //maximun value for a USHORT

	//view matrix to transform the camera
	glm::mat4 viewMatrix;
	//projection matrix to project the scene onto the monitor
	glm::mat4 projectionMatrix;

	//timer vars
	double currentTime = glfwGetTime();
	double previousTime = currentTime;
	double timePassed;

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update projection (needed if window size can be modified)
		projectionMatrix = glm::perspective(45.0f, ratio, 0.1f, 10000.f);

		//send the matrix to the shader
		shaders.setUniform("projectionMatrix", projectionMatrix);

		//update camera position via the view matrix
		viewMatrix = glm::lookAt(
			glm::vec3(-1.f, 1.f, 1.f), //where is the EYE of the camera
			cube.position, //a point we are looking at 
			glm::vec3(0.f, 1.f, 0.f) // up vector of the camera
		);

		//send the matrix to the shader
		shaders.setUniform("viewMatrix", viewMatrix);
		double currentTime = glfwGetTime();
		timePassed = currentTime - previousTime;
		if (timePassed > 0) previousTime = currentTime;
		cube.Update((float)timePassed);
		cube.Draw();
		glfwSwapBuffers(window); //display the graphics buffer to the screen
		glfwPollEvents(); //prime the message pump that GLFW uses for input events
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}