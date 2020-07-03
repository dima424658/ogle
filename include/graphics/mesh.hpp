#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <string_view>

#include <fstream>
#include <vector>
#include <iostream>

#include <system/log.hpp>

namespace Graphics
{
    class CMesh
    {
    private:
        struct Face
        {
            size_t vertex;
            size_t normal;
            size_t texture;
        };

    public:
        CMesh();
        CMesh(std::string_view path);
        CMesh(const CMesh&) = delete;
        ~CMesh();

        void LoadModel(std::string_view path);
        const std::string& GetName() const;
        const std::string& GetPath() const;

        bool isLoaded() const;

        void Draw() const;
    private:
        bool ReadFromObj(std::string_view path);

        std::string m_name;
        std::string m_modelPath;
        std::vector<float> m_data;
        GLuint m_VAO, m_VBO;
    };
}