#include "log.h"

ChessLogger& ChessLogger::getInstance(const std::string& logFileName) {
    static ChessLogger instance(logFileName);
    return instance;
}

ChessLogger::ChessLogger(const std::string& logFileName) 
    : logFile(logFileName, std::ios::out | std::ios::trunc)
{
}

ChessLogger::ChessLogger()
    : logFile("Schaakplezier.log", std::ios::out | std::ios::trunc)
{
}

ChessLogger::~ChessLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}