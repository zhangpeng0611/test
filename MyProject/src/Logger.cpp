
#include "Logger.hpp"
#include "Timestamp.hpp"

#include <iostream>

Logger *Logger::instance()
{
    static Logger logger;
    return &logger;
}

void Logger::setLogLevel(int level)
{
    this->logLevel_ = level;
}

void Logger::Log(std::string msg)
{
    switch (logLevel_)
    {
    case INFO:
        std::cout << "[INFO]";
        break;
    case ERROR:
        std::cout << "[ERROR]";
    case FATAL:
        break;
        std::cout << "[FATAL]";
    case DEBUG:
        break;
        std::cout << "[DEBUG]";
        break;
    default:
        break;
    }

    std::cout << Timestamp::now().toString() << ":" << msg << std::endl;
}