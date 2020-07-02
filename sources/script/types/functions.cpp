#include "script/types/functions.hpp"

void Script::RegisterFunctions(asIScriptEngine* engine) // TODO: вывести методы движка в отдельный файл
{
    int result;

	if(!strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
	{
		result = engine->RegisterGlobalFunction("void Log(string &in)", asFUNCTIONPR(Script::Log, (std::string& value), void), asCALL_CDECL);
        if(result < 0)
        {
           	System::Error() << "Failed to register global function void Log(string&).";
		    System::Exit(); 
        }
		
		result = engine->RegisterGlobalFunction("void Log(float &in)", asFUNCTIONPR(Script::Log, (float& value), void), asCALL_CDECL);
        if(result < 0)
        {
           	System::Error() << "Failed to register global function void Log(string&).";
		    System::Exit(); 
        }

		result = engine->RegisterGlobalFunction("void Log(const glm::vec3 &in)", asFUNCTIONPR(Script::Log, (const glm::vec3& value), void), asCALL_CDECL);
        if(result < 0)
        {
           	System::Error() << "Failed to register global function void Log(const glm::vec3&).";
		    System::Exit(); 
        }
	}
	else
	{
		result = engine->RegisterGlobalFunction("void Log(string &in)", asFUNCTION(Script::LogString_Generic), asCALL_GENERIC);
        if(result < 0)
        {
           	System::Error() << "Failed to register global generic function void Log(string&).";
		    System::Exit(); 
        }

		result = engine->RegisterGlobalFunction("void Log(float &in)", asFUNCTION(Script::LogFloat_Generic), asCALL_GENERIC);
        if(result < 0)
        {
           	System::Error() << "Failed to register global generic function void Log(float&).";
		    System::Exit(); 
        }

		result = engine->RegisterGlobalFunction("void Log(const glm::vec3 &in)", asFUNCTION(Script::LogVec3_Generic), asCALL_GENERIC);
        if(result < 0)
        {
           	System::Error() << "Failed to register global generic function void Log(const glm::vec3&).";
		    System::Exit(); 
        }
	}

}

void Script::Log(std::string& value)
{
	System::Log() << value;
}

void Script::LogString_Generic(asIScriptGeneric *generic)
{
	System::Log() << static_cast<std::string*>(generic->GetArgAddress(0))->c_str();
}

void Script::Log(float& value)
{
	System::Log() << std::to_string(value);
}

void Script::LogFloat_Generic(asIScriptGeneric *generic)
{
	System::Log() << std::to_string(*static_cast<float*>(generic->GetArgAddress(0)));
}

void Script::Log(const glm::vec3& value)
{
	System::Log() << std::to_string(value.x) << " " << std::to_string(value.y) << " " << std::to_string(value.z);
}

void Script::LogVec3_Generic(asIScriptGeneric *generic)
{
	auto value = reinterpret_cast<glm::vec3*>(generic);
	System::Log() << std::to_string(value->x) << " " << std::to_string(value->y) << " " << std::to_string(value->z);
}