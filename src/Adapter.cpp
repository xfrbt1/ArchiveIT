#include "../include/Adapter.h"
#include "../utils/file_operations.h"


void Adapter::updatePath(std::string &path)
{
    fileManager->setPath(path);
    fileManager->setInfo();
    fileManager->getInfo();
}


void Adapter::remove()
{
    fileManager->remove();
}


void Adapter::createArchive()
{
    std::string current_path = fileManager->getPath();
    if (fileManager->getIsRegular())
    {
        compressor->compressHuffman(current_path, changeExtension(current_path, ".huff"));
    }
}


void Adapter::unpackArchive()
{
    std::string current_path = fileManager->getPath();
    compressor->decompressHuffman(current_path, "");
}


