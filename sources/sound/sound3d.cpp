#include "sound/sound3d.hpp"

using namespace Sound;

CSound3D::CSound3D() : CSound2D()
{
    m_position = m_velocity = {0, 0, 0};
}

CSound3D::CSound3D(std::string_view path, bool loop) : CSound2D()
{
    // TODO
    FMOD_RESULT result;
    m_position = m_velocity = {0, 0, 0};

    Load(path);
}

void CSound3D::Update(float deltaTime, FMOD_VECTOR position)
{
    if (m_channel)
    {
        // TODO
        FMOD_RESULT result;
        position = position * g_distanceFactor;
        m_velocity = (position - m_position) / (deltaTime);
        m_position = position;

        result = m_channel->set3DAttributes(&m_position, &m_velocity);
        if (result != FMOD_OK)
        {
            System::Error() << "Failed to update FMOD sound: " << Sound::Error(result);
        }
    }
}

CSound3D::~CSound3D()
{
}

void CSound3D::Load(std::string_view path)
{
    FMOD::Sound *sound = nullptr;
    FMOD::Channel *channel = nullptr;

    FMOD_RESULT result;
    result = CDevice::Instance().GetSystem()->createSound(path.data(), FMOD_3D, 0, &sound);
    if (result != FMOD_OK)
    {
        System::Error() << "Failed to initialize FMOD sound: " << Sound::Error(result);
        m_sound = nullptr;
    }

    result = CDevice::Instance().GetSystem()->playSound(sound, 0, false, &channel);
    if (result != FMOD_OK)
    {
        System::Error() << "Failed to initialize FMOD sound: " << Sound::Error(result);
        channel = nullptr;
    }


    if (channel && sound)
    {
        result = channel->setPaused(true);
        result = channel->setVolume(m_volume);
        result = channel->setMode(FMOD_3D);
        result = m_sound->set3DMinMaxDistance(0.5f * g_distanceFactor, 5000.0f * g_distanceFactor);
        result = m_channel->set3DAttributes(&m_position, &m_velocity);

        this->~CSound3D();
        m_channel = channel;
        m_sound = sound;
    }
}

CSound3D *CSound3D::Factory(std::string &path)
{
    return new CSound3D(path.c_str(), true);
}

void CSound3D::RegisterScript()
{
    Script::RegisterObjectType("Sound3D", 0, asOBJ_REF);
    Script::RegisterObjectMethod("Sound3D", " void Play()", asMETHOD(Sound::CSound3D, Play), asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound3D", " void Pause()", asMETHOD(Sound::CSound3D, Pause), asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound3D", " void SetVolume(float &in)", asMETHOD(Sound::CSound3D, SetVolume), asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound3D", " float GetVolume()", asMETHOD(Sound::CSound3D, GetVolume), asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound3D", asBEHAVE_FACTORY, "Sound3D@ new_Sound3D(string &in)", asFUNCTION(CSound3D::Factory), asCALL_CDECL);
    Script::RegisterObjectBehaviour("Sound3D", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(Sound::CSound3D, AddRef), asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound3D", asBEHAVE_RELEASE, "void Release()", asMETHOD(Sound::CSound3D, Release), asCALL_THISCALL);
}