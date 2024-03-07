#include "logger.h"

inline void Logger::setLogLevel(LogLevel level) {
    logLevel = level;
}

inline void Logger::enableLogging(bool enable) {
    loggingEnabled = enable;
}

template<typename... Args>
inline void Logger::debug(const QString &format, Args... args) {
    log(Debug, format, args...);
}

template<typename... Args>
inline void Logger::info(const QString &format, Args... args) {
    log(Info, format, args...);
}

template<typename... Args>
inline void Logger::warning(const QString &format, Args... args) {
    log(Warning, format, args...);
}

template<typename... Args>
inline void Logger::error(const QString &format, Args... args) {
    log(Error, format, args...);
}

template<typename... Args>
inline void Logger::logHelper(LogLevel level, const QString &format, Args... args) {
    if (loggingEnabled && level >= logLevel) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        QString levelString;

        switch (level) {
        case Debug: levelString = "DEBUG"; break;
        case Info: levelString = "INFO"; break;
        case Warning: levelString = "WARNING"; break;
        case Error: levelString = "ERROR"; break;
        }

        QString logMessage;
        if constexpr (sizeof...(Args) > 0) {
            logMessage = QString("[%1] [%2] %3")
                             .arg(timestamp)
                             .arg(levelString)
                             .arg(format.arg(args...));
        } else {
            logMessage = QString("[%1] [%2] %3")
                             .arg(timestamp)
                             .arg(levelString)
                             .arg(format);
        }

        qDebug().noquote() << logMessage;
    }
}

template<typename... Args>
inline void Logger::log(LogLevel level, const QString &format, Args... args) {
    logHelper(level, format, args...);
}

inline void Logger::log(LogLevel level, const QString &format) {
    logHelper(level, format);
}
