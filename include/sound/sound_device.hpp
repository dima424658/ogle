#pragma once

#include "system/log.hpp"
#include "fmod.hpp"

namespace Sound
{
    constexpr float g_distanceFactor = 1.0f;
    const char* Error(FMOD_RESULT errcode);

    class CDevice
    {
    private:
        CDevice();
        CDevice(const CDevice&) = delete;  
        CDevice& operator=(CDevice&) = delete;

    public:
        ~CDevice();
        static CDevice& Instance();
        
        void Update();

        unsigned int GetVersion();
        FMOD::System* GetSystem();
        FMOD::Channel* GetChannel();

    private:
        FMOD::System* m_system;
        FMOD::Channel* m_channel;
        unsigned int m_version;
    };
};

inline FMOD_VECTOR operator+(FMOD_VECTOR const &l, FMOD_VECTOR const &r)
{ 
    return { l.x + r.x, l.y + r.y, l.z + r.z };
} 

inline FMOD_VECTOR operator-(FMOD_VECTOR const &l, FMOD_VECTOR const &r)
{ 
    return { l.x - r.x, l.y - r.y, l.z - r.z };
} 


inline FMOD_VECTOR operator*(FMOD_VECTOR const &l, float r)
{ 
    return { l.x * r, l.y * r, l.z * r };
} 
