#include <graphics/material.hpp>

using namespace Graphics;

CMaterial::CMaterial()
{
    for(size_t i = 0; i < MatID_size; i++)
    {
        m_useTexutre[i] = false;
        m_textures[i] = nullptr;
    }
    m_diffuseColor = SColor4(1.0f);
    m_specularPower = 1.0f;
}

CMaterial::~CMaterial()
{
    for(size_t i = 0; i < MatID_size; i++)
        DestroyTexture(i);
}

// Diffuse texture

bool CMaterial::HasDiffuse() const
{
    return HasTexture(MatID::MatDiffuse);
}

void CMaterial::LoadDiffuse(std::string_view path)
{
    LoadTexture(MatID::MatDiffuse, path);
}

void CMaterial::SetDiffuseColor(const SColor4& color)
{

    if(color.max() > 1.0f)
        m_diffuseColor = color / color.max();
    else 
        m_diffuseColor = color;
}

void CMaterial::DestroyDiffuse()
{
    DestroyTexture(MatID::MatDiffuse);
}

const SColor4& CMaterial::GetDiffuseColor() const
{
    return m_diffuseColor;
}

const CTexture* CMaterial::GetDiffuse() const
{
    return GetTexture(MatID::MatDiffuse);
}

// Normal texture

bool CMaterial::HasNormal() const
{
    return HasTexture(MatID::MatNormal);
}

void CMaterial::LoadNormal(std::string_view path)
{
    LoadTexture(MatID::MatNormal, path);
}

void CMaterial::DestroyNormal()
{
    DestroyTexture(MatID::MatNormal);
}

const CTexture* CMaterial::GetNormal() const
{
    return GetTexture(MatID::MatNormal);
}

// Specular texture

bool CMaterial::HasSpecular() const
{
    return HasTexture(MatID::MatSpecular);
}

void CMaterial::LoadSpecular(std::string_view path)
{
    LoadTexture(MatID::MatSpecular, path);
}

void CMaterial::SetSpecularPower(float val)
{
    if(val >= 1.0f)
        m_specularPower = 1.0f;
    else
        m_specularPower = val;
}

void CMaterial::DestroySpecular()
{
    DestroyTexture(MatID::MatSpecular);
}

float CMaterial::GetSpecularPower() const
{
    return m_specularPower;
}

const CTexture* CMaterial::GetSpecular() const
{
    return GetTexture(MatID::MatSpecular);
}

// Any texture

bool CMaterial::HasTexture(size_t i) const
{
    return m_useTexutre[i];
}

void CMaterial::LoadTexture(size_t i, std::string_view path)
{
    if(m_textures[i])
    {
        delete m_textures[i];
        m_textures[i] = nullptr;
    }

    m_textures[i] = new CTexture(path);
    m_useTexutre[i] = true;
}

void CMaterial::DestroyTexture(size_t i)
{
    if(m_textures[i])
    {
        delete m_textures[i];
        m_textures[i] = nullptr;
    }

    m_useTexutre[i] = false;
}

const CTexture* CMaterial::GetTexture(size_t i) const
{
    return m_textures[i];
}