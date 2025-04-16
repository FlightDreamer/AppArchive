#include "zstd_utils.h"
#include <zstd.h>
#include <iostream>
#include <stdexcept>
#include <signal.h>
#include <atomic>
#include <vector>
#include <fstream>

std::atomic<bool> running(true);

void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cerr << "\nOperation interrupted by user. Exiting...\n";
        running = false;
    }
}

namespace zstd_utils {

    bool compressFile(const std::string& inputFile, const std::string& outputFile, int compressionLevel, int numThreads) {
        std::ifstream in(inputFile, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Failed to open input file: " + inputFile);
        }

        std::ofstream out(outputFile, std::ios::binary);
        if (!out) {
            throw std::runtime_error("Failed to open output file: " + outputFile);
        }

        ZSTD_CStream* cstream = ZSTD_createCStream();
        if (!cstream) {
            throw std::runtime_error("Failed to create compression stream");
        }

        // Настройка многопоточности (только для сжатия)
        if (numThreads > 0) {
            size_t const threadsResult = ZSTD_CCtx_setParameter(cstream, ZSTD_c_nbWorkers, numThreads);
            if (ZSTD_isError(threadsResult)) {
                ZSTD_freeCStream(cstream);
                throw std::runtime_error("Failed to set thread count: " + std::string(ZSTD_getErrorName(threadsResult)));
            }
        }

        size_t const initResult = ZSTD_initCStream(cstream, compressionLevel);
        if (ZSTD_isError(initResult)) {
            ZSTD_freeCStream(cstream);
            throw std::runtime_error("Failed to initialize compression stream");
        }

        signal(SIGINT, signalHandler);

        const size_t bufferSize = ZSTD_CStreamInSize();
        std::vector<char> buffer(bufferSize);

        while (running && in.read(buffer.data(), bufferSize)) {
            size_t const readBytes = in.gcount();
            ZSTD_inBuffer input = {buffer.data(), readBytes, 0};

            while (input.pos < input.size && running) {
                std::vector<char> compressedBuffer(ZSTD_CStreamOutSize());
                ZSTD_outBuffer output = {compressedBuffer.data(), compressedBuffer.size(), 0};

                size_t const result = ZSTD_compressStream(cstream, &output, &input);
                if (ZSTD_isError(result)) {
                    ZSTD_freeCStream(cstream);
                    throw std::runtime_error("Compression failed: " + std::string(ZSTD_getErrorName(result)));
                }

                out.write(compressedBuffer.data(), output.pos);
            }
        }

        if (running) {
            ZSTD_outBuffer output = {nullptr, 0, 0};
            size_t const result = ZSTD_endStream(cstream, &output);
            if (ZSTD_isError(result)) {
                ZSTD_freeCStream(cstream);
                throw std::runtime_error("Failed to finalize compression: " + std::string(ZSTD_getErrorName(result)));
            }
        }

        ZSTD_freeCStream(cstream);
        return running;
    }

    bool decompressFile(const std::string& inputFile, const std::string& outputFile) {
        std::ifstream in(inputFile, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Failed to open input file: " + inputFile);
        }

        std::ofstream out(outputFile, std::ios::binary);
        if (!out) {
            throw std::runtime_error("Failed to open output file: " + outputFile);
        }

        ZSTD_DStream* dstream = ZSTD_createDStream();
        if (!dstream) {
            throw std::runtime_error("Failed to create decompression stream");
        }

        size_t const initResult = ZSTD_initDStream(dstream);
        if (ZSTD_isError(initResult)) {
            ZSTD_freeDStream(dstream);
            throw std::runtime_error("Failed to initialize decompression stream");
        }

        signal(SIGINT, signalHandler);

        const size_t bufferSize = ZSTD_DStreamInSize();
        std::vector<char> buffer(bufferSize);

        while (running && in.read(buffer.data(), bufferSize)) {
            size_t const readBytes = in.gcount();
            ZSTD_inBuffer input = {buffer.data(), readBytes, 0};

            while (input.pos < input.size && running) {
                std::vector<char> decompressedBuffer(ZSTD_DStreamOutSize());
                ZSTD_outBuffer output = {decompressedBuffer.data(), decompressedBuffer.size(), 0};

                size_t const result = ZSTD_decompressStream(dstream, &output, &input);
                if (ZSTD_isError(result)) {
                    ZSTD_freeDStream(dstream);
                    throw std::runtime_error("Decompression failed: " + std::string(ZSTD_getErrorName(result)));
                }

                out.write(decompressedBuffer.data(), output.pos);
            }
        }

        ZSTD_freeDStream(dstream);
        return running;
    }

} // namespace zstd_utils