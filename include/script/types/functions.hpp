#pragma once

#include <cstring>
#include <angelscript.h>
#include "system/log.hpp"

#include "glm/glm.hpp"

namespace Script
{
    void Log(std::string&);
    void LogString_Generic(asIScriptGeneric *generic);

    void Log(float&);
    void LogFloat_Generic(asIScriptGeneric *generic);


    void Log(const glm::vec3&);
    void LogVec3_Generic(asIScriptGeneric *generic);

    void RegisterFunctions(asIScriptEngine* engine);
}