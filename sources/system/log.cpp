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

template<typename T>
void CLog::SendToStreams(T value)
{
    m_fstream << value;
    std::cout << value;
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
    SendToStreams<float>(value);
    return *this;
}

CLog& CLog::operator<< (int value) noexcept
{
    SendToStreams<int>(value);
    return *this;
}

CLog& CLog::operator<< (unsigned int value) noexcept
{
    SendToStreams<unsigned int>(value);
    return *this;
}

void CLog::Write(std::string& value) noexcept
{
    SendToStreams<const char*>(value.c_str());
}

System::CLog& System::Log()
{
    return CLog::Instance();
}