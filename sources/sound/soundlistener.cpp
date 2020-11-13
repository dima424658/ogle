#include "sound/soundlistener.hpp"

using namespace Sound;

CSoundListener::CSoundListener()
{
    FMOD_RESULT result;
    m_forward = { 0.0f, 0.0f, -1.0f };
    m_up = { 0.0f, 1.0f, 0.0f };

    result = CDevice::Instance().GetSystem()->set3DSettings(1.0, g_distanceFactor, 1.0f);
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to initialize FMOD listener: " << Sound::Error(result);
        System::Exit();
    }

}

CSoundListener::~CSoundListener()
{

}

void CSoundListener::Update(float deltaTime, FMOD_VECTOR position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up)
{
    FMOD_RESULT result;

    position = position * g_distanceFactor;
    m_velocity = (position - m_position) / (deltaTime);
    m_position = position;

    result = CDevice::Instance().GetSystem()->set3DListenerAttributes(0, &m_position, &m_velocity, &forward, &up);
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to update FMOD listener: " << Sound::Error(result);
        System::Exit();
    }
}