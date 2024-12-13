#ifndef COURSE_PROJECT_BENCHMARK_H
#define COURSE_PROJECT_BENCHMARK_H

#include "implementations/IoPair.h"
#include "../utils/containers/Stack.cpp"


class Benchmark {

private:
    Stack<IoPair> IoStack;

public:
    void setValues(std::string input, std::string output);
    void removeValues();
    std::string getStat();
};

#endif //COURSE_PROJECT_BENCHMARK_H
