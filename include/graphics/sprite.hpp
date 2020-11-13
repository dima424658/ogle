#pragma once

#include <graphics/shader.hpp>

namespace Graphics
{
    class CSprite
    {
    public:
        CSprite();
        ~CSprite();

        void Render(const glm::vec2& screenSize, const glm::vec2& spriteSize,
         GLuint texture, const glm::vec3& color, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

    private:
        GLuint m_VAO, m_VBO;
        CShader m_shader;
    };

} // namespace Graphics