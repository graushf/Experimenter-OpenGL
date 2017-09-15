#include "Geodesicsphere.h"

Geodesicsphere::Geodesicsphere(Camera* camera, GLuint screenWidth, GLuint screenHeight)
{
	Geodesicsphere::camera = camera;
	Geodesicsphere::screenWidth = screenWidth;
	Geodesicsphere::screenHeight = screenHeight;
}

Geodesicsphere::~Geodesicsphere()
{

}

void Geodesicsphere::Create(int recursionLevel)
{
	middlePointIndexCache.clear();
	vertices.clear();
	indices.clear();
	index = 0;

	auto t = (1.0 + sqrt(5.0)) / 2.0;

	AddVertex(glm::vec3(-1, t, 0));
	AddVertex(glm::vec3(1, t, 0));
	AddVertex(glm::vec3(-1, -t, 0));
	AddVertex(glm::vec3(1, -t, 0));

	AddVertex(glm::vec3(0, -1, t));
	AddVertex(glm::vec3(0, 1, t));
	AddVertex(glm::vec3(0, -1, -t));
	AddVertex(glm::vec3(0, 1, -t));

	AddVertex(glm::vec3(t, 0, -1));
	AddVertex(glm::vec3(t, 0, 1));
	AddVertex(glm::vec3(-t, 0, -1));
	AddVertex(glm::vec3(-t, 0, 1));

	auto faces = std::vector<TriangleIndices>();
	faces.push_back(TriangleIndices(0, 11, 5));
	faces.push_back(TriangleIndices(0, 5, 1));
	faces.push_back(TriangleIndices(0, 1, 7));
	faces.push_back(TriangleIndices(0, 7, 10));
	faces.push_back(TriangleIndices(0, 10, 11));

	faces.push_back(TriangleIndices(1, 5, 9));
	faces.push_back(TriangleIndices(5, 11, 4));
	faces.push_back(TriangleIndices(11, 10, 2));
	faces.push_back(TriangleIndices(10, 7, 6));
	faces.push_back(TriangleIndices(7, 1, 8));

	faces.push_back(TriangleIndices(3, 9, 4));
	faces.push_back(TriangleIndices(3, 4, 2));
	faces.push_back(TriangleIndices(3, 2, 6));
	faces.push_back(TriangleIndices(3, 6, 8));
	faces.push_back(TriangleIndices(3, 8, 9));

	faces.push_back(TriangleIndices(4, 9, 5));
	faces.push_back(TriangleIndices(2, 4, 11));
	faces.push_back(TriangleIndices(6, 2, 10));
	faces.push_back(TriangleIndices(8, 6, 7));
	faces.push_back(TriangleIndices(9, 8, 1));

	for (auto i = 0; i < recursionLevel; ++i)
	{
		auto faces2 = std::vector<TriangleIndices>();

		for (auto tri : faces)
		{
			int a = GetMiddlePoint(tri.v1, tri.v2);
			int b = GetMiddlePoint(tri.v2, tri.v3);
			int c = GetMiddlePoint(tri.v3, tri.v1);

			faces2.push_back(TriangleIndices(tri.v1, a, c));
			faces2.push_back(TriangleIndices(tri.v2, b, a));
			faces2.push_back(TriangleIndices(tri.v3, c, b));
			faces2.push_back(TriangleIndices(a, b, c));
		}

		faces.clear();
		for (unsigned int j = 0; j < faces2.size(); ++j)
		{
			faces.push_back(faces2[j]);
		}
	}

	for (auto tri : faces)
	{
		this->indices.push_back(tri.v1);
		this->indices.push_back(tri.v2);
		this->indices.push_back(tri.v3);
	}

	std::cout << "<<< Finished Creating Geodesicsphere >>>" << std::endl;
}

int Geodesicsphere::AddVertex(glm::vec3 position)
{
	double length = sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
	vertices.push_back(glm::vec3(position.x / length, position.y / length, position.z / length));

	return index++;
}

int Geodesicsphere::GetMiddlePoint(int p1, int p2)
{
	bool firstPointIsSmaller = p1 < p2;
	int64_t smallerIndex = firstPointIsSmaller ? p1 : p2;
	int64_t greaterIndex = firstPointIsSmaller ? p2 : p1;
	int64_t key = (smallerIndex << 32) + greaterIndex;

	auto foundValueIterator = middlePointIndexCache.find(key);
	if (foundValueIterator != middlePointIndexCache.end())
	{
		return foundValueIterator->second;
	}

	glm::vec3 point1 = vertices[p1];
	glm::vec3 point2 = vertices[p2];
	glm::vec3 middle = glm::vec3((point1.x + point2.x) / 2.0, (point1.y + point2.y) / 2.0, (point1.z + point2.z) / 2.0);
	
	int i = this->AddVertex(middle);

	this->middlePointIndexCache.insert(std::make_pair(key, i));
	return i;
}


