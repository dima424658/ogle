#pragma once

#include <script/device.hpp>

#include <glm/detail/setup.hpp>
#undef GLM_HAS_CONSTEXPR

#include <glm/glm.hpp>


namespace Script
{
    void ConstructVec3(glm::vec3* ptr)
    {
        new(ptr) glm::vec3;
    }

    void ConstructVec3Scalar(glm::vec3* ptr, float val)
    {
        new(ptr) glm::vec3(val);
    }

    void ConstructVec3Float(glm::vec3* ptr, float x, float y, float z)
    {
        new(ptr) glm::vec3(x, y, z);
    }

    void ConstructVec3Vec3(glm::vec3* ptr, const glm::vec3& val)
    {
        new(ptr) glm::vec3(val.x, val.y, val.z);
    }

    void Destructor(glm::vec3* ptr)
    {
        //delete ptr;
    }
    
    glm::vec3* AddTwoVec3(const glm::vec3& l, const glm::vec3& r)
    {
        auto res = new glm::vec3(l + r);
        return res;
    }

    glm::vec3* SubTwoVec3(const glm::vec3& l, const glm::vec3& r)
    {
        auto res = new glm::vec3(l - r);
        return res;
    }

    glm::vec3* MulVec3Float(const glm::vec3& l, float r)
    {
        auto res = new glm::vec3(l * r);
        return res;
    }

    glm::vec3* MulFloatVec3(float l, const glm::vec3& r)
    {
        auto res = new glm::vec3(l * r);
        return res;
    }


    glm::vec3* DivVec3Float(const glm::vec3& l, float r)
    {
        auto res = new glm::vec3(l / r);
        return res;
    }
    

    glm::vec3* Cross(const glm::vec3& l, const glm::vec3& r)
    {
        auto res = new glm::vec3(glm::cross(l, r));
        return res;
    }

    void Normalize(glm::vec3& o)
    {
        o = glm::normalize(o);
    }

    void RegisterGLM()
    {
        SetDefaultNamespace("glm");

        RegisterObjectType("vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::vec3>());
      
        RegisterObjectProperty("vec3", "float x", offsetof(glm::vec3, x));
        RegisterObjectProperty("vec3", "float y", offsetof(glm::vec3, y));
        RegisterObjectProperty("vec3", "float z", offsetof(glm::vec3, z));

        RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructVec3), asCALL_CDECL_OBJFIRST);
        RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(ConstructVec3Float), asCALL_CDECL_OBJFIRST);
        RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(ConstructVec3Scalar), asCALL_CDECL_OBJFIRST);
        RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3 &in)", asFUNCTION(ConstructVec3Vec3), asCALL_CDECL_OBJFIRST);

        RegisterObjectBehaviour("vec3", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Destructor), asCALL_CDECL_OBJFIRST);

        RegisterObjectMethod("vec3", "vec3& opAssign(const vec3 &in)", asMETHODPR(glm::vec3, operator=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);

        RegisterObjectMethod("vec3", "vec3& opAddAssign(const vec3 &in)", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "vec3& opSubAssign(const vec3 &in)", asMETHODPR(glm::vec3, operator-=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "vec3& opMulAssign(const vec3 &in)", asMETHODPR(glm::vec3, operator*=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "vec3& opDivAssign(const vec3 &in)", asMETHODPR(glm::vec3, operator/=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);

        RegisterObjectMethod("vec3", "vec3& opAddAssign(float &in)", asMETHODPR(glm::vec3, operator+=, (float&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "vec3& opSubAssign(float &in)", asMETHODPR(glm::vec3, operator-=, (float&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "vec3& opMulAssign(float &in)", asMETHODPR(glm::vec3, operator*=, (float&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "vec3& opDivAssign(float &in)", asMETHODPR(glm::vec3, operator/=, (float&), glm::vec3&), asCALL_THISCALL);
        RegisterObjectMethod("vec3", "void normalize()", asFUNCTIONPR(Normalize, (glm::vec3&), void), asCALL_CDECL_OBJFIRST);
        RegisterObjectMethod("vec3", "float length()", asFUNCTIONPR(glm::length, (const glm::vec3&), float), asCALL_CDECL_OBJFIRST);

	    RegisterObjectMethod("vec3", "bool opEquals(const vec3 &in) const", asFUNCTIONPR(glm::operator==, (const glm::vec3&, const glm::vec3&), bool), asCALL_CDECL_OBJFIRST);
        RegisterGlobalFunction("float dot(const vec3 &in, const vec3 &in)", asFUNCTIONPR(glm::dot, (const glm::vec3&, const glm::vec3&), float), asCALL_CDECL);
        RegisterGlobalFunction("vec3& cross(const vec3 &in, const vec3 &in)", asFUNCTIONPR(Cross, (const glm::vec3&, const glm::vec3&), glm::vec3*), asCALL_CDECL);
        
        RegisterObjectMethod("vec3", "vec3& opAdd(const vec3 &in) const", asFUNCTIONPR(AddTwoVec3, (const glm::vec3&, const glm::vec3&), glm::vec3*), asCALL_CDECL_OBJFIRST);
	    RegisterObjectMethod("vec3", "vec3& opSub(const vec3 &in) const", asFUNCTIONPR(SubTwoVec3, (const glm::vec3&, const glm::vec3&), glm::vec3*), asCALL_CDECL_OBJFIRST);
	    RegisterObjectMethod("vec3", "vec3& opMul(float) const", asFUNCTIONPR(MulVec3Float, (const glm::vec3&, float), glm::vec3*), asCALL_CDECL_OBJFIRST);
	    RegisterObjectMethod("vec3", "vec3& opMul_r(float) const", asFUNCTIONPR(MulFloatVec3, (float, const glm::vec3&), glm::vec3*), asCALL_CDECL_OBJLAST);
	    RegisterObjectMethod("vec3", "vec3& opDiv(float) const", asFUNCTIONPR(DivVec3Float, (const glm::vec3&, float), glm::vec3*), asCALL_CDECL_OBJLAST);

        SetDefaultNamespace("");
    }

}