#pragma once

#include "definitions.h"
#include "bitboard.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio> // Include the header for sprintf
#include <cstdarg>

class ChessLogger { // TODO add error handling for when logfile cannot be opened
public:
    static ChessLogger& getInstance(const std::string& logFileName = "Schaakplezier.log");
    ChessLogger(const std::string& logFileName);
    ChessLogger();
    ~ChessLogger();
    
    void log(const char* msg, const Bitboard& bitboard) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
            logFile << bitboard << std::endl;
        }
    }

    void log(const char* msg, const Piece &piece) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
            logFile << piece << std::endl;
        }
    }
    
    void log(const char* msg) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
        }
    }

    template <typename T>
    void log(const T& message) {
        if (logFile.is_open()) {
            logFile << message << std::endl;
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

    void logHeader(const std::string& header);

private:
    template <typename... Args>
    void formatAndLog(std::ostringstream& oss, const char* format, Args... args) {
        char buffer[256];
        std::snprintf(buffer, 256, format, args...);
        oss << buffer;
    }

    std::ofstream logFile;
};
