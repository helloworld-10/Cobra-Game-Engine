#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Component.h"
class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat2(const std::string& name, const glm::mat2& mat);
    void setMat3(const std::string& name, const glm::mat3& mat);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setMat4Array(const std::string& name, const std::vector<glm::mat4> mat, int size);
    void setVec3Array(const std::string& name, const std::vector<glm::vec3> vecs, const std::string& structComp, int size);
    void setFloatArray(const std::string& name, const std::vector<float> vecs, const std::string& structComp, int size);
    void setPointLightArray(const std::string& name, const std::vector<PointLightComponent> lights, const std::vector<TransformComponent> transforms);
    void setDirLight(const std::string& name, const DirectionalLightComponent dirLight);

};