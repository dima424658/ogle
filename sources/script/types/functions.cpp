#include "script/types/functions.hpp"

void Script::RegisterFunctions(asIScriptEngine* engine)
{
    int result;

	if( !strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
	{
		result = engine->RegisterGlobalFunction("void Log(string &in)", asFUNCTION(Script::Log), asCALL_CDECL);
        if(result < 0)
        {
           	System::Log() << "Failed to register global function void Log(string&).\n";
		    System::Exit(); 
        }
	}
	else
	{
		result = engine->RegisterGlobalFunction("void Log(string &in)", asFUNCTION(Script::Log_Generic), asCALL_GENERIC);
        if(result < 0)
        {
           	System::Log() << "Failed to register global generic function void Log(string&).\n";
		    System::Exit(); 
        }
	}

}

void Script::Log(std::string& value)
{
	System::Log() << value;
}

void Script::Log_Generic(asIScriptGeneric *generic)
{
	System::Log() << static_cast<std::string*>(generic->GetArgAddress(0))->c_str();
}

