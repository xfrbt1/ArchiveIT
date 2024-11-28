#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../utils/file_op.h"
#include "../utils/implementations/HuffmanCompression.h"

#include "Compressor.h"


void Compressor::compressHuffman(const std::string &file_path, const std::string &output_path)
{
    try
    {
        std::string inputData = readFile(file_path);
        auto frequencyTable = buildFrequencyTable(inputData);
        auto huffmanTree = buildHuffmanTree(frequencyTable);
        auto huffmanCodes = generateHuffmanCodes(huffmanTree);
        std::string compressedData = compressData(inputData, huffmanCodes);
        std::ofstream outputFile(output_path, std::ios::binary);
        if (!outputFile)
        {
            throw std::ios_base::failure("Failed to open output file: " + output_path);
        }
        serializeFrequencyTable(outputFile, frequencyTable);
        outputFile << compressedData;
        std::cout << "Compression successful. Compressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Compression failed: " << e.what() << std::endl;
    }
}

void Compressor::decompressHuffman(const std::string &file_path, const std::string &output_path)
{
    try
    {
        std::ifstream inputFile(file_path, std::ios::binary);
        if (!inputFile)
        {
            throw std::ios_base::failure("Failed to open file for decompression: " + file_path);
        }
        auto frequencyTable = deserializeFrequencyTable(inputFile);
        auto huffmanTree = rebuildHuffmanTree(frequencyTable);
        std::string compressedData;
        inputFile >> compressedData;
        std::string decompressedData = decompressData(compressedData, huffmanTree);
        writeFile(output_path, decompressedData);
        std::cout << "Decompression successful. Decompressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}
