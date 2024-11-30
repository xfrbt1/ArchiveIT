#include "file_operations.h"


#include <fstream>
#include <stdexcept>
#include <string>


std::string readFile(const std::string &filePath)
{
    std::ifstream inputFile(filePath, std::ios::binary | std::ios::ate);
    if (!inputFile)
    {
        throw std::ios_base::failure("Failed to open file: " + filePath);
    }

    std::streamsize fileSize = inputFile.tellg();
    if (fileSize < 0)
    {
        throw std::ios_base::failure("Failed to read file size: " + filePath);
    }

    inputFile.seekg(0, std::ios::beg);
    std::string buffer(fileSize, '\0');
    if (!inputFile.read(buffer.data(), fileSize))
    {
        throw std::ios_base::failure("Failed to read file data: " + filePath);
    }

    return buffer;
}


void writeFile(const std::string &outputPath, const std::string &data)
{
    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile)
    {
        throw std::ios_base::failure("Failed to open file for writing: " + outputPath);
    }
    if (!outputFile.write(data.data(), data.size()))
    {
        throw std::ios_base::failure("Failed to write data to file: " + outputPath);
    }
}


std::string changeExtension(const std::string &path, const std::string &newExtension)
{
    std::filesystem::path pathObj(path);
    pathObj.replace_extension(newExtension);
    return pathObj.string();
}
