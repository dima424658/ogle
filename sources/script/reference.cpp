#include "script/reference.hpp"

using namespace Script;

iReference::iReference()
{
    ref_count = 1;
}

void iReference::AddRef()
{
    ref_count++;
}

void iReference::Release()
{
    if(!--ref_count)
        delete this;
}