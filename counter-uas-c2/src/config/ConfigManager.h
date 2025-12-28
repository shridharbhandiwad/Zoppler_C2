#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

namespace CounterUAS {

class ConfigManager : public QObject {
    Q_OBJECT
    
public:
    static ConfigManager& instance();
    
    bool loadConfig(const QString& path);
    bool saveConfig(const QString& path);
    bool loadDefaults();
    
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setValue(const QString& key, const QVariant& value);
    
    QJsonObject section(const QString& name) const;
    void setSection(const QString& name, const QJsonObject& section);
    
    QString configPath() const { return m_configPath; }
    
signals:
    void configChanged(const QString& key);
    void configLoaded();
    
private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    QJsonObject m_config;
    QString m_configPath;
};

} // namespace CounterUAS

#endif // CONFIGMANAGER_H
