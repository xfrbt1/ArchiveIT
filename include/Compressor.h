#ifndef COURSE_PROJECT_COMPRESSOR_H
#define COURSE_PROJECT_COMPRESSOR_H

#define compressor_params const std::string &file_path, const std::string &output_path

class Compressor {

public:
    Compressor() = default;
    ~Compressor() = default;

    void compressHuffman(compressor_params);
    void compressLZ77(const std::string &file_path, const std::string &output_path);
    void compressLZ78(const std::string &file_path, const std::string &output_path);

    void decompressHuffman(const std::string &file_path, const std::string &output_path);
    void decompressLZ77(const std::string &file_path, const std::string &output_path);
    void decompressLZ78(const std::string &file_path, const std::string &output_path);
};

#endif //COURSE_PROJECT_COMPRESSOR_H
