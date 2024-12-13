#include "Benchmark.h"


void Benchmark::setValues(std::string input, std::string output)
{
    IoPair pair{std::move(input), std::move(output)};
    IoStack.push(std::move(pair));
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
        return "Stack is empty.";
    }

    std::string stat = "Stack contains:\n";
    size_t count = 0;

    Stack<IoPair> tempStack = IoStack;

    while (!tempStack.empty())
    {
        IoPair &topPair = tempStack.top();
        stat += "Input: " + topPair.input + ", Output: " + topPair.output + "\n";
        tempStack.pop();
        ++count;
    }

    stat += "Total items: " + std::to_string(count);
    return stat;
}
