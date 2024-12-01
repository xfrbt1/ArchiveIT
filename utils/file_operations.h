#ifndef COURSE_PROJECT_FILE_OPERATIONS_H
#define COURSE_PROJECT_FILE_OPERATIONS_H


#include <string>


std::string readFile(const std::string &filePath);


void writeFile(const std::string &outputPath, const std::string &data);


std::string getExtension(const std::string &path);


std::string changeExtension(const std::string &filePath, const std::string &newExtension);


#endif //COURSE_PROJECT_FILE_OPERATIONS_H
