#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QString>
#include <QDateTime>

namespace CounterUAS {

class TimeUtils {
public:
    static QString formatDuration(qint64 ms);
    static QString formatTimestamp(qint64 ms);
    static QString formatTimeAgo(const QDateTime& time);
    static qint64 parseTimestamp(const QString& str);
    static QString zuluTime();
    static QDateTime fromZulu(const QString& zulu);
};

} // namespace CounterUAS

#endif // TIMEUTILS_H
