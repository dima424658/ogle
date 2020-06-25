//Это лучшее, что придумало человечество!

#pragma once

#include <string>
#include <fstream>
#include <string_view>
#include "system/log.hpp"
#include "script/device.hpp"

namespace Script
{
    class CScript
    {
    public:
        CScript(std::string_view path);
        ~CScript();

        void PrepareFunction(std::string_view decl);
        asIScriptContext* GetContext();
        void* Execute();
    private:
    };
};