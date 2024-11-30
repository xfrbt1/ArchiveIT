#ifndef COURSE_PROJECT_ADAPTER_H
#define COURSE_PROJECT_ADAPTER_H


#include "iostream"
#include "FileManager.h"
#include "Compressor.h"
#include "Archivator.h"


class Adapter {

private:
    Compressor * compressor;
    FileManager * fileManager;

public:
    Adapter (FileManager *fm, Compressor *comp): fileManager(fm), compressor(comp)
    {
        //
    }

    ~Adapter();

    void updatePath(std::string &path);
    void setCompressMethod(const std::string &compressMethod);
    void createArchive();
    void unpackArchive();
    void setInfo();
    void setBenchmark();
    void remove();
};

#endif //COURSE_PROJECT_ADAPTER_H
