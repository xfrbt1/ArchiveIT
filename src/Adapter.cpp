#include "../utils/file_operations.h"


#include "Adapter.h"


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
    if (fileManager->getIsRegular())
    {
        if (fileManager->getFileExtension() == ".huff")
        {
            compressor->decompressHuffman(current_path);
        }
//        if (fileManager->getFileExtension() == ".lz77")
//        {
//            compressor->decompressLZ77(current_path);
//        }
//        if (fileManager->getFileExtension() == ".lz78")
//        {
//            compressor->decompressLZ78(current_path);
//        }
    }
}


