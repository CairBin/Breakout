#ifndef _POST_PROCESSOR_H_
#define _POST_PROCESSOR_H_

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "GameUtil/Texture.h"
#include "GameUtil/SpriteRenderer.h"
#include "GameUtil/Shader.h"

namespace GameUtil{
    class PostProcessor{
    public:
        Shader PostProcessingShader;
        Texture2d Texture;
        GLuint Width, Height;
        GLboolean Confuse, Chaos, Shake;
        PostProcessor(Shader shader, GLuint width, GLuint height);
        void BeginRender();
        void EndRender();
        void Render(GLfloat time);
    private:
        GLuint MSFBO, FBO;
        GLuint RBO;
        GLuint VAO;
        void initRenderData();
    };
}


#endif