void Geodesicsphere::createGeodesicsphereGeom()
{
	float t = (1.0 + sqrt(5.0)) / 2.0;

	std::vector<glm::vec3> vec;
	vec.push_back(glm::vec3(-1.0f, t, 0.0f));
	vec.push_back(glm::vec3(1.0f, t, 0.0f));
	vec.push_back(glm::vec3(-1.0f, -t, 0.0f));
	vec.push_back(glm::vec3(1.0f, -t, 0.0f));
	
	vec.push_back(glm::vec3(0.0f, -1.0f, t));
	vec.push_back(glm::vec3(0.0f, 1.0f, t));
	vec.push_back(glm::vec3(0.0f, -1.0f, -t));
	vec.push_back(glm::vec3(0.0f, 1.0f, -t));

	vec.push_back(glm::vec3(t, 0.0f, -1.0f));
	vec.push_back(glm::vec3(t, 0.0f, 1.0f));
	vec.push_back(glm::vec3(-t, 0.0f, -1.0f));
	vec.push_back(glm::vec3(-t, 0.0f, 1.0f));

	GLfloat *flat_array = &vec[0].x;
	for (int i = 0; i < 36; i++)
		std::cout << flat_array[i] << std::endl;


	GLfloat verticesGeodesicsphere[] = {
		-1.0f, t, 0.0f,
		1.0f, t, 0.0f,
		-1.0f, -t, 0.0f,
		1.0f, -t, 0.0f,

		0.0f, -1.0f, t,
		0.0f, 1.0f, t,
		0.0f, -1.0f, -t,
		0.0f, 1.0f, -t,

		t, 0.0f, -1.0f,
		t, 0.0f, 1.0f,
		-t, 0.0f, -1.0f,
		-t, 0.0f, 1.0f
	};

	GLuint indicesGeodesicsphere[] = {
		// 5 faces around point 0
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		// 5 adjacent faces
		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		// 5 faces around point 3
		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		// 5 adjacent faces
		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};

	glGenVertexArrays(1, &geodesicsphereVAO);
	glGenBuffers(1, &geodesicsphereVBO);
	glGenBuffers(1, &geodesicsphereEBO);

	glBindVertexArray(geodesicsphereVAO);

	//std::cout << "SIzeof : " <<  << std::endl;

	glBindBuffer(GL_ARRAY_BUFFER, geodesicsphereVBO);
	glBufferData(GL_ARRAY_BUFFER, vec.size() * sizeof(glm::vec3), vec.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geodesicsphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGeodesicsphere), indicesGeodesicsphere, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geodesicsphere::renderGeodesicsphereGeom(glm::vec3 position, float distance, glm::vec3 color, int drawingMode)
{
	if (drawingMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	geodesicsphereShader.Use();
	// Create camera transformations
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(geodesicsphereShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(geodesicsphereShader.Program, "view");
	GLint projLoc = glGetUniformLocation(geodesicsphereShader.Program, "projection");
	GLint colorLoc = glGetUniformLocation(geodesicsphereShader.Program, "ucolor");

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(colorLoc, color.x, color.y, color.z);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(distance * position.x, distance * position.y, distance * position.z));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(geodesicsphereVAO);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Geodesicsphere::createGeodesicsphereShader()
{
	geodesicsphereShader = Shader("./shaders/shaderGeodesicsphere.vertex", "./shaders/shaderGeodesicsphere.frag");
}

void Geodesicsphere::createAdvancedGeodesicsphereGeom()
{
	glGenVertexArrays(1, &advancedGeodesicsphereVAO);
	glGenBuffers(1, &advancedGeodesicsphereVBO);
	glGenBuffers(1, &advancedGeodesicsphereEBO);

	glBindVertexArray(advancedGeodesicsphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, advancedGeodesicsphereVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, advancedGeodesicsphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geodesicsphere::renderAdvancedGeodesicsphere(glm::vec3 position, float distance, glm::vec3 scale, glm::vec3 color, int drawingMode)
{
	if (drawingMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} 
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	geodesicsphereShader.Use();
	// Create camera transformations
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(geodesicsphereShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(geodesicsphereShader.Program, "view");
	GLint projLoc = glGetUniformLocation(geodesicsphereShader.Program, "projection");
	GLint colorLoc = glGetUniformLocation(geodesicsphereShader.Program, "ucolor");

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(colorLoc, color.x, color.y, color.z);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(distance * position.x, distance * position.y, distance * position.z));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(advancedGeodesicsphereVAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
