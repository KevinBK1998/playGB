#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#ifndef LOGGER_H
#define LOGGER_H

enum LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

const string LOG_FILE = "console.log";
static LogLevel minLogLevel = WARNING;

class Logger
{
    ofstream *logFile;
    string context;
    string levelToString(LogLevel level);
    void log(LogLevel level, const string &context, const string &message);

public:
    Logger();
    ~Logger();
    void setLogLevel(LogLevel level);
    void debug(const string &context, const string &message);
    void info(const string &context, const string &message);
    void warn(const string &context, const string &message);
    void error(const string &context, const string &message);
    void logByte(const string &context, const string &name, uint8_t byteValue);
    void logWord(const string &context, const string &name, uint16_t wordValue);
};

static Logger logger = Logger();

#endif
