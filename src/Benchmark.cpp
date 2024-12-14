#include "Benchmark.h"

#include "filesystem"
#include "iostream"

#include "../utils/bytes_convertor.h"
#include "../utils/file_operations.h"


void Benchmark::setValues(std::string &input, std::string &output)
{
    IoPair pair{input, output};
    IoStack.push(pair);
}


void Benchmark::removeValues()
{
    if (IoStack.empty())
    {
        throw std::out_of_range("Empty stack!");
    }
    IoStack.pop();
}


std::string Benchmark::getStat()
{
    if (IoStack.empty())
    {
        std::cout << "stack empty";
        return "";
    }

    std::string stat = "contains:\n";
    size_t count = 0;

    Stack<IoPair> tempStack = IoStack;

    while (!tempStack.empty())
    {
        IoPair &topPair = tempStack.top();
        stat += "Input: " + topPair.input + ", Output: " + topPair.output + "\n";
        tempStack.pop();
        ++count;
    }
    return "";
}


std::string Benchmark::getPrevStat()
{
    std::string stat;
    if (IoStack.empty())
    {
        return stat;
    }

    IoPair topPair = IoStack.top();

    uintmax_t i = getFileSize(topPair.input);
    uintmax_t o = getFileSize(topPair.output);

    float result = static_cast<float>(i) / o;
    std::cout << i << ", " << o << " = " << result << std::endl;
    stat = std::to_string(result);

    return stat;
}


IoPair Benchmark::getLastOperationPair()
{
    if (IoStack.empty())
    {
//        throw std::exception;
    }
    return IoStack.top();
}


void Benchmark::removeOutput()
{
    Stack<IoPair> tempStack = IoStack;
    while (!tempStack.empty())
    {
        if (std::filesystem::is_directory(tempStack.top().output))
        {
            std::filesystem::remove_all(tempStack.top().output);
        }
        else
        {
            std::filesystem::remove(tempStack.top().output);
        }
        tempStack.pop();
    }
}
