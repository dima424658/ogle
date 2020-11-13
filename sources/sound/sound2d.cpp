#include <sound/sound2d.hpp>

using namespace Sound;

CSound2D::CSound2D()
{
    m_volume = 1.0f;
    m_sound = nullptr;
    m_channel = nullptr;
}

CSound2D::CSound2D(std::string_view path, bool loop)
{
    m_volume = 1.0f;
    m_sound = nullptr;
    m_channel = nullptr;

    Load(path);

    if (m_sound)
    {
        FMOD_RESULT result = m_sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
        if (result != FMOD_OK)
        {
            System::Error() << "Failed to set mode of FMOD sound: " << Sound::Error(result);
            //System::Exit();
        }
    }
}

void CSound2D::Play()
{
    if (m_channel)
    {
        FMOD_RESULT result = m_channel->setPaused(false);
        if (result != FMOD_OK)
            System::Error() << "Failed to play FMOD sound: " << Sound::Error(result);
    }
}

void CSound2D::Pause()
{
    if (m_channel)
    {
        FMOD_RESULT result = m_channel->setPaused(true);
        if (result != FMOD_OK)
        {
            System::Error() << "Failed to pause FMOD sound: " << Sound::Error(result);
            System::Exit();
        }
    }
}

bool Sound::CSound2D::IsPlaying() const
{
    if (m_channel)
    {
        bool pause;
        m_channel->getPaused(&pause);

        return !pause;
    }
    else
        return false;
}

CSound2D::~CSound2D()
{
    if (m_sound)
    {
        FMOD_RESULT result = m_sound->release();
        if (result != FMOD_OK)
            System::Error() << "Failed to destroy FMOD sound: " << Sound::Error(result);
    }

    m_sound = nullptr;
    m_channel = nullptr;
}

void CSound2D::Load(std::string_view path)
{
    FMOD::Sound *sound = nullptr;
    FMOD::Channel *channel = nullptr;

    FMOD_RESULT result;
    result = CDevice::Instance().GetSystem()->createSound(path.data(), FMOD_DEFAULT, 0, &sound);
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

        this->~CSound2D();
        m_channel = channel;
        m_sound = sound;
    }
}

CSound2D *CSound2D::Factory(std::string &path)
{
    return new CSound2D(path.c_str(), true);
}

void CSound2D::RegisterScript()
{
    Script::RegisterObjectType("Sound2D", 0, asOBJ_REF);
    Script::RegisterObjectMethod("Sound2D", " void Play()", asMETHOD(Sound::CSound2D, Play), asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound2D", " void Pause()", asMETHOD(Sound::CSound2D, Pause), asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound2D", " void SetVolume(float &in)", asMETHOD(Sound::CSound2D, SetVolume), asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound2D", " float GetVolume()", asMETHOD(Sound::CSound2D, GetVolume), asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound2D", asBEHAVE_FACTORY, "Sound2D@ new_Sound2D(string &in)", asFUNCTION(CSound2D::Factory), asCALL_CDECL);
    Script::RegisterObjectBehaviour("Sound2D", asBEHAVE_ADDREF, "void AddRef()", asMETHOD(Sound::CSound2D, AddRef), asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound2D", asBEHAVE_RELEASE, "void Release()", asMETHOD(Sound::CSound2D, Release), asCALL_THISCALL);
}

void CSound2D::SetVolume(float &vol)
{
    if (m_channel)
    {
        FMOD_RESULT result = m_channel->setVolume(m_volume);
        if (result != FMOD_OK)
            System::Error() << "Failed to set volume of FMOD sound\n"
                            << Sound::Error(result);
        else
            m_volume = vol;
    }
}

float CSound2D::GetVolume()
{
    if (m_channel)
    {
        FMOD_RESULT result = m_channel->getVolume(&m_volume);
        if (result != FMOD_OK)
            System::Error() << "Failed to get volume of FMOD sound: " << Sound::Error(result);
    }

    return m_volume;
}

std::string Sound::CSound2D::GetName()
{
    if (m_sound)
    {
        char name[256];
        m_sound->getName(name, 256);

        return std::string(name);
    }
    else
        return std::string();
}
