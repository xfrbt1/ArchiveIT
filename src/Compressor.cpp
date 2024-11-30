#include <iostream>
#include <memory>


#include "../utils/file_operations.h"
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
            throw std::ios_base::failure("Failed to open output file.");
        }
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


void Compressor::decompressHuffman(const std::string &file_path, const std::string &output_path)
{
    try
    {
        std::ifstream inputFile(file_path, std::ios::binary);
        if (!inputFile)
        {
            throw std::ios_base::failure("Failed to open input file.");
        }

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
        writeFile(output_path, decompressedData);

        std::cout << "Decompression successful. Decompressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}
