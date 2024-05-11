#include "Cube.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Cube::Draw()
{
	//send the modelMatrix to the shader
	shader->setUniform("modelMatrix", modelMatrix);

	glBindVertexArray(vaoID);

	//bind the texture
	tManager->BindTexture(texID);

	// Draw the triangle !
	glDrawElements(GL_TRIANGLE_STRIP, 100, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

void Cube::Update(float seconds)
{
	//update angle

	angle += seconds * 2;

	//calculate model matrix 
	modelMatrix = glm::mat4(1.f); //identity matrix
	//note the order of operations here: translate THEN rotate!
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.f, 0.f, 1.f));
}

Cube::Cube(std::string textureFileName, TextureManager* tManager, GLSLProgram* shaders)
{
	this->shader = shaders;
	this->tManager = tManager;

	texID = tManager->LoadTexture(textureFileName);
	assert(texID != 0);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	GLfloat g_vertex_buffer_data[] =
	{
		// 0				// u   v
		-0.5f, 0.5f, 0.0f,	0.25f,	0.66f,
		// 1
		-0.5f, -0.5f, 0.0f,	0.25f,	0.34f,
		// 2
		0.5f, 0.5f, 0.0f,	0.5f,	0.66f,
		// 3
		0.5f, -0.5f, 0.0f,	0.5f,	0.34f,
		// 4
		0.5f, -0.5f, -1.f,	0.75f,	0.34f,
		// 5
		0.5f, 0.5f, -1.f,	0.75f,	0.66f,
		//6
		-0.5f, -0.5f, -1.f, 1.00f,	0.33f,
		//7
		-0.5f, 0.5f, -1.f,	1.00f,	0.66f,
		//8 // left side,
		-0.5f, 0.5f, -1.0f, 0.00f,	0.66f,
		//9
		-0.5f, -0.5f, -1.f, 0.00f,	0.33f,
		//10
		0.5f, 0.5f, -1.0f,	0.5f,	1.00f,
		//11
		-0.5f, 0.5f, -1.0f,	0.25f, 1.00f,
		//12
		-0.5f, -0.5f, -1.0f, 0.25f,	0.f,
		//13
		0.5f, -0.5f, -1.0f,	0.5f,	0.f,
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
		5 * sizeof(GLfloat),  // stride
		(void*)0            // array buffer offset
	);

	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		5 * sizeof(GLfloat),  // stride
		BUFFER_OFFSET(sizeof(GLfloat) * 3)            // array buffer offset
	);

	glEnableVertexAttribArray(0); //channel 0
	glEnableVertexAttribArray(1); //channel 1

	//indices
	GLushort g_indice_buffer_data[] =
	{
		8,9,		// left
		0,1,2,		//front
		3,5,4,		//right
		7,6,		//back
		0xFFFF,		// primitive restart code
		10,11,2,	// top
		0,3,1,		// front
		13,12		// bottom
	};

	// Generate 1 buffer, put the resulting identifier in indexbuffer
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indice_buffer_data), g_indice_buffer_data, GL_STATIC_DRAW);

	glBindVertexArray(0); // Disable our Vertex Array Object? 

	glBindBuffer(GL_ARRAY_BUFFER, 0);// Disable our Vertex Buffer Object

	return;
}

Cube::~Cube()
{
	// delete VBO when object destroyed
	glDeleteBuffers(1, &vboID);
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &eboID);
}