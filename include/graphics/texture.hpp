#pragma once

#include <glad/glad.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

#include <iostream>
#include <string>

#include <system/log.hpp>

namespace Graphics
{
    class CTexture
    {
    private:
        uint8_t default_image[58] = {
            0x42, 0x4d, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00,
            0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
            0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
            0x00, 0x00, 0xc4, 0x0e, 0x00, 0x00, 0xc4, 0x0e, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00
            };

    public:
        CTexture();
        CTexture(std::string_view path);
        CTexture(const CTexture &o);
        CTexture(CTexture &&o) noexcept;
        ~CTexture();

        void LoadImage(std::string_view path);
        void Use(GLenum index = GL_TEXTURE0) const;
        GLuint GetID() const;
        const std::string &GetPath() const;

    private:
        void LoadDefault();

        std::string m_imagePath;
        GLuint m_id = 0;
        GLsizei m_width = 0, m_height = 0;
        GLint m_format = 0;
    };
}; // namespace Graphics