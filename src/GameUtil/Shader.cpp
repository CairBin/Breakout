#include "GameUtil/Shader.h"

namespace GameUtil{
    Shader::Shader(){}
    void Shader::Compile(const char *vertexPath, const char *fragmentPath, const char *geometryPath )
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {

            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
           
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        
        Id = glCreateProgram();
        glAttachShader(Id, vertex);
        glAttachShader(Id, fragment);
        glLinkProgram(Id);
        checkCompileErrors(Id, "PROGRAM");
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
        Shader &Shader::Use()
    {
        glUseProgram(this->Id);
    }
    void Shader::setBool(const std::string &name, GLboolean value) const
    {
        glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, GLint value) const
    {
        glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, GLfloat value) const
    {
        glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec2(const std::string &name, GLfloat x, GLfloat y) const
    {
        glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const
    {
        glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
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