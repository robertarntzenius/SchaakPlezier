#pragma once

#include "definitions.h"
#include "bitboard.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdarg>

enum LogLevel {
    LEVEL_ALL = 2,
    LEVEL_DEBUG = 1,
    LEVEL_RELEASE = 0
};

// TODO make inline constexpr of log calls to optimize them away! 
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

    void setLogLevel(LogLevel level) {
        logLevel = level;
    }
    
    template <typename... Args>
    void debug(Args... args) {
        if (logLevel < LEVEL_DEBUG) return;
        // log("DEBUG");
        log(args...);
    }

    template <typename... Args>
    void release(Args... args) {
        if (logLevel < LEVEL_RELEASE) return;
        // log("RELEASE");
        log(args...);
    }

    template <typename... Args>
    void all(Args... args) {
        // log("ALL");
        log(args...);
    }

    void logHeader(std::string headerName) {
        logHeader(headerName, "");
    }

    template<typename T>
    void logHeader(std::string headerName, T arg) {
        const std::size_t headerSize = 50;
        std::stringstream ss;

        ss << headerName << "(" << arg << ")";
        headerName = ss.str();

        if (headerName.size() > headerSize) {
            throw std::invalid_argument("Header name too long: " + std::to_string(headerName.size()));
        }

        std::string equals((headerSize - headerName.length() - 2) / 2, '=');
        std::string header = "\n" + equals + ' ' + headerName + ' ' + equals;
        log(header.c_str());
    }
// private:
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
    void log(Args... args) {
        if (logFile.is_open()) {
            ((logFile << args << ' '), ...) << std::endl;
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
private:
    explicit ChessLogger(const std::string& logFileName, LogLevel level = LEVEL_RELEASE)
        : logFile(logFileName, std::ios::out | std::ios::trunc), logLevel(level)
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
LogLevel logLevel;
};
