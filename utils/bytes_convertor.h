#ifndef COURSE_PROJECT_BYTES_CONVERTOR_H
#define COURSE_PROJECT_BYTES_CONVERTOR_H


#include "string"


// KB - kilobytes - 1024
// MB - megabytes - 1024 * 1024
// GB - gigabytes - 1024 * 1024 * 1024
// TB - terabytes - 1024 * 1024 * 1024 * 1024
std::string bytes_to_str(uintmax_t n);


#endif //COURSE_PROJECT_BYTES_CONVERTOR_H