﻿#include <graphics/texture.hpp>

using namespace Graphics;

CTexture::CTexture()
{
    LoadDefault();
}

CTexture::CTexture(std::string_view path)
{
    LoadImage(path);
}

CTexture::CTexture(const CTexture &o)
{
    GLuint fbo;
    m_width = o.m_width;
    m_height = o.m_height;
    m_format = o.m_format;
    m_imagePath = o.m_imagePath;

    glGenTextures(1, &m_id);
    glGenFramebuffers(1, &fbo);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, o.m_id, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_id, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);
    glDeleteFramebuffers(1, &fbo);
}

CTexture::CTexture(CTexture &&o) noexcept
{
    m_id = o.m_id;
    m_imagePath = std::move(o.m_imagePath);
    o.m_id = 0;
    m_width = o.m_width;
    m_height = o.m_height;
    m_format = o.m_format;
}

CTexture::~CTexture()
{
    if (m_id != 0)
    {
        glDeleteTextures(1, &m_id);
        m_imagePath = "";
        m_id = 0;
        m_width = 0;
        m_height = 0;
        m_format = 0;
    }
}

void CTexture::LoadImage(std::string_view path)
{
    this->~CTexture();

    SDL_Surface *surface = IMG_Load(path.data());
    if (!surface)
    {
        System::Warning() << "Failed to load image \"" << path << "\": " << IMG_GetError() << ". Setting default";
        LoadDefault();
        return;
    }

    if (surface->format->BitsPerPixel == 24)
        m_format = GL_RGB;
    else if (surface->format->BitsPerPixel == 32)
        m_format = GL_RGBA;
    else
    {
        System::Warning() << "Failed to determinate image format \"" << path << "\": " << IMG_GetError() << ". Setting default";
        SDL_FreeSurface(surface);
        LoadDefault();
        return;
    }

    m_width = surface->w;
    m_height = surface->h;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, m_format, surface->w, surface->h, 0, m_format, GL_UNSIGNED_BYTE, surface->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(surface);
    m_imagePath = path;
}

const std::string &Graphics::CTexture::GetPath() const
{
    return m_imagePath;
}

void CTexture::LoadDefault()
{
    SDL_RWops *rwops = SDL_RWFromMem(default_image, sizeof(default_image) / sizeof(uint8_t));
    SDL_Surface *surface = IMG_Load_RW(rwops, 0);
    SDL_FreeRW(rwops);
    if (!surface)
    {
        System::Error() << "Failed to load default image.";
        System::Exit();
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

    m_width = surface->w;
    m_height = surface->h;
    m_format = GL_RGB;

    SDL_FreeSurface(surface);
    m_imagePath = "";
}

void CTexture::Use(GLenum index) const
{
    glActiveTexture(index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

GLuint Graphics::CTexture::GetID() const
{
    return m_id;
}
