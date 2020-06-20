#pragma once

#include <fstream>
#include <iostream>

namespace System
{
    class CLog
    {
    private:
        CLog();
        CLog(const CLog&) = delete;  
        CLog& operator=(CLog&) = delete;

        template<typename T>
        void SendToStreams(T value);

    public:
        ~CLog();
        static CLog& Instance();

        CLog& operator<< (const char*) noexcept;
        CLog& operator<< (char) noexcept;
        CLog& operator<< (int) noexcept;
        CLog& operator<< (float) noexcept;
        CLog& operator<< (unsigned int) noexcept;
        CLog& operator<< (const std::string&) noexcept;
        CLog& operator<< (std::string_view) noexcept;
        void Write(std::string&) noexcept;

    private:
        std::fstream m_fstream;
    };

    System::CLog& Log();
    void Exit();
};