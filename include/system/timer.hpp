//
//  clock.hpp
//
//  Created on: 12 Sep. 2019
//      Author: Dmitry Pankov
//

#pragma once

#include <chrono>
#include <thread>

namespace System
{
    class CTimer
    {
    private:
        CTimer(std::chrono::milliseconds inDelay = std::chrono::milliseconds(60));
        CTimer(const CTimer&) = delete;
        CTimer& operator=(CTimer&) = delete;

    public:
        ~CTimer();

        void Sync() noexcept;
        void SetDelay(std::chrono::milliseconds inDelay) noexcept;
        void SetDelay(int inDelay) noexcept;
        float GetDelta() noexcept;

        static CTimer& Instance();

    private:
        std::chrono::high_resolution_clock::time_point m_startTime;
        std::chrono::high_resolution_clock::time_point m_time;
        std::chrono::microseconds m_delta;
        std::chrono::milliseconds m_delay;
    };
}