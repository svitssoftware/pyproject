#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture()
{

}

void Texture::LoadFromFile(string path)
{
    int w, h, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &nrComponents, STBI_rgb_alpha);
    if (data)
    {
        GLenum format{};
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }


        glGenTextures(1, &this->texName);
        glBindTexture(GL_TEXTURE_2D, this->texName);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, NULL);

        this->x = ToGLSize(w, WIDTH);
        this->y = ToGLSize(h, HEIGHT);
    }

    stbi_image_free(data);
}