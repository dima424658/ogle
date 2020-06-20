#pragma once

#include "sound/sound_device.hpp"

namespace Sound
{
    // Units per meter. feet = 3.28.  centimeters = 100.

    class CSoundListener
    {
    public:
        CSoundListener();
        ~CSoundListener();
        //Delta Time in ms. Position in meters
        void Update(float deltaTime, FMOD_VECTOR position);

    private:
        FMOD_VECTOR m_position;
        FMOD_VECTOR m_forward;
        FMOD_VECTOR m_up;
        FMOD_VECTOR m_velocity;
    };
};