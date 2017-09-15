#include "Billboard.h"

Billboard::Billboard(Camera* camera, float screenWidth, float screenHeight, char* texturebillboardpath, char* vertexShaderSrc, char* fragmentShaderSrc) 
{
	Billboard::camera = camera;
	Billboard::screenWidth = screenWidth;
	Billboard::screenHeight = screenHeight;
	createBillboardGeom(texturebillboardpath);
	createBillboardShader(vertexShaderSrc, fragmentShaderSrc);
}

void Billboard::createBillboardGeom(char* texturebillboardpath)
{
	glewInit();
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f   // Top Left
	};

	GLuint indices[] = { // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3	 // Second Triangle
	};

	GLfloat textureCoordinatesBillboard[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	glGenVertexArrays(1, &billboardVAO);
	glGenBuffers(1, &billboardVBO);
	glGenBuffers(1, &texCoordsBillboardVBO);
	glGenBuffers(1, &billboardEBO);
	// Bind the Vertex Array Object first, then binf and set vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(billboardVAO);

	glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBillboardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesBillboard), textureCoordinatesBillboard, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, billboardEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Texture 
	glGenTextures(1, &textureBillboard);
	glBindTexture(GL_TEXTURE_2D, textureBillboard);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;

	unsigned char* image_plane = SOIL_load_image(texturebillboardpath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Billboard::renderBillboardGeom(Camera* camera, glm::vec3 position, glm::vec3 scale, float distanceBillboard, int drawingMode)
{
	Billboard::camera = camera;
	if (drawingMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	billboardShader.Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBillboard);
	GLint aux = glGetUniformLocation(billboardShader.Program, "textureBillboard");
	glUniform1i(glGetUniformLocation(billboardShader.Program, "textureBillboard"), 0);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(billboardShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(billboardShader.Program, "view");
	GLint projLoc = glGetUniformLocation(billboardShader.Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	glm::vec3 posBillboard;

	posBillboard.x = distanceBillboard * position.x;
	posBillboard.y = distanceBillboard * position.y;
	posBillboard.z = distanceBillboard * position.z;
	
	glm::mat4 model;


	// Billboarding code
	glm::vec3 direction = camera->Position - posBillboard;
	direction = glm::normalize(direction);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0); // CHECK if camera true up vector needs to be used instead
	//glm::vec3 up = camera->Up;
	glm::vec3 right = glm::cross(direction, up);
	up = glm::cross(direction, right);
	up = glm::normalize(up);

	
	//model = glm::translate(model, glm::vec3(posBillboard.x, posBillboard.y, posBillboard.z));
	glm::mat4 _scaleM;
	_scaleM = glm::scale(_scaleM, scale);
	model = glm::translate(model, posBillboard);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint pointPosLoc = glGetUniformLocation(billboardShader.Program, "point_pos");
	GLint cameraPosLoc = glGetUniformLocation(billboardShader.Program, "camera_pos");
	GLint cameraUpLoc = glGetUniformLocation(billboardShader.Program, "camera_up");

	GLint directionLoc = glGetUniformLocation(billboardShader.Program, "_direction");;
	GLint upLoc = glGetUniformLocation(billboardShader.Program, "_up");
	GLint rightLoc = glGetUniformLocation(billboardShader.Program, "_right");

	GLint scaleLoc = glGetUniformLocation(billboardShader.Program, "_scale");

	glUniform3f(pointPosLoc, posBillboard.x, posBillboard.y, posBillboard.z);
	glUniform3f(cameraPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(cameraUpLoc, camera->Up.x, camera->Up.y, camera->Up.z);

	glUniform3f(directionLoc, direction.x, direction.y, direction.z);
	glUniform3f(upLoc, up.x, up.y, up.z);
	glUniform3f(rightLoc, right.x, right.y, right.z);
	glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(_scaleM));


	glBindVertexArray(billboardVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Billboard::createBillboardShader(char* vertexShaderSrc, char* fragmentShaderSrc)
{
	billboardShader = Shader(vertexShaderSrc, fragmentShaderSrc);
}