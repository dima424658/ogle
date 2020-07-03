#pragma once

#include <glm/glm.hpp>

namespace Graphics
{
    class CLight
    {
    public:
        CLight(const glm::vec3 &color = {1.0f, 1.0f, 1.0f}) : m_color(color), m_linear(0.7f), m_quadratic(1.8f)
        {
        }
        // епрст TODO: надо выделить компоненты в
        // отдельную категорию и ваще сделать наследования,
        // а то у меня свет без позиции остался - придется цеплять его из самого объекта...

        ~CLight()
        {
        }

        void SetColor(const glm::vec3 &color)
        {
            m_color = color;
        }

        const glm::vec3 &GetColor() const
        {
            return m_color;
        }

        void SetLinear(float val)
        {
            m_linear = val;
        }

        float GetLinear() const
        {
            return m_linear;
        }

        void SetQuadratic(float val)
        {
            m_quadratic = val;
        }

        float GetQuadratic() const
        {
            return m_quadratic;
        }


    private:
        glm::vec3 m_color;
        float m_linear;
        float m_quadratic;
    };
}; // namespace Graphics