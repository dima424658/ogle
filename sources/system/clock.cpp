//
//  clock.hpp
//
//  Created on: 12 Sep. 2019
//      Author: Dmitry Pankov
//

#include "system/clock.hpp"

Clock::Clock(std::chrono::milliseconds inDelay)
{
    m_startTime = std::chrono::high_resolution_clock::now();
    m_delay = inDelay;
    m_delta = std::chrono::milliseconds(0);
    m_time = std::chrono::high_resolution_clock::now();
}

Clock::~Clock()
{
}

void Clock::Sync() noexcept
{
    std::this_thread::sleep_until(m_time + m_delay);
    m_delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_time);
    m_time = std::chrono::high_resolution_clock::now();

    return;
}

void Clock::SetDelay(const std::chrono::milliseconds inDelay) noexcept
{
    m_delay = inDelay;
    
    return;
}

float Clock::GetDelta() noexcept
{
    return static_cast<float>(m_delta.count()) / 1000;
}