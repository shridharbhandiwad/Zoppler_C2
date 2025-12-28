#include "utils/TimeUtils.h"

namespace CounterUAS {

QString TimeUtils::formatDuration(qint64 ms) {
    if (ms < 1000) return QString("%1ms").arg(ms);
    if (ms < 60000) return QString("%1.%2s").arg(ms / 1000).arg((ms % 1000) / 100);
    
    int hours = ms / 3600000;
    int mins = (ms % 3600000) / 60000;
    int secs = (ms % 60000) / 1000;
    
    if (hours > 0) {
        return QString("%1:%2:%3").arg(hours).arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
    }
    return QString("%1:%2").arg(mins).arg(secs, 2, 10, QChar('0'));
}

QString TimeUtils::formatTimestamp(qint64 ms) {
    return QDateTime::fromMSecsSinceEpoch(ms).toString("hh:mm:ss.zzz");
}

QString TimeUtils::formatTimeAgo(const QDateTime& time) {
    qint64 secs = time.secsTo(QDateTime::currentDateTimeUtc());
    if (secs < 0) return "in the future";
    if (secs < 60) return QString("%1s ago").arg(secs);
    if (secs < 3600) return QString("%1m ago").arg(secs / 60);
    if (secs < 86400) return QString("%1h ago").arg(secs / 3600);
    return QString("%1d ago").arg(secs / 86400);
}

qint64 TimeUtils::parseTimestamp(const QString& str) {
    return QDateTime::fromString(str, Qt::ISODateWithMs).toMSecsSinceEpoch();
}

QString TimeUtils::zuluTime() {
    return QDateTime::currentDateTimeUtc().toString("yyyyMMdd'T'HHmmss'Z'");
}

QDateTime TimeUtils::fromZulu(const QString& zulu) {
    return QDateTime::fromString(zulu, "yyyyMMdd'T'HHmmss'Z'");
}

} // namespace CounterUAS
