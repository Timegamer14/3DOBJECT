#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> //for matrix transformation functions

#include "glslprogram.h"
#include "TextureManager.h"

class Cube
{
private:
	GLuint vaoID;
	GLuint vboID;
	GLuint texID;
	GLuint eboID;
	std::string textureFileName;
	TextureManager* tManager;
	glm::mat4 modelMatrix;
	float angle = 0.f;
	GLSLProgram* shader;
public:
	void Draw();
	void Update(float seconds);
	glm::vec3 position = glm::vec3(0.f, 0.0f, -1.f);
	Cube(std::string textureFileName, TextureManager* tManager, GLSLProgram* shaders);
	~Cube();
};
