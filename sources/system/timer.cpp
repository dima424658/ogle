//
//  clock.hpp
//
//  Created on: 12 Sep. 2019
//      Author: Dmitry Pankov
//

#include "system/timer.hpp"

using namespace System;

CTimer::CTimer(std::chrono::milliseconds inDelay)
{
    m_startTime = std::chrono::high_resolution_clock::now();
    m_delay = inDelay;
    m_delta = std::chrono::milliseconds(0);
    m_time = std::chrono::high_resolution_clock::now();
}

CTimer::~CTimer()
{
}

void CTimer::Sync() noexcept
{
    if(m_delay.count() != 0)
        std::this_thread::sleep_until(m_time + m_delay);

    m_delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_time);
    m_time = std::chrono::high_resolution_clock::now();

    return;
}

void CTimer::SetDelay(std::chrono::milliseconds inDelay) noexcept
{
    m_delay = std::move(inDelay);
    
    return;
}

void CTimer::SetDelay(int inDelay) noexcept
{
    m_delay = std::chrono::milliseconds(inDelay);

    return;
}

float CTimer::GetDelta() noexcept
{
    return static_cast<float>(m_delta.count()) / 1000;
}

float CTimer::GetTime() noexcept
{
    return static_cast<float>((std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_startTime)).count()) / 1000;
}

CTimer& CTimer::Instance()
{
    static CTimer instance;
    return instance;
}
