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

        // Вектор для хранения сжатых данных в формате (дистанция, длина, следующий символ)
        std::vector<std::tuple<size_t, size_t, char>> compressedData;

        const size_t windowSize = WINDOW_SIZE;               // Размер окна для поиска совпадений
        const size_t lookAheadBufferSize = BUFFER_SIZE;      // Размер буфера предварительного просмотра

        size_t i = 0; // Индекс текущего символа для обработки
        while (i < inputData.size())  // Пока есть данные для обработки
        {
            size_t matchLength = 0;  // Длина найденного совпадения
            size_t matchDistance = 0; // Дистанция до найденного совпадения
            size_t start = (i >= windowSize) ? (i - windowSize) : 0; // Начальная позиция для поиска совпадений

            // Цикл для поиска совпадений в предыдущем окне
            for (size_t j = start; j < i; ++j)
            {
                size_t length = 0; // Длина текущего совпадения
                // Исключаем условия для поиска максимального совпадения
                while (length < lookAheadBufferSize && i + length < inputData.size() && inputData[j + length] == inputData[i + length])
                {
                    ++length; // Увеличиваем длину совпадения
                }

                // Если найденное совпадение длиннее предыдущих
                if (length > matchLength)
                {
                    matchLength = length;  // Обновляем длину совпадения
                    matchDistance = i - j; // Обновляем дистанцию до начала совпадения
                }
            }

            // Получаем следующий символ, который не входит в совпадение
            char nextChar = (i + matchLength < inputData.size()) ? inputData[i + matchLength] : '\0';
            // Сохраняем найденные данные (дистанцию, длину и следующий символ)
            compressedData.push_back(std::make_tuple(matchDistance, matchLength, nextChar));
            // Перемещаемся до следующего символа для обработки
            i += matchLength + 1;
        }

        // Открываем выходной файл для записи в бинарном формате
        std::ofstream outputFile(output_path, std::ios::binary);
        if (!outputFile) // Проверяем на успешное открытие файла
        {
            throw std::ios_base::failure("Failed to open output file."); // Генерируем исключение, если не удалось открыть файл
        }

        // Записываем размер строки расширения
        size_t extSize = originalExtension.size();
        outputFile.write(reinterpret_cast<const char*>(&extSize), sizeof(extSize)); // Записываем размер
        outputFile.write(originalExtension.data(), extSize); // Записываем само расширение

        // Записываем сжатые данные
        for (const auto &entry : compressedData)
        {
            size_t distance, length;
            char literal;
            std::tie(distance, length, literal) = entry; // Извлекаем значения из кортежа

            // Записываем дистанцию, длину и символ в выходной файл
            outputFile.write(reinterpret_cast<const char*>(&distance), sizeof(distance));
            outputFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
            outputFile.write(&literal, sizeof(literal));
        }

        // Сообщение об успешной компрессии
        std::cout << "Compression successful. Compressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        // Обработка ошибок: выводим сообщение об ошибке
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
        // Читаем содержимое входного файла в строку
        std::string inputData = readFile(file_path);

        // Словарь для хранения уникальных строк и их индексов
        std::unordered_map<std::string, size_t> dictionary;
        size_t dictionaryIndex = 1; // Индекс для новых записей в словаре
        std::vector<std::pair<size_t, char>> compressedData; // Вектор для хранения сжатых данных (индекс, символ)
        std::string currentString; // Строка для текущего символа или последовательности

        // Проходим по каждому символу входных данных
        for (char c : inputData)
        {
            currentString += c; // Добавляем символ к текущей строке

            // Проверяем, если текущая строка уже существует в словаре
            if (dictionary.find(currentString) == dictionary.end())
            {
                size_t prefixIndex = 0; // Индекс для префикса (предыдущей строки)
                if (currentString.size() > 1) // Если длина текущей строки больше 1
                {
                    // Получаем индекс префикса, который на один символ короче текущей строки
                    prefixIndex = dictionary[currentString.substr(0, currentString.size() - 1)];
                }

                // Сохраняем вектор сжатых данных (индекс префикса и текущий символ)
                compressedData.emplace_back(prefixIndex, c);

                // Добавляем текущую строку в словарь с новым индексом
                dictionary[currentString] = dictionaryIndex++;

                currentString.clear(); // Очищаем текущую строку для продолжения
            }
        }

        // Проверяем, остались ли необработанные символы в текущей строке
        if (!currentString.empty())
        {
            // Получаем индекс префикса для текущей строки
            size_t prefixIndex = dictionary[currentString.substr(0, currentString.size() - 1)];
            compressedData.emplace_back(prefixIndex, currentString.back()); // Добавляем последний символ к сжатым данным
        }

        // Открываем файл для записи сжатых данных в бинарном формате
        std::ofstream outputFile(output_path, std::ios::binary);
        if (!outputFile) // Проверяем на успешное открытие файла
        {
            throw std::ios_base::failure("Failed to open output file."); // Генерируем исключение, если не удалось открыть файл
        }

        // Получаем расширение оригинального файла
        std::string originalExtension = getExtension(file_path);
        size_t extSize = originalExtension.size(); // Размер расширения
        outputFile.write(reinterpret_cast<const char *>(&extSize), sizeof(extSize)); // Записываем размер расширения
        outputFile.write(originalExtension.data(), extSize); // Записываем само расширение

        // Записываем сжатые данные в файл
        for (const auto &[prefixIndex, symbol] : compressedData)
        {
            outputFile.write(reinterpret_cast<const char *>(&prefixIndex), sizeof(prefixIndex)); // Индекс префикса
            outputFile.write(&symbol, sizeof(symbol)); // Текущий символ
        }

        // Сообщение об успешной компрессии
        std::cout << "Compression successful. Compressed file written to " << output_path << std::endl;
    }
    catch (const std::exception &e)
    {
        // Обработка ошибок: выводим сообщение об ошибке
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
