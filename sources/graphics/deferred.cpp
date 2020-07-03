#include <graphics/deferred.hpp>

using namespace Graphics;

CDeferred::CDeferred(int width, int height)
    : m_width(width), m_height(height), m_gShader("shaders/gbuffer.vs", "shaders/gbuffer.fs"), m_diffuseShader("shaders/diffuse.vs", "shaders/diffuse.fs")
{
    m_textureAlbedo = 0;
    m_texturePosition = 0;
    m_texutreNormal = 0;

    ConfigureFramebuffer(m_width, m_height);
    ConfigureRenderbuffer(m_width, m_height);
    InitScreen();

}

CDeferred::~CDeferred()
{
    DestroyScreen();
    DestroyFramebuffer();
    DestroyRenderbuffer();
}

void CDeferred::Prepare() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_gShader.Use();
}

void CDeferred::Draw(const glm::vec3& cameraPosition) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_diffuseShader.Use();
    m_diffuseShader.Set("gPosition", 0);
    m_diffuseShader.Set("gNormal", 1);
    m_diffuseShader.Set("gAlbedoSpec", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texturePosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texutreNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_textureAlbedo);

    m_diffuseShader.Set("lightPos", glm::vec3(4.0f, 4.0f, 4.0f));
    m_diffuseShader.Set("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_diffuseShader.Set("viewPos", cameraPosition);
    
    RenderScreen();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint CDeferred::DrawToTexture(const glm::vec3& cameraPosition) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_finalbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_diffuseShader.Use();
    m_diffuseShader.Set("gPosition", 0);
    m_diffuseShader.Set("gNormal", 1);
    m_diffuseShader.Set("gAlbedoSpec", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texturePosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texutreNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_textureAlbedo);

    m_diffuseShader.Set("viewPos", cameraPosition);

    RenderScreen();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return m_finalTexture;
}

void CDeferred::InitScreen()
{
    float vertices[] = {
        -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &m_screenVAO);
    glGenBuffers(1, &m_screenVBO);

    glBindVertexArray(m_screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void CDeferred::RenderScreen() const
{
    glBindVertexArray(m_screenVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void CDeferred::DestroyScreen()
{
    glDeleteVertexArrays(1, &m_screenVAO);
    glDeleteBuffers(1, &m_screenVBO);
}

void CDeferred::ResizeFramebuffer(int width, int height)
{
    m_width = width;
    m_height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glBindTexture(GL_TEXTURE_2D, m_texturePosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_texutreNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_textureAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Finalbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_finalbuffer);

    glBindTexture(GL_TEXTURE_2D, m_finalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CDeferred::ConfigureFramebuffer(int width, int height)
{
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenTextures(1, &m_texturePosition);
    glGenTextures(1, &m_texutreNormal);
    glGenTextures(1, &m_textureAlbedo);
    
    glBindTexture(GL_TEXTURE_2D, m_texturePosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texturePosition, 0);

    glBindTexture(GL_TEXTURE_2D, m_texutreNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_texutreNormal, 0);

    glBindTexture(GL_TEXTURE_2D, m_textureAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_textureAlbedo, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    constexpr GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    // Finalbuffer
    glGenFramebuffers(1, &m_finalbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_finalbuffer);

    glGenTextures(1, &m_finalTexture);
    glBindTexture(GL_TEXTURE_2D, m_finalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_finalTexture, 0);

    glDrawBuffers(1, attachments);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CDeferred::ConfigureRenderbuffer(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenRenderbuffers(1, &m_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, m_finalbuffer);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        System::Warning() << "OpenGL framebuffer incomplete.";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CDeferred::DestroyRenderbuffer()
{
    glDeleteRenderbuffers(1, &m_renderbuffer);
}

void CDeferred::DestroyFramebuffer()
{
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteTextures(1, &m_texturePosition);
    glDeleteTextures(1, &m_texutreNormal);
    glDeleteTextures(1, &m_textureAlbedo);
    // Finalbuffer
    glDeleteFramebuffers(1, &m_finalbuffer);
    glDeleteTextures(1, &m_finalTexture);
}

void CDeferred::Resize(int width, int height)
{
    ResizeFramebuffer(width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glViewport(0, 0, width, height);

    m_width = width;
    m_height = height;
}

const CShader& CDeferred::GetGShader() const
{
    return m_gShader;
}

const CShader& CDeferred::GetDiffuseShader() const
{
    return m_diffuseShader;
}