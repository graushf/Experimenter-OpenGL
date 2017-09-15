#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>

#include "Shader.h"
#include "Camera.h"

class Billboard {

public:

	Camera* camera;
	GLuint screenWidth;
	GLuint screenHeight;

	GLuint billboardVAO;
	GLuint billboardVBO;
	GLuint texCoordsBillboardVBO;
	GLuint billboardEBO;
	Shader billboardShader;
	GLuint textureBillboard;

	Billboard(Camera* camera, float screenWidth, float screenHeight, char* texturebillboardpath, char* vertexShaderSrc, char* fragmentShaderSrc);

	void createBillboardGeom(char* texturebillboardpath);
	void renderBillboardGeom(Camera* camera, glm::vec3 position, glm::vec3 scale, float distanceBillboard, int drawingMode);
	void createBillboardShader(char* vertexShaderSrc, char* fragmentShaderSrc);

};