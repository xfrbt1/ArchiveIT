#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <memory>

struct HuffmanNode
{
    char data;
    size_t frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char data, size_t frequency)
            : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};


struct CompareNode
{
    bool operator()(const std::shared_ptr<HuffmanNode> &a, const std::shared_ptr<HuffmanNode> &b)
    {
        return a->frequency > b->frequency;
    }
};
