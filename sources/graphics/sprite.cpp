#include <graphics/sprite.hpp>

using namespace Graphics;

CSprite::CSprite() : m_shader("shaders/sprite.vs", "shaders/sprite.fs")
{
    float vertices[] = {
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
}

CSprite::~CSprite()
{
    
}

void CSprite::Render(const glm::vec2& screenSize, const glm::vec2& spriteSize,
         GLuint texture, const glm::vec3& color, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.Use();
    m_shader.Set("uTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    m_shader.Set("uColor", color);

    m_shader.Set("uScreenSize", screenSize);
    m_shader.Set("uSpriteSize", spriteSize);

    m_shader.Set("model", model);
    m_shader.Set("view", view);
    m_shader.Set("orthographic", projection);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}