#ifndef ZSTD_UTILS_H
#define ZSTD_UTILS_H

#include <string>
#include <fstream>

namespace zstd_utils {

    // Функция для архивации файла
    bool compressFile(const std::string& inputFile, const std::string& outputFile, int compressionLevel, int numThreads);

    // Функция для распаковки файла
    bool decompressFile(const std::string& inputFile, const std::string& outputFile);

} // namespace zstd_utils

#endif