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
    
        void Prepare() const;
        void Draw(const glm::vec3& cameraPosition) const;
        GLuint DrawToTexture(const glm::vec3& cameraPosition) const;
        GLuint GetFinalbuffer() const;
        GLuint GetPickingTexture() const;

        // gets object id located at position
        GLuint GetObjectID(int x, int y) const;
        
        void Resize(int width, int height);

        const CShader& GetGShader() const;
        const CShader& GetDiffuseShader() const;
    private:
        void InitScreen();
        void RenderScreen() const;
        void DestroyScreen();

        void ConfigureFramebuffer(int width, int height);
        void ResizeFramebuffer(int width, int height);
        void DestroyFramebuffer();

        void ConfigureRenderbuffer(int width, int height);
        void DestroyRenderbuffer();

    private:
        int m_width, m_height;
        GLuint m_framebuffer, m_renderbuffer;
        GLuint m_finalbuffer, m_finalTexture;

        GLuint m_texturePosition, m_texutreNormal, m_textureAlbedo, m_texturePicking;

        CShader m_gShader, m_diffuseShader;

        GLuint m_screenVAO, m_screenVBO;
    };
}