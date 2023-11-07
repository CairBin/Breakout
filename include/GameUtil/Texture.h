#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "glad/glad.h"

namespace GameUtil{
    class Texture2d{
    public:
        GLuint ID;
        GLuint Width, Height;
        GLuint Internal_Format;
        GLuint Image_Format;
        GLuint Wrap_S;
        GLuint Wrap_T;
        GLuint Filter_Min;
        GLuint Filter_Max;

        Texture2d();
        void Generate(GLuint width, GLuint height, unsigned char *data);
        void Bind() const;
    };
}

#endif