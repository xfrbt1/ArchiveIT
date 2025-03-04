#include <iostream>
#include <memory>


#include "../utils/file_operations.h"
#include "./implementations/HuffmanCompression.h"


#include "Compressor.h"


#define BUFFER_SIZE 256
#define WINDOW_SIZE 2048


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

        std::string originalExtension = getExtension(file_path);
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
        std::string outputFilePath = generateOutputFilePath(file_path, "_decompressed_huff", originalExtension);
        writeFile(outputFilePath, decompressedData);
        std::cout << "Decompression successful. Decompressed file written to " << outputFilePath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}


void Compressor::compressLZ77(const std::string &file_path, const std::string &output_path) {
    try
    {
        std::string originalExtension = getExtension(file_path);
        std::string inputData = readFile(file_path);

        std::vector<std::tuple<size_t, size_t, char>> compressedData;

        const size_t windowSize = WINDOW_SIZE;
        const size_t lookAheadBufferSize = BUFFER_SIZE;

        size_t i = 0;
        while (i < inputData.size())
        {
            size_t matchLength = 0;
            size_t matchDistance = 0;
            size_t start = (i >= windowSize) ? (i - windowSize) : 0;

            for (size_t j = start; j < i; ++j)
            {
                size_t length = 0;
                while (length < lookAheadBufferSize && i + length < inputData.size() && inputData[j + length] == inputData[i + length])
                {
                    ++length;
                }

                if (length > matchLength)
                {
                    matchLength = length;
                    matchDistance = i - j;
                }
            }

            char nextChar = (i + matchLength < inputData.size()) ? inputData[i + matchLength] : '\0';
            compressedData.push_back(std::make_tuple(matchDistance, matchLength, nextChar));
            i += matchLength + 1;
        }

        std::ofstream outputFile(output_path, std::ios::binary);
        if (!outputFile)
        {
            throw std::ios_base::failure("Failed to open output file.");
        }

        size_t extSize = originalExtension.size();
        outputFile.write(reinterpret_cast<const char*>(&extSize), sizeof(extSize));
        outputFile.write(originalExtension.data(), extSize);

        for (const auto &entry : compressedData)
        {
            size_t distance, length;
            char literal;
            std::tie(distance, length, literal) = entry;

            outputFile.write(reinterpret_cast<const char*>(&distance), sizeof(distance));
            outputFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
            outputFile.write(&literal, sizeof(literal));
        }

        std::cout << "Compression successful. Compressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Compression failed: " << e.what() << std::endl;
    }
}


void Compressor::decompressLZ77(const std::string &file_path) {
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

        std::string decompressedData;
        while (inputFile)
        {
            size_t distance = 0, length = 0;
            char literal;

            inputFile.read(reinterpret_cast<char*>(&distance), sizeof(distance));
            inputFile.read(reinterpret_cast<char*>(&length), sizeof(length));

            if (inputFile.eof()) break;

            inputFile.read(&literal, sizeof(literal));

            if (distance > 0 && length > 0) {
                size_t start = decompressedData.size() - distance;
                for (size_t i = 0; i < length; ++i) {
                    decompressedData += decompressedData[start + i];
                }
            }
            decompressedData += literal;
        }

        std::string outputFilePath = generateOutputFilePath(file_path, "_decompressed_lz77", originalExtension);
        writeFile(outputFilePath, decompressedData);

        std::cout << "Decompression successful. Decompressed file written to " << outputFilePath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}


void Compressor::compressLZ78(const std::string &file_path, const std::string &output_path)
{
    try
    {
        std::string inputData = readFile(file_path);

        std::unordered_map<std::string, size_t> dictionary;
        size_t dictionaryIndex = 1;
        std::vector<std::pair<size_t, char>> compressedData;
        std::string currentString;

        for (char c : inputData)
        {
            currentString += c;

            if (dictionary.find(currentString) == dictionary.end())
            {
                size_t prefixIndex = 0;
                if (currentString.size() > 1)
                {
                    prefixIndex = dictionary[currentString.substr(0, currentString.size() - 1)];
                }

                compressedData.emplace_back(prefixIndex, c);

                dictionary[currentString] = dictionaryIndex++;

                currentString.clear();
            }
        }

        if (!currentString.empty())
        {
            size_t prefixIndex = dictionary[currentString.substr(0, currentString.size() - 1)];
            compressedData.emplace_back(prefixIndex, currentString.back());
        }


        std::ofstream outputFile(output_path, std::ios::binary);
        if (!outputFile)
        {
            throw std::ios_base::failure("Failed to open output file.");
        }

        std::string originalExtension = getExtension(file_path);
        size_t extSize = originalExtension.size();
        outputFile.write(reinterpret_cast<const char *>(&extSize), sizeof(extSize));
        outputFile.write(originalExtension.data(), extSize);

        for (const auto &[prefixIndex, symbol] : compressedData)
        {
            outputFile.write(reinterpret_cast<const char *>(&prefixIndex), sizeof(prefixIndex));
            outputFile.write(&symbol, sizeof(symbol));
        }

        std::cout << "Compression successful. Compressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Compression failed: " << e.what() << std::endl;
    }
}


void Compressor::decompressLZ78(const std::string &file_path)
{
    try
    {
        std::ifstream inputFile(file_path, std::ios::binary);
        if (!inputFile)
        {
            throw std::ios_base::failure("Failed to open input file.");
        }

        size_t extSize;
        inputFile.read(reinterpret_cast<char *>(&extSize), sizeof(extSize));
        std::string originalExtension(extSize, '\0');
        inputFile.read(&originalExtension[0], extSize);

        std::vector<std::string> dictionary;
        std::string decompressedData;

        while (inputFile)
        {
            size_t prefixIndex;
            char symbol;

            inputFile.read(reinterpret_cast<char *>(&prefixIndex), sizeof(prefixIndex));
            if (inputFile.eof()) break;
            inputFile.read(&symbol, sizeof(symbol));
            std::string newEntry = (prefixIndex > 0) ? dictionary[prefixIndex - 1] + symbol : std::string(1, symbol);
            decompressedData += newEntry;
            dictionary.push_back(newEntry);
        }

        std::string outputFilePath = generateOutputFilePath(file_path, "_decompressed_lz78", originalExtension);
        writeFile(outputFilePath, decompressedData);
        std::cout << "Decompression successful. Decompressed file written to " << outputFilePath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Decompression failed: " << e.what() << std::endl;
    }
}
