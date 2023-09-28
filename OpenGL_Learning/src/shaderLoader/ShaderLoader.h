#pragma once
#include <string>
#include "..\logSystem\Logger.h"

class ShaderLoader {
	Logger logger;
	Message err;
	Message info;

	std::string shaderCode;
	unsigned int shader;

public:
	bool load(std::string& filePath);
	unsigned int getShader();
	void deleteShader();
	void clear();
};

