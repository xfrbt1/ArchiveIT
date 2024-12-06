#ifndef COURSE_PROJECT_ARCHIVER_H
#define COURSE_PROJECT_ARCHIVER_H


#include "iostream"
#include "fstream"
#include "filesystem"

#include "Compressor.h"


struct FileMeta {
    std::string relativePath;
    size_t originalSize;
    std::string algorithm;
};


class Archiver {

private:
    Compressor *compressor;

public:
    explicit Archiver(Compressor *comp) : compressor(comp) {}

    void compressDirectory(const std::string &directoryPath, const std::string &archivePath, const std::string &algMethod);
    void decompressArchive(const std::string &archivePath);
};

#endif //COURSE_PROJECT_ARCHIVER_H
