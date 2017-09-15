#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>

#include "Shader.h"
#include "Camera.h"

struct TriangleIndices
{
	int v1, v2, v3;
	
	TriangleIndices(int v1, int v2, int v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
};

class Geodesicsphere
{
private:
	Camera* camera;
	GLuint screenWidth;
	GLuint screenHeight;

	int index;
	std::map<int64_t, int> middlePointIndexCache;

	GLuint geodesicsphereVAO;
	GLuint geodesicsphereVBO;
	GLuint geodesicsphereEBO;
	Shader geodesicsphereShader;

	GLuint advancedGeodesicsphereVAO;
	GLuint advancedGeodesicsphereVBO;
	GLuint advancedGeodesicsphereEBO;

	int AddVertex(glm::vec3 position);
	int GetMiddlePoint(int p1, int p2);


public:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	Geodesicsphere(Camera* camera, GLuint screenWidth, GLuint screenHeight);
	~Geodesicsphere();
	void Create(int recursionLevel);
	void createGeodesicsphereGeom();
	void renderGeodesicsphereGeom(glm::vec3 position, float distance, glm::vec3 color, int drawingMode);
	void createGeodesicsphereShader();


	void createAdvancedGeodesicsphereGeom();
	void renderAdvancedGeodesicsphere(glm::vec3 position, float distance, glm::vec3 scale, glm::vec3 color, int drawingMode);
};