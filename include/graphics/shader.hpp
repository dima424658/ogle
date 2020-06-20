#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

namespace Graphics
{
    class CShader
    {
    public:
        CShader(const std::string& vertexPath, const std::string& fragmentPath);
        CShader(const CShader&) = delete;
        ~CShader();

        void Use() const;
        unsigned int GetID() const;
        
        void Set(const std::string_view& name, const glm::mat4& arg) const;
        void Set(const std::string_view& name, const glm::vec3& arg) const;
        void Set(const std::string_view& name, const glm::vec4& arg) const;
        void Set(const std::string_view& name, GLfloat arg) const;
        void Set(const std::string_view& name, GLint arg) const;
        
    private:
        unsigned int m_id;
    };
};