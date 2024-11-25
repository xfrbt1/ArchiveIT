#ifndef COURSE_PROJECT_FILEMANAGER_H
#define COURSE_PROJECT_FILEMANAGER_H


#include "iostream"
#include "fstream"
#include "filesystem"

#include "../utils/time_to_string.h"
#include "../utils/bytes_convertor.h"


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
    void getInfo();
    void remove();

};


#endif //COURSE_PROJECT_FILEMANAGER_H
