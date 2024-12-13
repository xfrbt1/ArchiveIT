#ifndef COURSE_PROJECT_COMPRESSOR_H
#define COURSE_PROJECT_COMPRESSOR_H


class Compressor {

private:
    std::string compressionMethod;

public:
    void setCompressMethod(const std::string &compressMethod) {compressionMethod = compressMethod;}
    std::string getCompressMethod() {return compressionMethod;}

    void compressHuffman(const std::string &file_path, const std::string &output_path);
    void compressLZ77(const std::string &file_path, const std::string &output_path);
    void compressLZ78(const std::string &file_path, const std::string &output_path);

    void decompressHuffman(const std::string &file_path);
    void decompressLZ77(const std::string &file_path);
    void decompressLZ78(const std::string &file_path);
};

#endif //COURSE_PROJECT_COMPRESSOR_H
