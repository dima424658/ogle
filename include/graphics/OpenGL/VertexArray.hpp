#pragma once

#include <utility>
#include <glad/glad.h>

namespace OpenGL
{
    class VertexArray
    {
    public:
        VertexArray() noexcept
        {
            glGenVertexArrays(1, &m_data);
        }

        VertexArray(const VertexArray &) = delete;
        VertexArray &operator=(const VertexArray &) = delete;

        VertexArray(VertexArray &&o) noexcept
        {
            m_data = o.m_data;
            o.m_data = 0;
        }

        VertexArray &operator=(VertexArray &&o) noexcept
        {
            std::swap(m_data, o.m_data);

            return *this;
        }

        ~VertexArray()
        {
            if (m_data > 0)
            {
                glDeleteVertexArrays(1, &m_data);
                m_data = 0;
            }
        }

        operator GLuint() const noexcept
        {
            return m_data;
        }

    private:
        GLuint m_data;
    };
}; // namespace OpenGL