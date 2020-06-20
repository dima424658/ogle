#pragma once

#include <cstring>
#include <angelscript.h>
#include "system/log.hpp"

namespace Script
{
    void Log(std::string& generic);
    void Log_Generic(asIScriptGeneric *generic);
    void RegisterFunctions(asIScriptEngine* engine);
}

