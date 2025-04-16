#include "zstd_utils.h"
#include <iostream>
#include <cstdlib>

void printUsage() {
    std::cerr << "Usage:\n"
              << "  myapp a <original file> <archive file> [threads] [compression level] - Compress file\n"
              << "  myapp e <archive file> <original file>                               - Decompress file\n"
              << "Optional arguments:\n"
              << "  threads: Number of threads (default: 1)\n"
              << "  compression level: Compression level (default: 5)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 6) { // Минимум 4 аргумента, максимум 6
        printUsage();
        return EXIT_FAILURE;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    int compressionLevel = 5; // Значение по умолчанию для уровня сжатия
    int numThreads = 1;       // Значение по умолчанию для количества потоков

    try {
        if (mode == "a") {
            if (argc >= 5) {
                numThreads = std::stoi(argv[4]); // Четвертый аргумент - количество потоков
            }
            if (argc >= 6) {
                compressionLevel = std::stoi(argv[5]); // Пятый аргумент - уровень сжатия
            }

            if (!zstd_utils::compressFile(inputFile, outputFile, compressionLevel, numThreads)) {
                std::cerr << "Compression interrupted.\n";
                return EXIT_FAILURE;
            }
            std::cout << "File compressed successfully.\n";
        } else if (mode == "e") {
            if (!zstd_utils::decompressFile(inputFile, outputFile)) {
                std::cerr << "Decompression interrupted.\n";
                return EXIT_FAILURE;
            }
            std::cout << "File decompressed successfully.\n";
        } else {
            printUsage();
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}