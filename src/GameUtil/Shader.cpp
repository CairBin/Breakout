#include "GameUtil/Shader.h"

namespace GameUtil
{
    Shader::Shader() {}
    void Shader::Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource)
    {
        GLuint sVertex, sFragment, gShader;

        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);
        checkCompileErrors(sVertex, "VERTEX");

        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);
        checkCompileErrors(sFragment, "FRAGMENT");

        if (geometrySource != nullptr)
        {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &geometrySource, NULL);
            glCompileShader(gShader);
            checkCompileErrors(gShader, "GEOMETRY");
        }

        this->Id = glCreateProgram();
        glAttachShader(this->Id, sVertex);
        glAttachShader(this->Id, sFragment);
        if (geometrySource != nullptr)
            glAttachShader(this->Id, gShader);
        glLinkProgram(this->Id);
        checkCompileErrors(this->Id, "PROGRAM");

        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
        if (geometrySource != nullptr)
            glDeleteShader(gShader);
    }
    Shader &Shader::Use()
    {
        glUseProgram(this->Id);
    }
    void Shader::setBool(const std::string &name, GLboolean value, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, GLint value, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, GLfloat value, GLboolean useShader) 
    {
        if (useShader)
            this->Use();
        glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec2(const std::string &name, GLfloat x, GLfloat y, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat, GLboolean useShader)
    {
        if (useShader)
            this->Use();
        glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}