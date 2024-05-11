#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp> //for matrix transformation functions

#include "glslprogram.h"

class Block
{
private:
	GLuint vaoID;
	GLuint vboID;
	GLuint eboID;
	int NumIndices;
	glm::vec3 colour;
	GLSLProgram *shader;
	
public:
	glm::mat4 modelMatrix;
	void Draw();
	Block(GLSLProgram* shaders, glm::vec3 positionInfo, 
		glm::vec3 sizeInfo, glm::vec3 colours);
	~Block();

};