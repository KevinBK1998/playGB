#include "Logger.h"

using namespace std;

string Logger::levelToString(LogLevel level)
{
    switch (level)
    {
    case DEBUG:
        return "DEBUG";
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    case CRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

Logger::Logger()
{
    logFile = new ofstream(LOG_FILE, ios::app);
    if (!logFile->is_open())
    {
        cerr << "Error opening log file." << endl;
    }
}

Logger::~Logger()
{
    logFile->close();
    free(logFile);
}

void Logger::setLogLevel(LogLevel level) { minLogLevel = level; }

void Logger::log(LogLevel level, const string &context, const string &message)
{
    if (level < minLogLevel)
        return;
    // Get current timestamp
    time_t now = time(0);
    tm *timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp),
             "%Y-%m-%dT%H:%M:%S", timeinfo);

    // Create log entry
    ostringstream logEntry;
    logEntry << timestamp << " "
             << context << " "
             << "[" << levelToString(level) << "] : "
             << message << endl;

    // Output to console
    cout << logEntry.str();

    // Output to log file
    if (logFile->is_open())
    {
        (*logFile) << logEntry.str();
        logFile->flush(); // Ensure immediate write to file
    }
}

void Logger::debug(const string &context, const string &message) { log(DEBUG, context, message); }
void Logger::info(const string &context, const string &message) { log(INFO, context, message); }
void Logger::warn(const string &context, const string &message) { log(WARNING, context, message); }
void Logger::error(const string &context, const string &message) { log(ERROR, context, message); }

void Logger::logByte(const string &context, const string &name, uint8_t byteValue)
{
    ostringstream messageStream;
    messageStream << name << " = " << hex << showbase << unsigned(byteValue);
    debug(context, messageStream.str());
}

void Logger::logWord(const string &context, const string &name, uint16_t wordValue)
{
    ostringstream messageStream;
    messageStream << name << " = " << hex << showbase << wordValue;
    debug(context, messageStream.str());
}
