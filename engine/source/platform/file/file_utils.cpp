#include "file_utils.hpp"
#include <fstream>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

namespace Airwave
{
bool FileUtils::exists(const std::string &path)
{
    std::ifstream file(path);
    return file.good();
}

size_t FileUtils::fileSize(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return 0;
    return file.tellg();
}

std::string FileUtils::readFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::vector<std::string> FileUtils::listDirectory(const std::string &path)
{
    std::vector<std::string> files;

#ifdef _WIN32
    std::string searchPath = path + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (std::string(fd.cFileName) != "." && std::string(fd.cFileName) != "..") files.push_back(fd.cFileName);
        } while (FindNextFileA(hFind, &fd));
        FindClose(hFind);
    }
#else
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") files.push_back(entry->d_name);
        }
        closedir(dir);
    }
#endif
    return files;
}


bool FileUtils::createFile(const std::string &path)
{
    std::ofstream file(path);
    return file.good();
}

bool FileUtils::deleteFile(const std::string &path)
{
    return std::remove(path.c_str()) == 0;
}

bool FileUtils::createDirectory(const std::string &path)
{
#ifdef _WIN32
    return CreateDirectoryA(path.c_str(), NULL);
#else
    return mkdir(path.c_str(), 0777) == 0;
#endif
}

bool FileUtils::deleteDirectory(const std::string &path)
{
#ifdef _WIN32
    return RemoveDirectoryA(path.c_str());
#else
    return rmdir(path.c_str()) == 0;
#endif
}

std::string FileUtils::getFileExtension(const std::string &path)
{
    size_t pos = path.find_last_of(".");
    return (pos == std::string::npos) ? "" : path.substr(pos + 1);
}

std::string FileUtils::getFileName(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

std::string FileUtils::getParentPath(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    return (pos == std::string::npos) ? "" : path.substr(0, pos);
}

} // namespace Airwave