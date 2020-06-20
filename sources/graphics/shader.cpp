#include "graphics/shader.hpp"

using namespace Graphics;

CShader::CShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    int result;
    
    std::ifstream file;

    file.open(vertexPath);
    if(!file.is_open())
        throw std::runtime_error(std::string("Failed to open shader file: ") + vertexPath);

    std::string vertexSrc((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    file.open(fragmentPath);
    if(!file.is_open())
        throw std::runtime_error(std::string("Failed to open shader file: ") + vertexPath);

    std::string fragmentSrc((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    const char* vertexBuf = vertexSrc.c_str();
    const char* fragmentBuf = fragmentSrc.c_str();

    unsigned int vertexHandle, fragmentHandle;
    
    vertexHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexHandle, 1, &vertexBuf, nullptr);
    glCompileShader(vertexHandle);    
    glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        char infoLog[1024];
        glGetShaderInfoLog(vertexHandle, 1024, nullptr, infoLog);
        throw std::runtime_error(std::string("Failed to compile vertex shader: ") + vertexPath + '\n' + infoLog);
    }

    fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentHandle, 1, &fragmentBuf, nullptr);
    glCompileShader(fragmentHandle);    
    glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        char infoLog[1024];
        glGetShaderInfoLog(fragmentHandle, 1024, nullptr, infoLog);
        throw std::runtime_error(std::string("Failed to compile fragment shader: ") + fragmentPath + '\n' + infoLog);
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexHandle);
    glAttachShader(m_id, fragmentHandle);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if(!result)
    {
        char infoLog[1024];
        glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);
        throw std::runtime_error(std::string("Failed to link shader program: \n") + std::string(infoLog));
    }

    glDeleteShader(vertexHandle);
    glDeleteShader(fragmentHandle);

}

CShader::~CShader()
{
    glDeleteProgram(m_id);
}

GLuint CShader::GetID() const
{
    return m_id;
}

void CShader::Set(const std::string_view& name, const glm::mat4& arg) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, &arg[0][0]);
}

void CShader::Set(const std::string_view& name, const glm::vec3& arg) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.data()), 1, &arg[0]); 
}

void CShader::Set(const std::string_view& name, const glm::vec4& arg) const
{
    glUniform4fv(glGetUniformLocation(m_id, name.data()), 1, &arg[0]); 
}

void CShader::Set(const std::string_view& name, GLfloat arg) const
{
    glUniform1f(glGetUniformLocation(m_id, name.data()), arg);
}

void CShader::Set(const std::string_view& name, GLint arg) const
{
    glUniform1i(glGetUniformLocation(m_id, name.data()), arg); 
}


void CShader::Use() const
{
    glUseProgram(m_id);
}
