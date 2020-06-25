#include "graphics/device.hpp"

using namespace Graphics;

CDevice::CDevice(int width, int height)
{    
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(std::string("Failed to initialize SDL2: ") + SDL_GetError());

    m_window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!m_window)
        throw std::runtime_error(std::string("Failed to create SDL2 window: ") + SDL_GetError());

    m_context = SDL_GL_CreateContext(m_window);
    if(!m_context)
        throw std::runtime_error(std::string("Failed to create SDL2 OpenGL context: ") + SDL_GetError());
 
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD.");

    #if __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    #endif
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

}

CDevice::~CDevice()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void CDevice::Begin()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CDevice::End()
{
    SDL_GL_SwapWindow(m_window);
}

SDL_Window* CDevice::GetWindow() const
{
    return m_window;
}

const SDL_GLContext& CDevice::GetGLContext() const
{
    return m_context;
}

CDevice& CDevice::Instance()
{
    static CDevice instance(1600, 900);
    return instance;
}