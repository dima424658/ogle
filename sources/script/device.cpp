#include "script/device.hpp"

using namespace Script;

CDevice::CDevice()
{
    m_engine = asCreateScriptEngine();
    if(m_engine == nullptr)
	{
		System::Error() << "Failed to create script engine."; // TODO
		System::Exit();
	}

	m_engine->SetMessageCallback(asFUNCTION(Script::MessageCallback), 0, asCALL_CDECL);
    
    m_context = m_engine->CreateContext();
    if(!m_context)
    {
		System::Error() << "Failed to create script context.";
		System::Exit();
    }

    m_module = m_engine->GetModule(0, asGM_ALWAYS_CREATE);
    if(!m_module)
    {
        System::Error() << "Failed to get script module.";
        System::Exit();
    }
}

asIScriptModule* CDevice::GetModule(const char *moduleName)
{
    return m_module;
}

asIScriptContext* CDevice::GetContext()
{
    return m_context;
}

asIScriptEngine* CDevice::GetEngine()
{
    return m_engine;
}


CDevice::~CDevice()
{
    m_context->Release();
    m_engine->Release();
}

CDevice& CDevice::Instance()
{
    static CDevice instance;
    return instance;
}

void Script::MessageCallback(const asSMessageInfo *msg, void *param)
{
	if( msg->type == asMSGTYPE_WARNING ) 
        System::Warning() << "AngelScript: " << msg->section << " (" << msg->row << " : " << msg->col << ") : " << msg->message;
	else
    if( msg->type == asMSGTYPE_INFORMATION ) 
        System::Log() << "AngelScript: " << msg->section << " (" << msg->row << " : " << msg->col << ") : " << msg->message;
    else
        System::Error() << "AngelScript: " << msg->section << " (" << msg->row << " : " << msg->col << ") : " << msg->message;

}