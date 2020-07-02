#include "script/script.hpp"

using namespace Script;


CScript::CScript(std::string_view path)
{
	std::ifstream fileStream;
    try
    {
        fileStream.open(path.data());
    }
    catch(const std::exception& e)  // TODO: сделать пустой скрипт и не вылетать.
    {
        System::Error() << "Failed to open script file: " << path << '\n' << "Error: " << e.what();
        System::Exit();
    }

	std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    int result;
	asIScriptModule* mod = CDevice::Instance().GetModule(0);
	result = mod->AddScriptSection("script", content.data(), content.size());
	if( result < 0 ) // TODO
	{
        System::Error() << "Module AddScriptSection() failed.";
        System::Exit();
	}

    result = mod->Build();
	if( result < 0 ) 
	{
        System::Error() << "Module Build() failed." << '\n';
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
        System::Error() << "Could not find function " << decl;
        System::Exit();
    }

    result = CDevice::Instance().GetContext()->Prepare(function);
	if( result < 0 ) 
	{
        System::Error() << "Failed to prepare script context.";
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
			System::Warning() << "The script was aborted before it could finish. Probably it timed out.\n";
		else if( result == asEXECUTION_EXCEPTION )
		{ // TODO
			asIScriptFunction *func = CDevice::Instance().GetContext()->GetExceptionFunction();
			System::Warning() << "The script ended with an exception.\n"
			<< "Function: " << func->GetDeclaration() << '\n'
			<< "Module: " << func->GetModuleName() << '\n'
			<< "Section: " << func->GetScriptSectionName() << '\n'
			<< "Line: " << CDevice::Instance().GetContext()->GetExceptionLineNumber() << '\n'
			<< "Description: " << CDevice::Instance().GetContext()->GetExceptionString();
		}
		else
			System::Warning() << "The script ended for some unforeseen reason (" << result << ")." << '\n';
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

