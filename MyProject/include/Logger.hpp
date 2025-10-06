#pragma once

#include <string>
#include "nocopyable.hpp"

enum LogLevel
{
    INFO,  // 普通信息
    ERROR, // 错误信息
    FATAL, // core信息
    DEBUG, // 调试信息
};

class Logger : nocopyable
{
public:
    static Logger *instance();

    void setLogLevel(int level);

    void Log(std::string msg);

private:
    int logLevel_;
};