#pragma once

#include <glad/glad.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

#include <iostream>
#include <string>

namespace Graphics
{
    class CTexture
    {
    public:
        CTexture(const std::string& path);
        CTexture(const CTexture&) = delete;
        ~CTexture();

        void Use(GLenum index = GL_TEXTURE0);
        
    private:
        GLuint m_id;
    };
};