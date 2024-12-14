#ifndef COURSE_PROJECT_IOPAIR_H
#define COURSE_PROJECT_IOPAIR_H

#include "string"

class IoPair {
public:
    std::string input;
    std::string output;
    IoPair(std::string &in, std::string &out) : input(in), output(out) {}
};


#endif //COURSE_PROJECT_IOPAIR_H
