#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QList>

namespace CounterUAS {

/**
 * @brief Log level enum
 */
enum class LogLevel {
    Debug = 0,
    Info,
    Warning,
    Error,
    Critical
};

/**
 * @brief Log entry structure
 */
struct LogEntry {
    QDateTime timestamp;
    LogLevel level;
    QString category;
    QString message;
    QString threadId;
};

/**
 * @brief Singleton logger class
 */
class Logger : public QObject {
    Q_OBJECT
    
public:
    static Logger& instance();
    
    // Configuration
    void setLogLevel(LogLevel level) { m_minLevel = level; }
    LogLevel logLevel() const { return m_minLevel; }
    
    void setLogToFile(bool enable, const QString& path = QString());
    void setLogToConsole(bool enable) { m_logToConsole = enable; }
    void setMaxLogEntries(int count) { m_maxEntries = count; }
    
    // Logging methods
    void debug(const QString& category, const QString& message);
    void info(const QString& category, const QString& message);
    void warning(const QString& category, const QString& message);
    void error(const QString& category, const QString& message);
    void critical(const QString& category, const QString& message);
    
    void log(LogLevel level, const QString& category, const QString& message);
    
    // Log retrieval
    QList<LogEntry> recentLogs(int count = 100) const;
    QList<LogEntry> logsByLevel(LogLevel level) const;
    QList<LogEntry> logsByCategory(const QString& category) const;
    void clearLogs();
    
    // Export
    bool exportToFile(const QString& path) const;
    
signals:
    void logAdded(const LogEntry& entry);
    
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void writeToFile(const LogEntry& entry);
    QString formatEntry(const LogEntry& entry) const;
    QString levelString(LogLevel level) const;
    
    mutable QMutex m_mutex;
    QList<LogEntry> m_logs;
    
    LogLevel m_minLevel = LogLevel::Info;
    bool m_logToConsole = true;
    bool m_logToFile = false;
    QFile* m_logFile = nullptr;
    int m_maxEntries = 10000;
};

} // namespace CounterUAS

#endif // LOGGER_H
