#pragma once

#include <glm/glm.hpp>

#include <graphics/texture.hpp>
#include <array>
#include <cassert>

namespace Graphics
{
    class CMaterial
    {
    private:
        enum MatID
        {
            MatDiffuse = 0, MatNormal = 1, MatSpecular = 2
        };
        constexpr static size_t MatID_size = 3;

    public:
        CMaterial();
        ~CMaterial();

        bool HasDiffuse() const;
        void LoadDiffuse(std::string_view path);
        void DestroyDiffuse();
        void SetDiffuseColor(const glm::vec3& color);
        const glm::vec3& GetDiffuseColor() const;
        const CTexture* GetDiffuse() const;

        bool HasNormal() const;
        void LoadNormal(std::string_view path);
        void DestroyNormal();
        const CTexture* GetNormal() const;

        bool HasSpecular() const;
        void LoadSpecular(std::string_view path);
        void DestroySpecular();
        void SetSpecularPower(float val);
        float GetSpecularPower() const;
        const CTexture* GetSpecular() const;

    private:
        bool HasTexture(size_t i) const;
        void LoadTexture(size_t i, std::string_view path);
        void DestroyTexture(size_t i);
        const CTexture* GetTexture(size_t i) const;

    private:
        std::array<bool, 3> m_useTexutre;
        glm::vec3 m_diffuseColor;
        glm::vec3 m_normalColor;
        float m_specularPower;

        std::array<CTexture*, 3> m_textures;
    };
};