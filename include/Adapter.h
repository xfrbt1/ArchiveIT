#ifndef COURSE_PROJECT_ADAPTER_H
#define COURSE_PROJECT_ADAPTER_H


#include "iostream"
#include "FileManager.h"
#include "Benchmark.h"
#include "Compressor.h"
#include "Archiver.h"
#include "Logger.h"


class Adapter {

private:
    Compressor * compressor;
    Archiver * archiver;
    FileManager * fileManager;
    Benchmark * benchmark;
    Logger * logger = nullptr;

public:
    Adapter (FileManager *fm, Archiver *arch, Compressor *comp, Benchmark *bm): fileManager(fm), archiver(arch), compressor(comp), benchmark(bm)
    {
    }

    std::string getStat();
    std::string getCompressedPath();
    void updatePath(std::string &path);
    void setCompressMethod(const std::string &compressMethod);
    void compress(const std::string &i, const std::string &o, const std::string &method);
    void createArchive();
    void unpackArchive();
    void remove();
};

#endif //COURSE_PROJECT_ADAPTER_H
