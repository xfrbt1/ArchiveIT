#ifndef COURSE_PROJECT_IOPAIR_H
#define COURSE_PROJECT_IOPAIR_H

#include "string"

class IoPair {

public:
    std::string input;
    std::string output;
    IoPair(std::string in, std::string out) : input(std::move(in)), output(std::move(out)) {}

};

#endif //COURSE_PROJECT_IOPAIR_H
