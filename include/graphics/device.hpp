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
        CDevice(int width, int height);
        CDevice(const CDevice&) = delete;
        CDevice& operator=(CDevice&) = delete;
    public:
        ~CDevice();

        void Begin();
        void End();

        SDL_Window* GetWindow() const;
        const SDL_GLContext& GetGLContext() const;
        static CDevice& Instance();

    private:
    
    private:
        SDL_Window* m_window;
        SDL_GLContext m_context;
    };

    inline CDevice& GetGraphics()
    {
        return CDevice::Instance();
    }
};