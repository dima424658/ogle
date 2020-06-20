#pragma once

#include <glad/glad.h>

#include <graphics/shader.hpp>
#include <system/log.hpp>

namespace Graphics
{
    class CDeferred
    {
    public:
        CDeferred(int width, int height);
        ~CDeferred();

        void ResizeFramebuffer(int width, int height);
    
        void Begin();
        void End(const glm::vec3& cameraPosition);
        
        CShader& GetShader();
    private:
        void InitScreen();
        void RenderScreen();
        void DestroyScreen();

        void ConfigureFramebuffer(int width, int height);
        void DestroyFramebuffer();

        void ConfigureRenderbuffer(int width, int height);
        void DestroyRenderbuffer();

    private:
        int m_width, m_height;
        GLuint m_framebuffer, m_renderbuffer;
        GLuint m_texturePosition, m_texutreNormal, m_textureAlbedo;

        CShader m_gShader, m_diffuseShader;

        GLuint m_screenVAO, m_screenVBO;
    };
}