#pragma once
#include <sound/sound2d.hpp>

namespace Sound
{
    class CSound3D : public CSound2D
    {
    public:
        CSound3D();
        CSound3D(std::string_view path, bool loop = false);
        ~CSound3D();

        static CSound3D* Factory(std::string& path);
        static void RegisterScript();

        virtual void Load(std::string_view path) override;

        void Update(float deltaTime, FMOD_VECTOR position);
    private:
        FMOD_VECTOR m_position;
        FMOD_VECTOR m_velocity;

    };
};