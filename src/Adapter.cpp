#include "../utils/file_operations.h"

#include "Adapter.h"


void Adapter::setCompressMethod(const std::string &compressionMethod)
{
    compressor->setCompressMethod(compressionMethod);
}


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
        if (compressor->getCompressMethod() == "huff")
        {
            compressor->compressHuffman(current_path, changeExtension(current_path, ".huff"));
        }
        if (compressor->getCompressMethod() == "lz77")
        {
            compressor->compressLZ77(current_path, changeExtension(current_path, ".lz77"));
        }
        if (compressor->getCompressMethod() == "lz78")
        {
            compressor->compressLZ78(current_path, changeExtension(current_path, ".lz78"));
        }
    }
    else
    {
        if (compressor->getCompressMethod() == "huff")
        {
            archiver->compressDirectory(current_path, changeExtension(current_path, ".arc"), "huff");
        }
        if (compressor->getCompressMethod() == "lz77")
        {
            archiver->compressDirectory(current_path, changeExtension(current_path, ".arc"), "lz77");
        }
        if (compressor->getCompressMethod() == "lz78")
        {
            archiver->compressDirectory(current_path, changeExtension(current_path, ".arc"), "lz78");
        }
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
        if (fileManager->getFileExtension() == ".lz77")
        {
            compressor->decompressLZ77(current_path);
        }
        if (fileManager->getFileExtension() == ".lz78")
        {
            compressor->decompressLZ78(current_path);
        }
        if (fileManager->getFileExtension() == ".arc")
        {
            archiver->decompressArchive(current_path);
        }
    }
}


