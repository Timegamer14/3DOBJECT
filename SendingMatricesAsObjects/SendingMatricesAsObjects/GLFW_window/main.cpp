#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glslprogram.h"

#include <stdlib.h>
#include <stdio.h>

#include "Triforce.h"

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
	GLFWwindow* window;

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


	//load shaders
	GLSLProgram shaders;
	shaders.compileShaderFromFile("vertex.glsl", GLSLShader::GLSLShaderType::VERTEX);
	shaders.compileShaderFromFile("fragment.glsl", GLSLShader::GLSLShaderType::FRAGMENT);
	shaders.link();
	shaders.use();

	glfwSetKeyCallback(window, key_callback);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//turn on single-sided triangles
	glEnable(GL_CULL_FACE);
	
	//timer vars
	double currentTime = glfwGetTime();
	double previousTime = currentTime;
	double timePassed;

	Triforce* tri = new Triforce(&shaders);
	tri->rotationSpeed = 1.5f;
	tri->axis = glm::vec3(1.f, 0.f, 0.f);

	while(!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update angle
		currentTime = glfwGetTime();
		timePassed = currentTime - previousTime;
		if(timePassed > 0) previousTime = currentTime;

		tri->Update((float)timePassed);
		
		tri->Draw();
		
		glfwSwapBuffers(window); //display the graphics buffer to the screen
		glfwPollEvents(); //prime the message pump that GLFW uses for input events
	}

	delete tri;

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}