#include "utils/Logger.h"
#include <QMutexLocker>
#include <QTextStream>
#include <QThread>
#include <iostream>

namespace CounterUAS {

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {}

Logger::~Logger() {
    if (m_logFile) {
        m_logFile->close();
        delete m_logFile;
    }
}

void Logger::setLogToFile(bool enable, const QString& path) {
    QMutexLocker locker(&m_mutex);
    
    if (m_logFile) {
        m_logFile->close();
        delete m_logFile;
        m_logFile = nullptr;
    }
    
    m_logToFile = enable;
    
    if (enable && !path.isEmpty()) {
        m_logFile = new QFile(path);
        if (!m_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            delete m_logFile;
            m_logFile = nullptr;
            m_logToFile = false;
        }
    }
}

void Logger::debug(const QString& category, const QString& message) {
    log(LogLevel::Debug, category, message);
}

void Logger::info(const QString& category, const QString& message) {
    log(LogLevel::Info, category, message);
}

void Logger::warning(const QString& category, const QString& message) {
    log(LogLevel::Warning, category, message);
}

void Logger::error(const QString& category, const QString& message) {
    log(LogLevel::Error, category, message);
}

void Logger::critical(const QString& category, const QString& message) {
    log(LogLevel::Critical, category, message);
}

void Logger::log(LogLevel level, const QString& category, const QString& message) {
    if (level < m_minLevel) return;
    
    LogEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.level = level;
    entry.category = category;
    entry.message = message;
    entry.threadId = QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()));
    
    {
        QMutexLocker locker(&m_mutex);
        
        m_logs.append(entry);
        while (m_logs.size() > m_maxEntries) {
            m_logs.removeFirst();
        }
        
        if (m_logToConsole) {
            QString formatted = formatEntry(entry);
            if (level >= LogLevel::Error) {
                std::cerr << formatted.toStdString() << std::endl;
            } else {
                std::cout << formatted.toStdString() << std::endl;
            }
        }
        
        if (m_logToFile && m_logFile) {
            writeToFile(entry);
        }
    }
    
    emit logAdded(entry);
}

QList<LogEntry> Logger::recentLogs(int count) const {
    QMutexLocker locker(&m_mutex);
    
    if (count >= m_logs.size()) {
        return m_logs;
    }
    
    return m_logs.mid(m_logs.size() - count);
}

QList<LogEntry> Logger::logsByLevel(LogLevel level) const {
    QMutexLocker locker(&m_mutex);
    
    QList<LogEntry> result;
    for (const auto& entry : m_logs) {
        if (entry.level == level) {
            result.append(entry);
        }
    }
    return result;
}

QList<LogEntry> Logger::logsByCategory(const QString& category) const {
    QMutexLocker locker(&m_mutex);
    
    QList<LogEntry> result;
    for (const auto& entry : m_logs) {
        if (entry.category == category) {
            result.append(entry);
        }
    }
    return result;
}

void Logger::clearLogs() {
    QMutexLocker locker(&m_mutex);
    m_logs.clear();
}

bool Logger::exportToFile(const QString& path) const {
    QMutexLocker locker(&m_mutex);
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    for (const auto& entry : m_logs) {
        stream << formatEntry(entry) << "\n";
    }
    
    file.close();
    return true;
}

void Logger::writeToFile(const LogEntry& entry) {
    if (!m_logFile || !m_logFile->isOpen()) return;
    
    QTextStream stream(m_logFile);
    stream << formatEntry(entry) << "\n";
    stream.flush();
}

QString Logger::formatEntry(const LogEntry& entry) const {
    return QString("[%1] [%2] [%3] %4")
               .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
               .arg(levelString(entry.level), -8)
               .arg(entry.category, -20)
               .arg(entry.message);
}

QString Logger::levelString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

} // namespace CounterUAS
