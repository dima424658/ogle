#include "sound/sound2d.hpp"

using namespace Sound;

CSound2D::CSound2D(const char* path, bool loop)
{
    m_volume = 100.0f;
    FMOD_RESULT result;
    result = CDevice::Instance().GetSystem()->createSound(path, FMOD_DEFAULT, 0, &m_sound);
    
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to initialize FMOD sound: " << Sound::Error(result);
        System::Exit();
    }

    result = m_sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to set mode of FMOD sound: " << Sound::Error(result);
        System::Exit();
    }  

    result = CDevice::Instance().GetSystem()->playSound(m_sound, 0, false, &m_channel);
    if (result != FMOD_OK)
    {
        System::Error() << "Failed to play FMOD sound: " << Sound::Error(result);
        System::Exit();
    }

    result = m_channel->setPaused(true);
    result = m_channel->setVolume(m_volume);
}

void CSound2D::Play()
{
    FMOD_RESULT result;

    result = m_channel->setPaused(false);
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to play FMOD sound: " << Sound::Error(result);
        System::Exit();
    }  
}

void CSound2D::Pause()
{
    FMOD_RESULT result;

    result = m_channel->setPaused(true);
    if (result != FMOD_OK)
    {
        System::Error() << "Failed to pause FMOD sound: " << Sound::Error(result);
        System::Exit();
    }
}

bool Sound::CSound2D::IsPlaying() const
{
    bool pause;
    m_channel->getPaused(&pause);

    return !pause;
}

CSound2D::~CSound2D()
{
    FMOD_RESULT result;
    result = m_sound->release();
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to initialize FMOD sound: " << Sound::Error(result);
        System::Exit();
    }
}


CSound2D* CSound2D::Factory(std::string& path)
{
    return new CSound2D(path.c_str(), true);
}

void CSound2D::RegisterScript()
{
    Script::RegisterObjectType("Sound2D",0,asOBJ_REF);
    Script::RegisterObjectMethod("Sound2D"," void Play()",asMETHOD(Sound::CSound2D, Play),asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound2D"," void Pause()",asMETHOD(Sound::CSound2D, Pause),asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound2D"," void SetVolume(float &in)",asMETHOD(Sound::CSound2D, SetVolume),asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound2D"," float GetVolume()",asMETHOD(Sound::CSound2D, GetVolume),asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound2D",asBEHAVE_FACTORY,"Sound2D@ new_Sound2D(string &in)",asFUNCTION(CSound2D::Factory),asCALL_CDECL);
    Script::RegisterObjectBehaviour("Sound2D",asBEHAVE_ADDREF,"void AddRef()",asMETHOD(Sound::CSound2D,AddRef),asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound2D",asBEHAVE_RELEASE,"void Release()",asMETHOD(Sound::CSound2D,Release),asCALL_THISCALL);
}

void CSound2D::SetVolume(float &vol)
{
    FMOD_RESULT result;
    m_volume = vol;
    result = m_channel->setVolume(m_volume);
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to set volume of FMOD sound\n" << Sound::Error(result);
        System::Exit();
    }
}

float CSound2D::GetVolume()
{
    FMOD_RESULT result;
    result = m_channel->getVolume(&m_volume);
    if(result != FMOD_OK)
    {
        System::Error() << "Failed to get volume of FMOD sound: " << Sound::Error(result);
        System::Exit();
    }

    return m_volume;
}

std::string Sound::CSound2D::GetName()
{
    char name[256];
    m_sound->getName(name, 256);
    
    return std::string(name);
}
