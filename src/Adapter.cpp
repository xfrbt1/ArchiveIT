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


std::string Adapter::getStat()
{
    return benchmark->getPrevStat();
}


std::string Adapter::getLastOPath()
{
    return benchmark->getLastOperationPair().output;
}


void Adapter::compress(const std::string &i, const std::string &o, const std::string &method)
{
    if (method == "huff")
    {
        compressor->compressHuffman(i, o);
    }
    if (method == "lz77")
    {
        compressor->compressLZ77(i, o);
    }
    if (method == "lz78")
    {
        compressor->compressLZ78(i, o);
    }
}


void Adapter::createArchive()
{
    std::string current_path = fileManager->getPath();

    const std::unordered_map<std::string, std::string> method_to_extension =
            {
            {"huff", ".huff"},
            {"lz77", ".lz77"},
            {"lz78", ".lz78"}
            };

    std::string method = compressor->getCompressMethod();

    auto it = method_to_extension.find(method);
    if (it == method_to_extension.end())
    {
        std::cout << "No method choosing!";
        return;
    }

    std::string extension = it->second;
    std::string output_path = changeExtension(current_path, extension);

    try
    {
        if (fileManager->getIsRegular())
        {
            compress(current_path, output_path, method);
        }
        else
        {
            output_path = changeExtension(current_path, ".arc");
            archiver->compressDirectory(current_path, output_path, method);
        }
        benchmark->setValues(current_path, output_path);
    }
    catch (const std::exception &e)
    {
    }

}

void Adapter::unpackArchive()
{
    std::string current_path = fileManager->getPath();
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
