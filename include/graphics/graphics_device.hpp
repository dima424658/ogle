#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "system/log.hpp"

#include <functional>
#include <stdexcept>
#include <iostream>

#include "graphics/shader.hpp"

namespace Graphics
{
    class CDevice
    {
    private:
        CDevice();
        CDevice(const CDevice&) = delete;
        CDevice& operator=(CDevice&) = delete;
    public:
        ~CDevice();

        void Begin();
        void End();

        int WindowShouldClose();

        SDL_Window* GetWindow() const;
        const SDL_GLContext& GetGLContext() const;
        static CDevice& Instance();

    private:
    
    private:
        SDL_Window* m_window;
        SDL_GLContext m_context;

        GLuint m_framebuffer, m_renderbuffer;
        GLuint m_texturePosition, m_texutreNormal, m_textureAlbedo;
    };

    static void callback(int code, const char* error);
};