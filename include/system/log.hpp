#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

#include <system/timer.hpp>

namespace System
{
    class CLog
    {
    private:
        CLog();
        CLog(const CLog&) = delete;  
        CLog& operator=(CLog&) = delete;

        static CLog& Instance();

        template<typename T>
        void SendToStreams(T value);

    public:
        enum class SType
        {
            Warning, Error, Log
        };

        using message_t = std::tuple<SType, float, std::string>;
        ~CLog();

        static CLog& Log();
        static CLog& Error();
        static CLog& Warning();
        static const std::vector<message_t>& GetMessages();
        static void ClearMessages();

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
        std::vector<message_t> m_messages;
    };

    System::CLog& Log();
    System::CLog& Error();
    System::CLog& Warning();

    void Exit();
};