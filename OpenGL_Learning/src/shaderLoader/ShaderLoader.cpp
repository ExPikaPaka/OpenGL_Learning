#include "ShaderLoader.h"
#include <fstream>
#include <iostream>

#include <glad/glad.h>

bool ShaderLoader::load(std::string& filePath) {
	// Logger message initialization 
	err.init("[ERROR]", Color(242, 242, 242), Color(197, 15, 31));
	info.init("[INFO]", Color(242, 242, 242), Color(58, 150, 221));
	logger.logToConsole(true);


	// Opening file
	std::ifstream shaderFile(filePath);
	
	// Check
	if (!shaderFile.is_open()) {
		logger.addCombined(err, " Failed to open shader file: ", filePath, "\n");
		return false;
	}
	
	// Reading
	std::string line;
	while (std::getline(shaderFile, line)) {
		shaderCode += line + '\n';
	}
	

	// Defining shader type via filePath extension
	std::string fileExtension = filePath.substr(filePath.find_last_of('.') + 1);
	GLenum shaderType = 0;

	if (fileExtension == "vert") {
		shaderType = GL_VERTEX_SHADER;

	} else if (fileExtension == "frag") {
		shaderType = GL_FRAGMENT_SHADER;

	} else {
		logger.addCombined(err, " Unsupported shader file extension: ", fileExtension, "\n");
		return false;
	}


	// Creating shader
	const char* c_str = shaderCode.c_str();
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &c_str, NULL);
	glCompileShader(shader);

	// Check for compiling errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		logger.addCombined(err, " Failed to compile shader <", filePath, ">.", "\n");
		logger.addCombined(" Log: ", infoLog, "\n");
		return false;
	}

	return true;
}

unsigned int ShaderLoader::getShader() {
	return shader;
}

void ShaderLoader::deleteShader() {
	glDeleteShader(shader);
	shaderCode.clear();
}

void ShaderLoader::clear() {
	shaderCode.clear();
	shader = 0;
}
