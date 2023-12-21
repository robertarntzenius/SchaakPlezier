#include "log.h"

ChessLogger::ChessLogger(const std::string& logFileName) 
    : logFile(logFileName, std::ios::out | std::ios::trunc)
{
}

ChessLogger::ChessLogger()
    : logFile("Schaakplezier.log", std::ios::out | std::ios::trunc)
{
}

ChessLogger& ChessLogger::getInstance(const std::string& logFileName) {
    static ChessLogger instance(logFileName);
    return instance;
}

ChessLogger::~ChessLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void ChessLogger::logHeader(const std::string& text) {
    int linesize = 50;

    std::string spaces((linesize - text.length()) / 2, ' ');
    std::string equals(linesize, '=');
    std::string header = "\n" + equals +
                         "\n" + spaces + text +
                         "\n" + equals +
                         "\n";
    log(header);
}