#include <graphics/texture.hpp>

using namespace Graphics;

CTexture::CTexture(const std::string& path)
{
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if(!surface)
        throw std::runtime_error(std::string("Failed to load texture: ") + path + "\n" + IMG_GetError());
        
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLint format;
    if(surface->format->BitsPerPixel == 24)
        format = GL_RGB;
    else if(surface->format->BitsPerPixel == 32)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(surface);
}

CTexture::~CTexture()
{
    glDeleteTextures(1, &m_id);
}

void CTexture::Use(GLenum index)
{
    glActiveTexture(index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}