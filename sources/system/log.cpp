#include "system/log.hpp"

using namespace System;

void System::Exit()
{
    std::exit(0);
}

CLog::CLog()
{
    m_fstream.open("engine.log", std::ofstream::out | std::ofstream::app);
}

CLog::~CLog()
{
    m_fstream.close();
}

CLog& CLog::Instance()
{
    static CLog instance;
    return instance;
}    

CLog& CLog::Log() // TODO сделать вменяемые логи в кои-то веке
{
    CLog::Instance().m_messages.push_back(std::make_tuple(SType::Log, GetTimer().GetTime(), std::string("")));

    CLog::Instance().m_fstream << "\n";
    std::cout << "\n";

    return CLog::Instance();
}

CLog& CLog::Error()
{
    CLog::Instance().m_messages.push_back(std::make_tuple(SType::Error, GetTimer().GetTime(), std::string("")));

    CLog::Instance().m_fstream << "\n";
    std::cout << "\n";
    
    return CLog::Instance();
}

CLog& CLog::Warning()
{
    CLog::Instance().m_messages.push_back(std::make_tuple(SType::Warning, GetTimer().GetTime(), std::string("")));

    CLog::Instance().m_fstream << "\n";
    std::cout << "\n";

    return CLog::Instance();
}

const std::vector<CLog::message_t>& CLog::GetMessages()
{
    return CLog::Instance().m_messages;
}


void CLog::ClearMessages()
{
    CLog::Instance().m_messages.clear();
}

template<typename T>
void CLog::SendToStreams(T value)
{
    m_fstream << value;
    std::cout << value;
    
    if(m_messages.size() > 0)
    {
        auto str = std::get<2>(m_messages[m_messages.size() - 1]);
        str += value;
        std::get<2>(m_messages[m_messages.size() - 1]) = str;
    }

    return;
}

CLog& CLog::operator<<(const std::string& value) noexcept
{
    SendToStreams<std::string>(value);
    return *this;
}

CLog& CLog::operator<< (std::string_view value) noexcept
{
    SendToStreams<std::string_view>(value);
    return *this;
}

CLog& CLog::operator<< (const char* value) noexcept
{
    SendToStreams<const char*>(value);
    return *this;
}

CLog& CLog::operator<< (char value) noexcept
{
    SendToStreams<char>(value);
    return *this;
}

CLog& CLog::operator<< (float value) noexcept
{
    SendToStreams<std::string>(std::to_string(value));
    return *this;
}

CLog& CLog::operator<< (int value) noexcept
{
    SendToStreams<std::string>(std::to_string(value));
    return *this;
}

CLog& CLog::operator<< (unsigned int value) noexcept
{
    SendToStreams<std::string>(std::to_string(value));
    return *this;
}

void CLog::Write(std::string& value) noexcept
{
    SendToStreams<const char*>(value.c_str());
}

System::CLog& System::Log()
{
    return CLog::Log();
}

System::CLog& System::Error()
{
    return CLog::Error();
}

System::CLog& System::Warning()
{
    return CLog::Warning();
}