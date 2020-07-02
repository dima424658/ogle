#pragma once

#include <glm/glm.hpp>

#include <graphics/texture.hpp>
#include <array>

namespace Graphics
{
    struct SColor4
    {
    private:
        float color[4];
    public:
        SColor4(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        {
            color[0] = r;
            color[1] = g;
            color[2] = b;
            color[3] = a;
        }

        SColor4& operator=(const glm::vec4& vec)
        {
            this->color[0] = vec.r;
            this->color[1] = vec.g;
            this->color[2] = vec.b;
            this->color[3] = vec.a;

            return *this;
        }

        operator float*() const
        {
            return const_cast<float*>(color);
        }

        float operator[](size_t i) const
        {
            if(i < 4)
                return color[i];
            else 
                return 1.0f;
        }

        float r() const
        {
            return color[0];
        }

        float g() const
        {
            return color[1];
        }

        float b() const
        {
            return color[2];
        }

        float a() const
        {
            return color[3];
        }

        float max() const
        {
            return std::max(std::max(color[0], color[1]), std::max(color[2], color[3]));
        }

        operator glm::vec4() const
        {
            return glm::vec4(color[0], color[1], color[2], color[3]);
        }

    };

    static SColor4 operator/(const SColor4& col, float val)
    {
        if(val == 0.0f)
            return SColor4(1.0f, 1.0f, 1.0f, 1.0f);

        return SColor4(col[0] / val, col[1] / val, col[2] / val, col[3] / val);
    }

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
        void SetDiffuseColor(const SColor4& color);
        const SColor4& GetDiffuseColor() const;
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
        SColor4 m_diffuseColor;
        SColor4 m_normalColor;
        float m_specularPower;

        std::array<CTexture*, 3> m_textures;
    };
};