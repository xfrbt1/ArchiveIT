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

    void createArchive();

    void unpackArchive();

    void updateInfo();

    void setBenchmark();


//
//
//    void remove()
//    {
//        std::string filePath = fileManager->getPath();
//        if (!filePath.empty())
//        {
//            fileManager->remove(filePath);
//        }
//        else
//        {
//            std::cerr << "Ошибка: Путь не установлен.\n";
//        }
//    }
//
//
//    std::string getInfo() const
//    {
//        std::string filePath = fileManager->getPath();
//        return fileManager->getFileInfo(filePath);
//    }


};

#endif //COURSE_PROJECT_ADAPTER_H
