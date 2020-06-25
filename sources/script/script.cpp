#include "script/script.hpp"

using namespace Script;


CScript::CScript(std::string_view path)
{
	std::ifstream fileStream;
    try
    {
        fileStream.open(path.data());
    }
    catch(const std::exception& e)
    {
        System::Log() << "Failed to open script file: " << path << '\n';
        System::Log() << "Error: " << e.what() << '\n';
        System::Exit();
    }

	std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    int result;
	asIScriptModule* mod = CDevice::Instance().GetModule(0);
	result = mod->AddScriptSection("script", content.data(), content.size());
	if( result < 0 ) 
	{
        System::Log() << "Module AddScriptSection() failed." << '\n';
        System::Exit();
	}

    result = mod->Build();
	if( result < 0 ) 
	{
        System::Log() << "Module Build() failed." << '\n';
        System::Exit();
    }

    fileStream.close();
}


void CScript::PrepareFunction(std::string_view decl)
{
    int result;
	
	asIScriptModule* mod = CDevice::Instance().GetModule(0);
	
	//int a =mod->GetFunctionCount();
    asIScriptFunction* function = mod->GetFunctionByDecl("float calc(float, float)");
    if(!function)
    {
        System::Log() << "Could not find function " << decl << '\n';
        System::Exit();
    }

    result = CDevice::Instance().GetContext()->Prepare(function);
	if( result < 0 ) 
	{
        System::Log() << "Failed to prepare script context.\n";
        System::Exit();
	}
}


void* CScript::Execute()
{
    int result;
    result = CDevice::Instance().GetContext()->Execute();

    if( result != asEXECUTION_FINISHED )
	{
		// The execution didn't finish as we had planned. Determine why.
		if( result == asEXECUTION_ABORTED )
			System::Log() << "The script was aborted before it could finish. Probably it timed out.\n";
		else if( result == asEXECUTION_EXCEPTION )
		{
			System::Log() << "The script ended with an exception.\n";

			// Write some information about the script exception
			asIScriptFunction *func = CDevice::Instance().GetContext()->GetExceptionFunction();
			System::Log() << "func: " << func->GetDeclaration() << '\n';
			System::Log() << "modl: " << func->GetModuleName() << '\n';
			System::Log() << "sect: " << func->GetScriptSectionName() << '\n';
			System::Log() << "line: " << CDevice::Instance().GetContext()->GetExceptionLineNumber() << '\n';
			System::Log() << "desc: " << CDevice::Instance().GetContext()->GetExceptionString() << '\n';
		}
		else
			System::Log() << "The script ended for some unforeseen reason (" << result << ")." << '\n';
	}
	else
	{
		// Retrieve the return value from the context
		return CDevice::Instance().GetContext()->GetAddressOfReturnValue();
	}

	return nullptr;
}

asIScriptContext* CScript::GetContext()
{
	return CDevice::Instance().GetContext();
}

CScript::~CScript()
{
    
}

