#pragma once

#include <QDebug>
#include <QDateTime>

class Logger
{
public:
    enum LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    static void setLogLevel(LogLevel level);
    static void enableLogging(bool enable);

    template<typename... Args>
    static void debug(const QString &format, Args... args);

    template<typename... Args>
    static void info(const QString &format, Args... args);

    template<typename... Args>
    static void warning(const QString &format, Args... args);

    template<typename... Args>
    static void error(const QString &format, Args... args);

private:
    static LogLevel logLevel;
    static bool loggingEnabled;

    template<typename... Args>
    static void logHelper(LogLevel level, const QString &format, Args... args);

    template<typename... Args>
    static void log(LogLevel level, const QString &format, Args... args);

    static void log(LogLevel level, const QString &format);
};

#include "logger.inl"  // 包含模板实现
