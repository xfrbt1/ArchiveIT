#ifndef COURSE_PROJECT_FILEMANAGER_H
#define COURSE_PROJECT_FILEMANAGER_H


#include "iostream"
#include "fstream"
#include "filesystem"


class FileManager {

private:

    std::string currentPath;

public:

    void setPath(const std::string &newPath);

    void getInfo() const;

    void remove();

};

void FileManager::setPath(const std::string &newPath)
{

}

void FileManager::getInfo() const
{

}

void FileManager::remove() {
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
    catch (const std::exception &ex)
    {
        std::cerr << "remove error: " << ex.what() << std::endl;
    }
}


#endif //COURSE_PROJECT_FILEMANAGER_H
