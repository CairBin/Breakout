#ifndef _SHADER_H_
#define _SHADER_H_

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace GameUtil{
    class Shader{
    
    public:
        GLuint Id;
        Shader();
        Shader &Use();
        void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);
        void setBool(const std::string &name, GLboolean value,GLboolean useShader=false) ;
        void setInt(const std::string &name, GLint value,GLboolean useShader = false) ;
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, GLfloat value, GLboolean useShader = false) ;
        // ------------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value, GLboolean useShader = false) ;
        void setVec2(const std::string &name, GLfloat x, GLfloat y, GLboolean useShader = false) ;
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value, GLboolean useShader = false) ;
        void setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false) ;
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value, GLboolean useShader = false) ;
        void setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat, GLboolean useShader = false) ;
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat, GLboolean useShader = false) ;
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat, GLboolean useShader = false) ;

    private:
        void checkCompileErrors(GLuint shader, std::string type);
    };

}

#endif