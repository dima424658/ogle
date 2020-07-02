#pragma once

#include <angelscript.h>


#include "system/log.hpp"


namespace Script
{
    class CDevice
    {
    private:
        CDevice();
        CDevice(const CDevice&) = delete;
        CDevice& operator=(CDevice&) = delete;
    public:
        ~CDevice();
        static CDevice& Instance();

        asIScriptModule* GetModule(const char *module);
        asIScriptContext* GetContext();
        asIScriptEngine* GetEngine();
    private:
        asIScriptEngine* m_engine;
        asIScriptModule* m_module;
        asIScriptContext* m_context;
    };

    void MessageCallback(asSMessageInfo const*, void *);


	inline void RegisterGlobalProperty(const char *declaration, void *pointer)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->RegisterGlobalProperty(declaration, pointer);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterGlobalProperty() failed." << '\n';
            System::Exit();
	    }
    }
	
    inline void SetDefaultNamespace(const char* space)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->SetDefaultNamespace(space);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterObjectType() failed." << '\n';
            System::Exit();
	    }
    }

    inline void RegisterObjectType(const char *obj, int byteSize, asDWORD flags)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->RegisterObjectType(obj, byteSize, flags);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterObjectType() failed." << '\n';
            System::Exit();
	    }
    }

	inline void RegisterObjectProperty(const char *obj, const char *declaration, int byteOffset, int compositeOffset = 0, bool isCompositeIndirect = false)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->RegisterObjectProperty(obj, declaration, byteOffset, compositeOffset, isCompositeIndirect);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterObjectProperty() failed." << '\n';
            System::Exit();
	    }
    }

	inline void RegisterObjectMethod(const char *obj, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary = 0, int compositeOffset = 0, bool isCompositeIndirect = false)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->RegisterObjectMethod(obj, declaration, funcPointer, callConv, auxiliary, compositeOffset, isCompositeIndirect);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterObjectMethod() failed." << '\n';
            System::Exit();
	    }
    }

	inline void RegisterObjectBehaviour(const char *obj, asEBehaviours behaviour, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary = 0, int compositeOffset = 0, bool isCompositeIndirect = false)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->RegisterObjectBehaviour(obj, behaviour, declaration, funcPointer, callConv, auxiliary, compositeOffset, isCompositeIndirect);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterObjectBehaviour() failed." << '\n';
            System::Exit();
	    }
    }

	inline void RegisterGlobalFunction(const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary = 0)
    {
        int result;
        result = Script::CDevice::Instance().GetEngine()->RegisterGlobalFunction(declaration, funcPointer, callConv, auxiliary);
        if( result < 0 ) 
	    {
            System::Error() << "Script engine RegisterGlobalFunction() failed." << '\n';
            System::Exit();
	    }
    }

    inline CDevice& GetScript()
    {
        return CDevice::Instance();
    }
};