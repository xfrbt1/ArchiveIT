#ifndef COURSE_PROJECT_ADAPTER_H
#define COURSE_PROJECT_ADAPTER_H


#include "iostream"
#include "Compressor.h"
#include "FileManager.h"
#include "Archivator.h"


class Adapter {

private:
//    Compressor * compressor;
    FileManager * fileManager;
//    Archivator * archivator;
//    InfoManager * infoManager;

public:
    Adapter
            (
//    Compressor *comp,
    FileManager *fm
//    Archivator *arch,
//    InfoManager * im
            )
            :
//    compressor(comp),
    fileManager(fm)
//    archivator(arch),
//    infoManager(im)
            {}

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
