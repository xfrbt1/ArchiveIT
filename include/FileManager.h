#ifndef COURSE_PROJECT_FILEMANAGER_H
#define COURSE_PROJECT_FILEMANAGER_H


#include "iostream"
#include "fstream"
#include "filesystem"


class FileManager {

private:
    std::string currentPath;
    std::string lastModified;
    bool isRegular;
    uintmax_t size;


public:
    int setPath(std::string &newPath);
    int setPathObjectType();
    int setSize();
    int setLastModified();
    void setInfo();

    std::string getPath();
    std::string getLastModified();
    std::string getFileExtension();
    bool getIsRegular();
    uintmax_t getSize();
    uintmax_t getSize(std::string &filePath);
    void getInfo();

    void remove();

};


#endif //COURSE_PROJECT_FILEMANAGER_H
