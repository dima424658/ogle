//
//  clock.hpp
//
//  Created on: 12 Sep. 2019
//      Author: Dmitry Pankov
//

#pragma once

#include <chrono>
#include <thread>

class Clock
{
public:
    Clock(std::chrono::milliseconds inDelay = std::chrono::milliseconds(60));
    ~Clock();
    Clock(const Clock&) = delete;

    void Sync() noexcept;
    void SetDelay(const std::chrono::milliseconds inDelay) noexcept;
    float GetDelta() noexcept;

private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_time;
    std::chrono::microseconds m_delta;
    std::chrono::milliseconds m_delay;
};