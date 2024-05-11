#pragma once
#include "glslprogram.h"

class Triforce
{
	GLuint vboID, vaoID;
public:
	GLSLProgram *shaders;
	glm::mat4 modelMatrix;
	//...and an accumulator for rotatation:
	float angle = 0.f;
	float rotationSpeed = 0.f;
	glm::vec3 axis = glm::vec3(0.f, 1.f, 0.f);

	void Update(float seconds);
	void Draw();
	Triforce(GLSLProgram* theShaderProgram);
	~Triforce();
};