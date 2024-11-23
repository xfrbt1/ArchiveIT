#ifndef COURSE_PROJECT_ADAPTER_H
#define COURSE_PROJECT_ADAPTER_H


#include "iostream"
#include "Compressor.h"
#include "FileManager.h"
#include "Archivator.h"
#include "InfoManager.h"

class Adapter {

private:
    Compressor * compressor;
    FileManager * fileManager;
    Archivator * archivator;
    InfoManager * infoManager;

public:
    Adapter
            (
    Compressor *comp,
    FileManager *fm,
    Archivator *arch,
    InfoManager * im
            )
            :
    compressor(comp),
    fileManager(fm),
    archivator(arch),
    infoManager(im)
            {}

    ~Adapter();

    void setPath(const std::string &path);

    void setCompressMethod(const std::string &compressMethod);

    void createArchive();

    void unpackArchive();

    void updateInfo();

    void setBenchmark();

};

#endif //COURSE_PROJECT_ADAPTER_H
