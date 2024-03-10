#pragma once

#include "definitions.h"
#include "bitboard.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <utility>

/* 
LEVEL_VERBOSE: Extra debug info + more verbose, logged if level=verbose
LEVEL_DEBUG: debug info, logged if level = verbose/debug
LEVEL_ESSENTIAL: only required output to play chess, always logged
*/
enum LogLevel {
    LEVEL_ESSENTIAL = 0,
    LEVEL_DEBUG = 1,
    LEVEL_VERBOSE = 2
};

class ChessLogger {
public:
    static ChessLogger& getInstance(const std::string &logFileName = "Schaakplezier.log") {
        static ChessLogger instance;
        instance.setLogFile(logFileName);
        return instance;
    }

    ~ChessLogger() {
        for (auto &entry : logFilesMap) {
            std::ofstream &logFile = entry.second;
            if (logFile.is_open()) {
                logFile.close();
            }
        }
    }

    void setLogFile(const std::string& logFileName) {
        if (logFilesMap[logFileName].is_open()) {
            return;
        }

        logFilesMap[logFileName] = std::ofstream(logFileName, std::ios::out | std::ios::trunc);
        currentLogFileName = logFileName;
        
        if (!logFilesMap[currentLogFileName].is_open()) {
            std::cerr << "Error: Unable to open logFilesMap[currentLogFileName]: '" << logFileName << "'\n";
        }
    }

    void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    LogLevel getLogLevel() {
        return logLevel;
    }
    
    template <typename... Args>
    inline void debug(Args... args) {
        if (logLevel < LEVEL_DEBUG) return;
        log(args...);
    }

    template <typename T>
    inline void debug(T arg) {
        if (logLevel < LEVEL_DEBUG) return;
        log(arg);
    }

    template <typename... Args>
    inline void essential(Args... args) {
        if (logLevel < LEVEL_ESSENTIAL) return;
        log(args...);
    }

    template <typename T>
    inline void essential(T arg) {
        if (logLevel < LEVEL_ESSENTIAL) return;
        log(arg);
    }

    template <typename... Args>
    inline void verbose(Args... args) {
        if (logLevel < LEVEL_VERBOSE) return;
        log(LEVEL_VERBOSE, args...);
    }

    template <typename T>
    inline void verbose(T arg) {
        if (logLevel < LEVEL_VERBOSE) return;
        log(arg);
    }

    void logHeader(std::string headerName) {
        if (logLevel < LEVEL_VERBOSE) return;
        logHeader(std::move(headerName), "");
    }

    template<typename T>
    void logHeader(std::string headerName, T arg) {
        if (logLevel < LEVEL_DEBUG) return;
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

private:
    void log(std::ostringstream& os) {
        if (logFilesMap[currentLogFileName].is_open()) {
            logFilesMap[currentLogFileName] << os.str() << std::endl << std::endl;
        }
    }

    void log(const char* msg) {
        if (logFilesMap[currentLogFileName].is_open()) {
            logFilesMap[currentLogFileName] << msg << std::endl;
        }
    }

    void log(const char* msg, const Bitboard& bitboard) {
        if (logFilesMap[currentLogFileName].is_open()) {
            logFilesMap[currentLogFileName] << msg << std::endl;
            logFilesMap[currentLogFileName] << bitboard << std::endl;
        }
    }

    template <typename T>
    void log(const T& msg) {
        if (logFilesMap[currentLogFileName].is_open()) {
            logFilesMap[currentLogFileName] << msg << std::endl;
        }
    }

    template <typename... Args>
    void log(Args... args) {
        if (logFilesMap[currentLogFileName].is_open()) {
            ((logFilesMap[currentLogFileName] << args << ' '), ...) << std::endl;
        }
    }

    template <typename... Args>
    void log(const char* format, Args... args) {
        if (logFilesMap[currentLogFileName].is_open()) {
            std::ostringstream oss;
            formatAndLog(oss, format, args...);
            logFilesMap[currentLogFileName] << oss.str() << std::endl;
        }
    }

    explicit ChessLogger(LogLevel level = LEVEL_ESSENTIAL)
        : logFilesMap(), currentLogFileName(), logLevel(level)
    {
    }

    template <typename... Args>
    void formatAndLog(std::ostringstream& oss, const char* format, Args... args) {
        char buffer[256];
        std::snprintf(buffer, 256, format, args...);
        oss << buffer;
    }

    std::unordered_map<std::string, std::ofstream> logFilesMap;
    std::string currentLogFileName;
    LogLevel logLevel;
};