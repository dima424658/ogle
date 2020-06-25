#pragma once

#include "sound/device.hpp"
#include "script/device.hpp"
#include "script/reference.hpp"

namespace Sound
{
    class CSound2D : public Script::iReference
    {
    public:
        CSound2D(const char* path, bool loop);
        ~CSound2D();

        static CSound2D* Factory(std::string& path);
        static void RegisterScript();

        void Play();
        void Pause();

        bool IsPlaying() const;

        void SetVolume(float &vol);
        float GetVolume();
        std::string GetName();

    protected:
        FMOD::Sound* m_sound;
        FMOD::Channel* m_channel;
        float m_volume;
    };

};