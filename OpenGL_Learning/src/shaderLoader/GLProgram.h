#pragma once
#include <glad/glad.h>
#include <string>
#include "ShaderLoader.h"
#include "../logSystem/Logger.h"
#include <glm/glm.hpp>

class GLProgram {
public:
    // the program ID
    unsigned int ID;


    // Initializators
    GLProgram() {};
    GLProgram(std::string& vetexShaderPath, std::string& fragmentShaderPath);

    bool init(std::string& vetexShaderPath, std::string& fragmentShaderPath);

    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setVec3(const char* name, float value1, float value2, float value3);
    void setVec3(const char* name, glm::vec3 value);
    void setMat4(const char* name, glm::mat4 matrix);

private:
    ShaderLoader loader;
    Logger logger;
    Message err;
    Message info;
};

