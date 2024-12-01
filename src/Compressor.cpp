#include <iostream>
#include <memory>


#include "../utils/file_operations.h"
#include "./implementations/HuffmanCompression.h"


#include "Compressor.h"


void Compressor::compressHuffman(const std::string &file_path, const std::string &output_path)
{
    try
    {
        size_t dotPos = file_path.rfind('.');
        std::string originalExtension = (dotPos != std::string::npos) ? file_path.substr(dotPos) : "";

        std::string inputData = readFile(file_path);
        auto frequencyTable = buildFrequencyTable(inputData);
        auto huffmanTree = buildHuffmanTree(frequencyTable);
        auto huffmanCodes = generateHuffmanCodes(huffmanTree);
        std::string compressedData = compressData(inputData, huffmanCodes);

        std::ofstream outputFile(output_path, std::ios::binary);
        if (!outputFile)
        {
            throw std::ios_base::failure("Failed to open output file.");
        }
        size_t extSize = originalExtension.size();
        outputFile.write(reinterpret_cast<const char*>(&extSize), sizeof(extSize));
        outputFile.write(originalExtension.data(), extSize);

        serializeFrequencyTable(outputFile, frequencyTable);
        serializeTree(outputFile, huffmanTree);
        writeBits(outputFile, compressedData);
        std::cout << "Compression successful. Compressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Compression failed: " << e.what() << std::endl;
    }
}


void Compressor::decompressHuffman(const std::string &file_path)
{
    try
    {
        std::ifstream inputFile(file_path, std::ios::binary);
        if (!inputFile)
        {
            throw std::ios_base::failure("Failed to open input file.");
        }

        size_t extSize;
        inputFile.read(reinterpret_cast<char*>(&extSize), sizeof(extSize));
        std::string originalExtension(extSize, '\0');
        inputFile.read(&originalExtension[0], extSize);

        auto frequencyTable = deserializeFrequencyTable(inputFile);
        auto huffmanTree = deserializeTree(inputFile);

        std::string compressedData;
        char byte;
        while (inputFile.get(byte))
        {
            for (int i = 7; i >= 0; --i)
            {
                compressedData += ((byte >> i) & 1) ? '1' : '0';
            }
        }

        std::string decompressedData = decompressData(compressedData, huffmanTree);

        size_t lastSlashPos = file_path.find_last_of("/\\");
        std::string directory = (lastSlashPos != std::string::npos) ? file_path.substr(0, lastSlashPos + 1) : "";
        std::string baseName = (lastSlashPos != std::string::npos) ? file_path.substr(lastSlashPos + 1) : file_path;

        size_t dotPos = baseName.find_last_of('.');
        std::string outputBaseName = (dotPos != std::string::npos) ? baseName.substr(0, dotPos) : baseName;
        std::string outputFilePath = directory + outputBaseName + "_decompressed" + originalExtension;

        writeFile(outputFilePath, decompressedData);

        std::cout << "Decompression successful. Decompressed file written to " << outputFilePath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}

