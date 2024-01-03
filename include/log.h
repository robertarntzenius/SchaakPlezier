#pragma once

#include "definitions.h"
#include "bitboard.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio> // Include the header for snprintf
#include <cstdarg>


class ChessLogger {
public:
    static ChessLogger& getInstance(const std::string &logFileName = "Schaakplezier.log") {
        static ChessLogger instance(logFileName);
        return instance;
    }

    ~ChessLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(std::ostringstream& os) {
        if (logFile.is_open()) {
            logFile << os.str() << std::endl << std::endl;
        }
    }

    void log(const char* msg) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
        }
    }

    void log(const char* msg, const Bitboard& bitboard) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
            logFile << bitboard << std::endl;
        }
    }

    template <typename T>
    void log(const T& msg) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
        }
    }

    template <typename... Args>
    void log(const char* format, Args... args) {
        if (logFile.is_open()) {
            std::ostringstream oss;
            formatAndLog(oss, format, args...);
            logFile << oss.str() << std::endl;
        }
    }

    void logHeader(const std::string& headerName) {
        const std::size_t headerSize = 50;

        if (headerName.size() > headerSize) {
            throw std::invalid_argument("Header name too long: " + std::to_string(headerName.size()));
        }

        std::string spaces((headerSize - headerName.length()) / 2, ' ');
        std::string equals(headerSize, '=');
        std::string header = "\n" + equals +
                             "\n" + spaces + headerName +
                             "\n" + equals +
                             "\n";
        log(header.c_str());
    }

private:
    explicit ChessLogger(const std::string& logFileName)
        : logFile(logFileName, std::ios::out | std::ios::trunc)
    {
        if (!logFile.is_open()) {
            std::cerr << "Error: Unable to open logfile: '" << logFileName << "'\n";
        }
    }

    template <typename... Args>
    void formatAndLog(std::ostringstream& oss, const char* format, Args... args) {
        char buffer[256];
        std::snprintf(buffer, 256, format, args...);
        oss << buffer;
    }

    std::ofstream logFile;
};
