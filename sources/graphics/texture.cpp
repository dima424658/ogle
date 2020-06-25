#include <graphics/texture.hpp>

using namespace Graphics;

CTexture::CTexture()
{
    LoadDefault();
}

CTexture::CTexture(const std::string& path)
{
    LoadImage(path);
}

CTexture::CTexture(const CTexture& o)
{
    GLuint fbo;
    m_width = o.m_width;
    m_height = o.m_height;
    m_format = o.m_format;

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

CTexture::CTexture(CTexture&& o) noexcept
{
    m_id = o.m_id;
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
        m_id = 0;
        m_width = 0;
        m_height = 0;
        m_format = 0;
    }
}

void CTexture::LoadImage(const std::string& path)
{
    this->~CTexture();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        System::Log() << "Failed to load image \"" << path << "\": " << IMG_GetError() << ". Setting default\n";
        LoadDefault();
        return;
    }

    if (surface->format->BitsPerPixel == 24)
        m_format = GL_RGB;
    else if (surface->format->BitsPerPixel == 32)
        m_format = GL_RGBA;
    else
    {
        System::Log() << "Failed to determinate image format \"" << path << "\": " << IMG_GetError() << ". Setting default\n";
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
}

void CTexture::LoadDefault()
{
    SDL_RWops* rwops = SDL_RWFromMem(default_image, sizeof(default_image) / sizeof(uint8_t));
    SDL_Surface* surface = IMG_Load_RW(rwops, 0);
    SDL_FreeRW(rwops);
    if (!surface)
        throw std::runtime_error("Failed to load default image\n");

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

    m_width = surface->w;
    m_height = surface->h;
    m_format = GL_RGB;

    SDL_FreeSurface(surface);
}

void CTexture::Use(GLenum index)
{
    glActiveTexture(index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

GLuint Graphics::CTexture::GetID()
{
    return m_id;
}
