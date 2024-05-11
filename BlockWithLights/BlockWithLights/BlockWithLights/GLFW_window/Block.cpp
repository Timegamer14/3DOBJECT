#include "Block.h"

//this macro helps calculate offsets for VBO stuff
//Pass i as the number of bytes for the offset, so be sure to use sizeof() 
//to help calculate bytes accurately.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Block::Draw()
{
	//send the colour to the shader
	shader->setUniform("rgb", colour);

	//send the modelMatrix to the shader
	shader->setUniform("modelMatrix", modelMatrix);

	glBindVertexArray(vaoID);
	// Draw the triangle !
	glDrawElements(GL_TRIANGLE_STRIP, NumIndices, GL_UNSIGNED_SHORT,NULL); // Starting from vertex 0; 6 vertices total -> 2 triangles
	glBindVertexArray(0);

}

Block::Block(GLSLProgram* shaders, glm::vec3 positionInfo, 
	glm::vec3 sizeInfo, glm::vec3 colours)
{
	//calculate model matrix 
	modelMatrix = glm::mat4(1.f); //identity matrix
	//note the order of operations here: translate THEN rotate!
	modelMatrix = glm::translate(modelMatrix, positionInfo);

	shader = shaders;
	colour = colours;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	GLfloat g_vertex_buffer_data[] =
	{
		//******* FRONT FACE *********
		//0 lower left front
		-0.5f * sizeInfo.x, -0.5f * sizeInfo.y, 0.5f * sizeInfo.z,
		0.f, 0.f, 1.f,
		//1 lower right front
		0.5f * sizeInfo.x, -0.5f * sizeInfo.y, 0.5f * sizeInfo.z,
		0.f, 0.f, 1.f,
		//2 upper left front
		-0.5f * sizeInfo.x, 0.5f * sizeInfo.y, 0.5f * sizeInfo.z,
		0.f, 0.f, 1.f,
		//3 upper right front
		0.5f * sizeInfo.x, 0.5f * sizeInfo.y, 0.5f * sizeInfo.z,
		0.f, 0.f, 1.f,

		//******* TOP FACE *********
		//4 upper left fronttop
		-0.5f * sizeInfo.x, 0.5f * sizeInfo.y, 0.5f * sizeInfo.z,
		0.f, 1.f, 0.f,
		//5 upper right fronttop
		0.5f * sizeInfo.x, 0.5f * sizeInfo.y, 0.5f * sizeInfo.z,
		0.f, 1.f, 0.f,
		//6 upper left backtop
		-0.5f * sizeInfo.x, 0.5f * sizeInfo.y, -0.5f * sizeInfo.z,
		0.f, 1.f, 0.f,
		//7 upper right backtop
		0.5f * sizeInfo.x, 0.5f * sizeInfo.y, -0.5f * sizeInfo.z,
		0.f, 1.f, 0.f,
	};	

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vboID);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//POSITION data
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		6 * sizeof(GLfloat),  // stride
		(void*)0            // array buffer offset
	);

	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		6 * sizeof(GLfloat),  // stride
		BUFFER_OFFSET(sizeof(GLfloat) * 3)	// array buffer offset
	);

	//indices
	GLushort g_indice_buffer_data[] =
	{
		0,1,2,3, //front
		0xFFFF,
		4,5,6,7, //top
	};
	NumIndices = 9;

	// Generate 1 buffer, put the resulting identifier in indexbuffer
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indice_buffer_data), g_indice_buffer_data, GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(0); //channel 0
	glEnableVertexAttribArray(1); //channel 0
	
	glBindVertexArray(0); // Disable our Vertex Array Object? 

	glBindBuffer(GL_ARRAY_BUFFER, 0);// Disable our Vertex Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// Disable our Element Buffer
}

Block::~Block()
{
	// delete VBO when object destroyed
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &eboID);
	glDeleteVertexArrays(1, &vaoID);

}