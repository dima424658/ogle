#pragma once

#include <string>
#include <filesystem>

namespace Platform
{
    std::string file_open(std::string_view rootFolder = std::filesystem::current_path().string());
    std::string file_save(std::string_view rootFolder = std::filesystem::current_path().string());
}; // namespace Platform
