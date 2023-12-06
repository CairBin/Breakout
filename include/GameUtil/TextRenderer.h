#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include <map>
#include "glad/glad.h"
#include "GameUtil/Texture.h"
#include "GameUtil/Shader.h"
#include "glm/glm.hpp"

namespace GameUtil{
    typedef struct Character{
        GLuint TextureId;
        glm::ivec2 Size;
        glm::ivec2 Bearing; // 从基线到图示符左侧/顶部的偏移量
        GLuint Advance; //下一个字的水平偏移
    } Character;

    class TextRenderer{
    public:
        std::map<GLchar,Character> Characters;
        Shader TextShader;
        TextRenderer(const Shader& shader);
        void Load(std::string font, GLuint fontSize);

        void RenderText(std::string text,
                        GLfloat x, GLfloat y,
                        GLfloat scale,
                        glm::vec3 color = glm::vec3(1.0f));

    private:
        GLuint VAO, VBO;
    };
}

#endif