#include <unordered_set>
#include <filesystem>
#include <iostream>

#include "../utils/file_operations.h"

#include "Archiver.h"


void Archiver::compressDirectory(const std::string &directoryPath, const std::string &archivePath, const std::string &compressionMethod)
{
    try
    {
        namespace fs = std::filesystem;

        if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
        {
            throw std::invalid_argument("Invalid directory path.");
        }

        std::ofstream archiveFile(archivePath, std::ios::binary);
        if (!archiveFile)
        {
            throw std::ios_base::failure("Failed to create archive file.");
        }

        for (const auto &entry : fs::recursive_directory_iterator(directoryPath))
        {
            if (fs::is_regular_file(entry))
            {
                std::string relativePath = fs::relative(entry.path(), directoryPath).string();
                std::string extension = entry.path().extension().string();

                size_t pathLength = relativePath.size();
                archiveFile.write(reinterpret_cast<const char *>(&pathLength), sizeof(pathLength));
                archiveFile.write(relativePath.c_str(), pathLength);

                if (extension == ".json" || extension == ".txt" || extension == ".html" || extension == ".xml")
                {
                    std::string compressedFilePath = entry.path().string() + ".tmp";

                    if (compressionMethod == "huff") compressor->compressHuffman(entry.path().string(), compressedFilePath);
                    else if (compressionMethod == "lz77") compressor->compressLZ77(entry.path().string(), compressedFilePath);
                    else if (compressionMethod == "lz78") compressor->compressLZ78(entry.path().string(), compressedFilePath);

                    size_t methodLength = compressionMethod.size();
                    archiveFile.write(reinterpret_cast<const char *>(&methodLength), sizeof(methodLength));
                    archiveFile.write(compressionMethod.c_str(), methodLength);

                    std::string compressedData = readFile(compressedFilePath);
                    size_t dataSize = compressedData.size();
                    archiveFile.write(reinterpret_cast<const char *>(&dataSize), sizeof(dataSize));
                    archiveFile.write(compressedData.data(), dataSize);

                    fs::remove(compressedFilePath);
                }
                else
                {
                    std::string fileData = readFile(entry.path().string());
                    size_t methodLength = 0;
                    archiveFile.write(reinterpret_cast<const char *>(&methodLength), sizeof(methodLength)); // No compression method

                    size_t dataSize = fileData.size();
                    archiveFile.write(reinterpret_cast<const char *>(&dataSize), sizeof(dataSize));
                    archiveFile.write(fileData.data(), dataSize);
                }
            }
        }

        std::cout << "Compression successful. Archive created at: " << archivePath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Compression failed: " << e.what() << std::endl;
    }
}

void Archiver::decompressArchive(const std::string &archivePath)
{
    try
    {
        namespace fs = std::filesystem;

        if (!fs::exists(archivePath) || !fs::is_regular_file(archivePath))
        {
            throw std::invalid_argument("Invalid archive path.");
        }

        std::string archiveDirectory = fs::path(archivePath).parent_path().string();
        std::string extractedDirectoryName = fs::path(archivePath).stem().string();
        std::string extractedDirectoryPath = archiveDirectory + "/" + extractedDirectoryName + "###";

        if (!fs::exists(extractedDirectoryPath))
        {
            fs::create_directory(extractedDirectoryPath);
        }

        std::ifstream archiveFile(archivePath, std::ios::binary);
        if (!archiveFile)
        {
            throw std::ios_base::failure("Failed to open archive file.");
        }

        while (archiveFile)
        {
            size_t pathLength;
            archiveFile.read(reinterpret_cast<char *>(&pathLength), sizeof(pathLength));
            if (archiveFile.eof())
                break;

            std::string relativePath(pathLength, '\0');
            archiveFile.read(&relativePath[0], pathLength);

            size_t methodLength;
            archiveFile.read(reinterpret_cast<char *>(&methodLength), sizeof(methodLength));

            std::string extractedFilePath = extractedDirectoryPath + "/" + relativePath;
            fs::create_directories(fs::path(extractedFilePath).parent_path());

            if (methodLength > 0)
            {
                std::string compressionMethod(methodLength, '\0');
                archiveFile.read(&compressionMethod[0], methodLength);

                size_t dataSize;
                archiveFile.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
                std::string compressedData(dataSize, '\0');
                archiveFile.read(&compressedData[0], dataSize);

                std::string tempCompressedFile = extractedFilePath + ".tmp";
                writeFile(tempCompressedFile, compressedData);

                if (compressionMethod == "huff") compressor->decompressHuffman(tempCompressedFile);
                else if (compressionMethod == "lz77") compressor->decompressLZ77(tempCompressedFile);
                else if (compressionMethod == "lz78") compressor->decompressLZ78(tempCompressedFile);

                fs::remove(tempCompressedFile);
            }
            else
            {
                size_t dataSize;
                archiveFile.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
                std::string fileData(dataSize, '\0');
                archiveFile.read(&fileData[0], dataSize);

                writeFile(extractedFilePath, fileData);
            }
        }
        std::cout << "Decompression successful. Files extracted to: " << extractedDirectoryPath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}
