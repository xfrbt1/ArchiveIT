#ifndef COURSE_PROJECT_LOGGER_H
#define COURSE_PROJECT_LOGGER_H


class Logger {

public:
    void printInfo(const std::string &msg) {std::cout << msg << std::endl;};
    void printError(const std::string &msg) {std::cout << msg << std::endl;};
};

#endif //COURSE_PROJECT_LOGGER_H
