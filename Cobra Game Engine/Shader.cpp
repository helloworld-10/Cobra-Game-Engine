#include "Shader.h"



    unsigned int ID;
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, frag;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fShaderCode, NULL);
        glCompileShader(frag);
        // print compile errors if any
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, frag);
        glLinkProgram(ID);
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(frag);
    }

    void Shader::use()
    {
        glUseProgram(ID);
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void Shader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec2(const std::string& name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string& name, const glm::mat2& mat)
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4Array(const std::string& name, const std::vector<glm::mat4> matrices, int size) {
        for (unsigned int i = 0; i < size; i++)
        {
            Shader::setMat4((name+"[" + std::to_string(i) + "]"), matrices[i]);
        }

    }

    void Shader::setVec3Array(const std::string& name, const std::vector<glm::vec3> vecs, const std::string& structComp,int size)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            Shader::setVec3((name + "[" + std::to_string(i) + "]"+structComp), vecs[i]);
        }
    }

    void Shader::setFloatArray(const std::string& name, const std::vector<float> vecs, const std::string& structComp, int size)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            Shader::setFloat((name + "[" + std::to_string(i) + "]" + structComp), vecs[i]);
        }
    }

    void Shader::setPointLightArray(const std::string& name, const std::vector<PointLightComponent> lights, const std::vector<TransformComponent> transforms) {
        for (unsigned int i = 0; i < lights.size(); i++)
        {
            
            Shader::setVec3((name + "[" + std::to_string(i) + "].color"), lights[i].color);
            Shader::setVec3((name + "[" + std::to_string(i) + "].function"), lights[i].attenuation);
        }
        for (int i = 0; i < transforms.size(); i++)
        {
            Shader::setVec3((name + "[" + std::to_string(i) + "].position"), transforms[i].position);
        }

    }

    void Shader::setDirLight(const std::string& name, const DirectionalLightComponent dirLight)
    {
        Shader::setVec3((name+".color"), dirLight.color);
        Shader::setVec3((name+ ".direction"), dirLight.direction);
    }

