#pragma once

#include "sound/sound_device.hpp"
#include "script/script_device.hpp"
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
        void SetVolume(float &vol);
        float GetVolume();

    protected:
        FMOD::Sound* m_sound;
        FMOD::Channel* m_channel;
        float m_volume;
    };

};