#pragma once

#include <cstdio>
#include <array>

struct CVector3
{
public:

    CVector3(float, float, float);
    ~CVector3();

    operator float*()
    {
        return data.data();
    }

private:
    std::array<float, 3> data;
};