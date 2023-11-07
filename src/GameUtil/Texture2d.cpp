#include "GameUtil/Texture.h"

namespace GameUtil{
    Texture2d::Texture2d()
        : Width(0), Height(0),
          Internal_Format(GL_RGB),
          Image_Format(GL_RGB),
          Wrap_T(GL_REPEAT),
          Filter_Min(GL_LINEAR),
          Filter_Max(GL_LINEAR)
    {
        glGenTextures(1, &this->ID);
    }
    void Texture2d::Generate(GLuint width, GLuint height, unsigned char *data)
    {
        this->Width = width;
        this->Height = height;
        glBindTexture(GL_TEXTURE_2D, this->ID);
        glTexImage2D(GL_TEXTURE_2D,
                     0, this->Internal_Format, width, height, 0,
                     this->Image_Format,
                     GL_UNSIGNED_BYTE, data);
    }

    void Texture2d::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, this->ID);
    }
}