#pragma once

#include <utility>
#include <glad/glad.h>

namespace OpenGL
{
    class Buffer
    {
    public:
        Buffer(GLsizeiptr size = 0, const void * data = nullptr, GLenum usage = GL_STATIC_DRAW) noexcept
        {
            glGenBuffers(1, &m_data);
	        glBindBuffer(GL_ARRAY_BUFFER, m_data); // TODO: GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
            //if(size > 0) // TODO
                glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        }

        Buffer(const Buffer &o) : m_size(o.size())
        {              
            if(this == &o)
                return;

            glGenBuffers(1, &m_data);
            glBindBuffer(GL_COPY_READ_BUFFER, o);
            glBindBuffer(GL_COPY_WRITE_BUFFER, m_data);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, o.size());
        }

        Buffer &operator=(const Buffer &o)
        {
            if(this == &o)
                return *this;

            m_size = o.size();
            glBindBuffer(GL_COPY_READ_BUFFER, o);
            glBindBuffer(GL_COPY_WRITE_BUFFER, m_data);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, o.size());

            return *this;
        }

        Buffer(Buffer &&o) noexcept
        {
            m_data = o.m_data;
            m_size = o.m_size;
            o.m_data = 0;
            o.m_size = 0;
        }

        Buffer &operator=(Buffer &&o) noexcept
        {
            std::swap(m_data, o.m_data);
            std::swap(m_size, o.m_size);

            return *this;
        }

        ~Buffer()
        {
            if (m_data > 0)
            {
                glDeleteBuffers(1, &m_data);
                m_data = 0;
            }
        }

        operator GLuint() const noexcept
        {
            return m_data;
        }

        GLsizeiptr size() const noexcept
        {
            return m_size;
        }
     
    private:
        GLuint m_data;
        GLsizeiptr m_size;
    };
}; // namespace OpenGL