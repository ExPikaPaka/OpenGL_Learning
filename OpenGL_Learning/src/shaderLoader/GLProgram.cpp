#include "GLProgram.h"

GLProgram::GLProgram(std::string& vetexShaderPath, std::string& fragmentShaderPath) {
	init(vetexShaderPath,fragmentShaderPath);
}

bool GLProgram::init(std::string& vetexShaderPath, std::string& fragmentShaderPath) {
	// Logger message initialization 
	err.init("[ERROR]", Color(242, 242, 242), Color(197, 15, 31));
	info.init("[INFO]", Color(242, 242, 242), Color(58, 150, 221));
	logger.logToConsole(true);



	// Shaders ID
	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;

	// Vertex shader loading
	if (loader.load(vetexShaderPath)) {
		vertexShader = loader.getShader();
	} else {
		return false;
	}
	loader.clear();

	// Fragment shader loading
	if (loader.load(fragmentShaderPath)) {
		fragmentShader = loader.getShader();
	}
	else {
		return false;
	}
	loader.clear();




	// Creating program
	ID = glCreateProgram();

	// Attaching shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);


	// Program linking status
	int success = 0;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (success) {
		logger.addCombined(info, " Shader program linked successfully.", "\n");
		
	} else {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		logger.addCombined(err, " Failed to link program with shaders:", "\n");
		logger.addCombined("     ", vetexShaderPath, "\n");
		logger.addCombined("     ", fragmentShaderPath, "\n");
		logger.addCombined(" Log: ", infoLog, "\n");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}

void GLProgram::use() {
	glUseProgram(ID);
}

void GLProgram::setBool(const char* name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void GLProgram::setInt(const char* name, int value) {
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void GLProgram::setFloat(const char* name, float value) {
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void GLProgram::setVec3(const char* name, float value1, float value2, float value3) {
	glUniform3f(glGetUniformLocation(ID, name), value1, value2, value3);
}

void GLProgram::setVec3(const char* name, glm::vec3 value) {
	glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void GLProgram::setMat4(const char* name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &matrix[0][0]);
}
