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


std::string getExtension(const std::string &path)
{
    size_t dotPos = path.rfind('.');
    std::string originalExtension = (dotPos != std::string::npos) ? path.substr(dotPos) : "";
    return originalExtension;
}


std::string changeExtension(const std::string &path, const std::string &newExtension)
{
    std::filesystem::path pathObj(path);
    pathObj.replace_extension(newExtension);
    return pathObj.string();
}


std::string generateOutputFilePath(const std::string &inputFilePath, const std::string &suffix, const std::string &newExtension = "")
{
    size_t lastSlashPos = inputFilePath.find_last_of("/\\");
    std::string directory = (lastSlashPos != std::string::npos) ? inputFilePath.substr(0, lastSlashPos + 1) : "";
    std::string baseName = (lastSlashPos != std::string::npos) ? inputFilePath.substr(lastSlashPos + 1) : inputFilePath;
    size_t dotPos = baseName.find_last_of('.');
    std::string outputBaseName = (dotPos != std::string::npos) ? baseName.substr(0, dotPos) : baseName;
    std::string extension = (dotPos != std::string::npos) ? baseName.substr(dotPos) : "";
    return directory + outputBaseName + suffix + (newExtension.empty() ? extension : newExtension);
}


uintmax_t getFileSize(std::string &filePath)
{
    uintmax_t size = 0;
    if (std::filesystem::is_regular_file(filePath))
    {
        size = std::filesystem::file_size(filePath);
    }
    else
    {
        if (std::filesystem::exists(filePath) && std::filesystem::is_directory(filePath))
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(filePath))
            {
                if (std::filesystem::is_regular_file(entry.path()))
                {
                    size += std::filesystem::file_size(entry.path());
                }
            }
        }
    }
    return size;
}
