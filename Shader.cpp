#include "Shader.h"

#include <vector>
#include <cassert>

std::string loadTextFile(const std::string& filename) {
	std::stringstream buffer;
	buffer << std::ifstream(filename.c_str()).rdbuf();
	return buffer.str();
}


GLuint compileShaderStage(GLenum stage, const std::string& source)
{
	GLuint shader = glCreateShader(stage);
	const char* srcArray[] = { source.c_str() };

	glShaderSource(shader, 1, srcArray, NULL);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);
		std::string s = std::string(errorLog.begin(), errorLog.end());
		std::cout << s << std::endl;
		assert(false);

		glDeleteShader(shader);
		shader = GL_NONE;
	}
	return shader;
}

Shader::Shader() {
	Shader::Program = -1;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string sVertexShaderSource;
	std::string sFragmentShaderSource;

	sVertexShaderSource = loadTextFile(vertexPath);
	sFragmentShaderSource = loadTextFile(fragmentPath);

	GLuint vertexShader = compileShaderStage(GL_VERTEX_SHADER, sVertexShaderSource);
	GLuint fragmentShader = compileShaderStage(GL_FRAGMENT_SHADER, sFragmentShaderSource);

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	GLint success;
	GLchar infoLog[512];
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as theyy're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() {
	glUseProgram(this->Program);
}