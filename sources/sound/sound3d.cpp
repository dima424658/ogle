#include "sound/sound3d.hpp"

using namespace Sound;

CSound3D::CSound3D(const char* path, bool loop) : CSound2D(path, loop)
{
    // !!!
    FMOD_RESULT result;
    m_position = m_velocity = {0, 0, 0};

    result = m_sound->setMode(FMOD_3D);
    
    result = m_sound->set3DMinMaxDistance(0.5f * g_distanceFactor, 5000.0f * g_distanceFactor);

    result = CDevice::Instance().GetSystem()->playSound(m_sound, 0, false, &m_channel);

    result = m_channel->set3DAttributes(&m_position, &m_velocity);
}

void CSound3D::Update(float deltaTime, FMOD_VECTOR position)
{
    // !!!
    FMOD_RESULT result;

    m_velocity = (position - m_position) * (1000.0f / deltaTime);
    m_position = position;

    result = m_channel->set3DAttributes(&m_position, &m_velocity);
}

CSound3D::~CSound3D()
{

}

CSound3D* CSound3D::Factory(std::string& path)
{
    return new CSound3D(path.c_str(), true);
}

void CSound3D::RegisterScript()
{
    Script::RegisterObjectType("Sound3D",0,asOBJ_REF);
    Script::RegisterObjectMethod("Sound3D"," void Play()",asMETHOD(Sound::CSound3D, Play),asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound3D"," void Pause()",asMETHOD(Sound::CSound3D, Pause),asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound3D"," void SetVolume(float &in)",asMETHOD(Sound::CSound3D, SetVolume),asCALL_THISCALL);
    Script::RegisterObjectMethod("Sound3D"," float GetVolume()",asMETHOD(Sound::CSound3D, GetVolume),asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound3D",asBEHAVE_FACTORY,"Sound3D@ new_Sound3D(string &in)",asFUNCTION(CSound3D::Factory),asCALL_CDECL);
    Script::RegisterObjectBehaviour("Sound3D",asBEHAVE_ADDREF,"void AddRef()",asMETHOD(Sound::CSound3D,AddRef),asCALL_THISCALL);
    Script::RegisterObjectBehaviour("Sound3D",asBEHAVE_RELEASE,"void Release()",asMETHOD(Sound::CSound3D,Release),asCALL_THISCALL);
}