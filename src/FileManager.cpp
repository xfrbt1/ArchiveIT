#include "FileManager.h"


#include "../utils/time_to_string.h"
#include "../utils/bytes_convertor.h"
#include "../utils/file_operations.h"


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
    size = getFileSize(currentPath);
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
    std::cout << "size: " << bytes_to_str(size) << std::endl;
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


bool FileManager::getIsRegular() {return isRegular;}


std::string FileManager::getLastModified() {return lastModified;}


std::string FileManager::getPath() {return currentPath;}


uintmax_t FileManager::getSize() {return size;}


uintmax_t FileManager::getSize(std::string &filePath) {return getFileSize(filePath);}


std::string FileManager::getFileExtension() {return getExtension(currentPath);}
