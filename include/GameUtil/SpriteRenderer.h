#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "GameUtil/Shader.h"
#include "GameUtil/Texture.h"

namespace GameUtil{
    class SpriteRenderer{
    private:
        Shader shader;
        GLuint quadVAO;

        void initRenderData();

    public:
        SpriteRenderer(const Shader &shader);
        ~SpriteRenderer();
        void DrawSprite(const Texture2d &texture, glm::vec2 position,
                        glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
                        glm::vec3 color = glm::vec3(1.0f));
    };
}

#endif