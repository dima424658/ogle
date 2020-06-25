#pragma once

#include "system/log.hpp"

#include <fmod.hpp>
#include <glm/glm.hpp>

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

inline FMOD_VECTOR operator+(const FMOD_VECTOR& l, const FMOD_VECTOR& r)
{ 
    return { l.x + r.x, l.y + r.y, l.z + r.z };
} 

inline FMOD_VECTOR operator-(const FMOD_VECTOR& l, const FMOD_VECTOR& r)
{ 
    return { l.x - r.x, l.y - r.y, l.z - r.z };
} 


inline FMOD_VECTOR operator*(const FMOD_VECTOR& l, float r)
{ 
    return { l.x * r, l.y * r, l.z * r };
} 

inline FMOD_VECTOR toFMOD_VECTOR(const glm::vec3& o)
{
    return { o.x, o.y, o.z };
}
