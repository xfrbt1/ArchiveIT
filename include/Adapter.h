#ifndef COURSE_PROJECT_ADAPTER_H
#define COURSE_PROJECT_ADAPTER_H


#include "iostream"
#include "Compressor.h"
#include "FileManager.h"


class Adapter {

private:
    Compressor * compressor;
    FileManager * fileManager;


public:
//    Adapter(Compressor *comp, FileManager *fm)
//            : compressor(comp), fileManager(fm) {}
//
//
//    void setPath(const std::string &path)
//    {
//        fileManager->setPath(path);
//    }
//
//
//    void compress()
//    {
//        std::string filePath = fileManager->getPath(); // Получаем путь
//        if (fileManager->isFile(filePath))
//        {
//            compressor->compressFile(filePath); // Сжимаем файл
//        }
//        else if (fileManager->isDirectory(filePath))
//        {
//            compressor->compressDirectory(filePath); // Сжимаем каталог
//        }
//    }
//
//
//    void decompress()
//    {
//        std::string filePath = fileManager->getPath();
//        if (fileManager->isCompressedFile(filePath))
//        {
//            compressor->decompressFile(filePath);
//        }
//        else
//        {
//            std::cerr << "Ошибка: Невозможно распаковать некорректный файл.\n";
//        }
//    }
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
//

};

#endif //COURSE_PROJECT_ADAPTER_H
