#include "../include/FileManager.h"


int FileManager::setPath(std::string &newPath)
{
    if (std::filesystem::exists(newPath))
    {
        currentPath = newPath;
        return 1;
    }
    return 0;
}


int FileManager::setPathObjectType()
{
    if (std::filesystem::is_regular_file(currentPath))
    {
        isRegular = true;
        return 1;
    }
    if (std::filesystem::is_directory(currentPath))
    {
        isRegular = false;
        return 1;
    }
    return 0;
}


int FileManager::setSize()
{
    if (isRegular)
    {
        size = std::filesystem::file_size(currentPath);
    }
    else
    {
        uintmax_t totalSize = 0;
        if (std::filesystem::exists(currentPath) && std::filesystem::is_directory(currentPath))
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPath))
            {
                if (std::filesystem::is_regular_file(entry.path()))
                {
                    totalSize += std::filesystem::file_size(entry.path());
                }
            }
        }
        size = totalSize;
    }
    return 1;
}


int FileManager::setLastModified()
{
    auto f_updated_time = std::filesystem::last_write_time(currentPath);
    auto sys_updated_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>
            (
                    f_updated_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
            );
    lastModified = time_to_string(sys_updated_time);
    return 1;
}


void FileManager::setInfo()
{
    if (currentPath.empty())
    {
        return;
    }
    setPathObjectType();
    setSize();
    setLastModified();
}


void FileManager::getInfo()
{
    std::cout << "path: " << currentPath << std::endl;
    std::cout << "type: " << (isRegular ? "regular" : "directory") << std::endl;
    std::cout << "size: " << size << std::endl;
    std::cout << "lastModified: " << lastModified << std::endl;
}


void FileManager::remove()
{
    if (currentPath.empty())
    {
        return;
    }
    try
    {
        auto path = std::filesystem::path(currentPath);

        if (std::filesystem::exists(path))
        {
            if (std::filesystem::is_directory(path))
            {
                std::filesystem::remove_all(path);
            }
            else
            {
                std::filesystem::remove(path);
            }
        }
        else
        {
            return;
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cout << "remove error: " << e.what() << std::endl;
    }
}