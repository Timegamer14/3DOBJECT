#include "Triforce.h"

//#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> //for matrix transformation functions


Triforce::Triforce(GLSLProgram* theShaderProgram)
{
	shaders = theShaderProgram;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] =
	{
		//triangle #0
		-0.5f, -0.5f, 0.1f, //lower left corner
		0.5f, -0.5f, 0.1f, //lower right corner
		0.0f, 0.5f, 0.1f, //top corner

		//triangle #0
		-0.5f, -0.5f, -0.1f, //backside, lower left corner
		0.0f, 0.5f, -0.1f, //backside, top corner
		0.5f, -0.5f, -0.1f, //backside, lower right corner

		//triangle#0 - left edge of triforce 
		0.0f, 0.5f, 0.1f,
		0.0f, 0.5f, -0.1f,
		-0.5f, -0.5f, 0.1f,

		//triangle #0 - left edge of triforce
		0.0f, 0.5f, -0.1f,
		-0.5f, -0.5f, -0.1f,
		-0.5f, -0.5f, 0.1f,

		// Triangle #0 - Right edge of triforce
		0.0f, 0.5f, 0.1f,
		0.0f, 0.5f, -0.1f,
		0.5f, -0.5f, 0.1f,   // changed x-coordinate from -0.5f to 0.5f

		// Triangle #0 - Right edge of triforce
		0.0f, 0.5f, -0.1f,
		0.5f, -0.5f, -0.1f,  // changed x-coordinate from -0.5f to 0.5f
		0.5f, -0.5f, 0.1f,

		// Adjusted triangle #1
		-1.0f, -1.5f, 0.1f, // lower left corner
		 0.0f, -1.5f, 0.1f, // lower right corner
		  -0.5f, -0.5f, 0.1f, // top corner

		  //triangle #1
		  -1.0f, -1.5f, -0.1f, //backside, lower left corner
			-0.5f, -0.5f, -0.1f, //backside, top corner
			0.0f, -1.5f, -0.1f, //backside, lower right corner

			//triangle#1 - left edge of triforce 
	   0.0f, -1.5f, 0.1f,
	   0.0f, -1.5f, -0.1f,
		-0.5f, -0.5f, 0.1f,

		//triangle #1 - left edge of triforce
		0.0f, -1.5f, -0.1f,
	   -0.5f, -0.5f, -0.1f,
		-0.5f, -0.5f, 0.1f,
		
			

	   // Adjusted triangle #2
		 0.0f, -1.5f, 0.1f, // lower left corner
		 1.0f, -1.5f, 0.1f, // lower right corner
		  0.5f, -0.5f, 0.1f, // top corner

		  //triangle #2
		  0.0f, -1.5f, -0.1f, //backside, lower left corner - adjusted
		  1.0f, -1.5f, -0.1f, //backside, lower right corner - adjusted
		0.5f, -0.5f, -0.1f, //backside, top corner - adjusted


		


 // Triangle #2 - Right edge of triforce
  0.5f, -0.5f, 0.1f,
  0.5f, -0.5f, -0.1f,
  1.0f, -1.5f, 0.1f,

  // Triangle #2 - Right edge of triforce
  0.5f, -0.5f, -0.1f,
  1.0f, -1.5f, -0.1f,
  1.0f, -1.5f, 0.1f,



	};

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vboID);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0); // Disable our Vertex Array Object? 

	glBindBuffer(GL_ARRAY_BUFFER, 0);// Disable our Vertex Buffer Object

	return;
}

Triforce::~Triforce()
{
	// delete VBO when object destroyed
	glDeleteBuffers(1, &vboID);
	glDeleteVertexArrays(1, &vaoID);
}

void Triforce::Draw()
{
	glEnable(GL_DEPTH_TEST);  // Ensure depth testing is enabled
	//glEnable(GL_CULL_FACE);   // Ensure face culling is enabled

	shaders->setUniform("modelMatrix", modelMatrix);

	glBindVertexArray(vaoID);
	glDrawArrays(GL_TRIANGLES, 0, 72);  // Assuming your triforce consists of 12 vertices
	glBindVertexArray(0);

	glDisable(GL_CULL_FACE);  // Optionally disable face culling after drawing
}

void Triforce::Update(float seconds)
{
	angle += seconds * rotationSpeed;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();

	modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.f, 1.f, 0.f));
}