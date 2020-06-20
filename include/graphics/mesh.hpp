#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

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
    CMesh(const std::string& path);
    CMesh(const CMesh&) = delete;
    ~CMesh();

    void Draw() const;
private:
    void ReadFromObj(const std::string& path);

    std::vector<float> m_data;
    GLuint m_VAO, m_VBO;
};