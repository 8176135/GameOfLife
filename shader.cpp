//
// Created by username on 11/09/2021.
//
#include <glad/glad.h>
#include "shader.h"

static void checkShaderCompileSuccess(GLuint shaderId) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX_OR_FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

static void checkProgramCompileSuccess(GLuint programId) {
	int success;
	char infoLog[512];
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode   = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure &e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	// vertex Shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	{
		auto vShaderPtr = vertexCode.c_str();
		auto vShaderLen = static_cast<int>(vertexCode.length());
		glShaderSource(vertexShaderId, 1, &vShaderPtr, &vShaderLen);
		glCompileShader(vertexShaderId);
		checkShaderCompileSuccess(vertexShaderId);
	}

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	{
		auto fShaderPtr = fragmentCode.c_str();
		auto fShaderLen = static_cast<int>(fragmentCode.length());
		glShaderSource(fragmentShaderId, 1, &fShaderPtr, &fShaderLen);
		glCompileShader(fragmentShaderId);
		checkShaderCompileSuccess(fragmentShaderId);
	}

	ID = glCreateProgram();

	glAttachShader(ID, vertexShaderId);
	glAttachShader(ID, fragmentShaderId);
	glLinkProgram(ID);
	checkProgramCompileSuccess(ID);
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

void Shader::use() {
	glUseProgram(ID);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
