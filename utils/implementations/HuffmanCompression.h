#ifndef COURSE_PROJECT_HUFFMANCOMPRESSION_H
#define COURSE_PROJECT_HUFFMANCOMPRESSION_H

#include "iostream"
#include "../utils/structures/HuffmanNode.h"


std::unordered_map<char, size_t> buildFrequencyTable(const std::string &inputData)
{
    std::unordered_map<char, size_t> frequencyTable;
    for (char c : inputData)
    {
        frequencyTable[c]++;
    }
    return frequencyTable;
}


// Строим дерево Хаффмана
std::shared_ptr<HuffmanNode> buildHuffmanTree(const std::unordered_map<char, size_t> &frequencyTable)
{
    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, CompareNode> minHeap;

    for (const auto &entry : frequencyTable)
    {
        minHeap.push(std::make_shared<HuffmanNode>(entry.first, entry.second));
    }

    while (minHeap.size() > 1)
    {
        std::shared_ptr<HuffmanNode> left = minHeap.top();
        minHeap.pop();
        std::shared_ptr<HuffmanNode> right = minHeap.top();
        minHeap.pop();

        std::shared_ptr<HuffmanNode> newNode = std::make_shared<HuffmanNode>('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    return minHeap.top();
}

// Рекурсивная генерация кодов Хаффмана
void generateHuffmanCodesHelper(std::shared_ptr<HuffmanNode> root, const std::string &currentCode,
                                std::unordered_map<char, std::string> &huffmanCodes)
{
    if (!root)
        return;

    if (root->data != '\0')
    {
        huffmanCodes[root->data] = currentCode;
    }

    generateHuffmanCodesHelper(root->left, currentCode + "0", huffmanCodes);
    generateHuffmanCodesHelper(root->right, currentCode + "1", huffmanCodes);
}

// Генерация таблицы кодов Хаффмана
std::unordered_map<char, std::string> generateHuffmanCodes(std::shared_ptr<HuffmanNode> root)
{
    std::unordered_map<char, std::string> huffmanCodes;
    generateHuffmanCodesHelper(root, "", huffmanCodes);
    return huffmanCodes;
}

// Строим строку с сжатыми данными на основе кодов Хаффмана
std::string compressData(const std::string &inputData, const std::unordered_map<char, std::string> &huffmanCodes)
{
    std::string bitStream;
    for (char c : inputData)
    {
        bitStream += huffmanCodes.at(c);
    }
    return bitStream;
}

// Запись битового потока в файл
void writeBits(std::ofstream &outputFile, const std::string &bitStream)
{
    uint8_t buffer = 0;
    int count = 0;

    for (char bit : bitStream)
    {
        buffer = (buffer << 1) | (bit - '0');
        count++;
        if (count == 8)
        {
            outputFile.put(buffer);
            buffer = 0;
            count = 0;
        }
    }
    if (count > 0)
    {
        buffer <<= (8 - count); // Дополняем до 8 бит
        outputFile.put(buffer);
    }
}

// Сериализация таблицы частот
void serializeFrequencyTable(std::ofstream &outputFile, const std::unordered_map<char, size_t> &frequencyTable)
{
    size_t tableSize = frequencyTable.size();
    outputFile.write(reinterpret_cast<const char*>(&tableSize), sizeof(tableSize));

    for (const auto &pair : frequencyTable)
    {
        outputFile.put(pair.first);
        outputFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }
}

// Сериализация дерева Хаффмана
void serializeTree(std::ofstream &outputFile, std::shared_ptr<HuffmanNode> root)
{
    if (!root) return;
    if (root->data != '\0') {
        outputFile.put('1');
        outputFile.put(root->data);
    } else {
        outputFile.put('0');
    }
    serializeTree(outputFile, root->left);
    serializeTree(outputFile, root->right);
}

// Десериализация таблицы частот
std::unordered_map<char, size_t> deserializeFrequencyTable(std::ifstream &inputFile)
{
    std::unordered_map<char, size_t> frequencyTable;

    size_t tableSize;
    inputFile.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

    for (size_t i = 0; i < tableSize; ++i)
    {
        char symbol;
        inputFile.get(symbol);

        size_t frequency;
        inputFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));

        frequencyTable[symbol] = frequency;
    }

    return frequencyTable;
}

// Восстановление дерева Хаффмана
std::shared_ptr<HuffmanNode> deserializeTree(std::ifstream &inputFile)
{
    char type;
    inputFile.get(type);

    if (type == '1') {
        char symbol;
        inputFile.get(symbol);
        return std::make_shared<HuffmanNode>(symbol, 0);
    } else if (type == '0') {
        auto left = deserializeTree(inputFile);
        auto right = deserializeTree(inputFile);
        auto node = std::make_shared<HuffmanNode>('\0', 0);
        node->left = left;
        node->right = right;
        return node;
    }
    return nullptr;
}


// Декомпрессия данных
std::string decompressData(const std::string &compressedData, std::shared_ptr<HuffmanNode> huffmanTree)
{
    std::string decompressedData;
    std::shared_ptr<HuffmanNode> currentNode = huffmanTree;

    for (char bit : compressedData)
    {
        if (bit == '0')
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;

        if (!currentNode->left && !currentNode->right)
        {
            decompressedData += currentNode->data;
            currentNode = huffmanTree;
        }
    }

    return decompressedData;
}


#endif //COURSE_PROJECT_HUFFMANCOMPRESSION_H
