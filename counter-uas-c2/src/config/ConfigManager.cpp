#include "config/ConfigManager.h"
#include "utils/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

namespace CounterUAS {

ConfigManager& ConfigManager::instance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() {
    loadDefaults();
}

bool ConfigManager::loadConfig(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::instance().error("ConfigManager", "Failed to open config: " + path);
        return false;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();
    
    if (error.error != QJsonParseError::NoError) {
        Logger::instance().error("ConfigManager", "JSON parse error: " + error.errorString());
        return false;
    }
    
    m_config = doc.object();
    m_configPath = path;
    
    Logger::instance().info("ConfigManager", "Loaded config: " + path);
    emit configLoaded();
    return true;
}

bool ConfigManager::saveConfig(const QString& path) {
    QString savePath = path.isEmpty() ? m_configPath : path;
    
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        Logger::instance().error("ConfigManager", "Failed to save config: " + savePath);
        return false;
    }
    
    QJsonDocument doc(m_config);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    Logger::instance().info("ConfigManager", "Saved config: " + savePath);
    return true;
}

bool ConfigManager::loadDefaults() {
    m_config = QJsonObject();
    
    // System defaults
    QJsonObject system;
    system["updateRateHz"] = 10;
    system["logLevel"] = "INFO";
    system["logPath"] = "logs/";
    m_config["system"] = system;
    
    // Track manager defaults
    QJsonObject trackManager;
    trackManager["correlationDistanceM"] = 100.0;
    trackManager["coastingTimeoutMs"] = 5000;
    trackManager["dropTimeoutMs"] = 15000;
    trackManager["enableKalmanFilter"] = true;
    m_config["trackManager"] = trackManager;
    
    // Threat assessor defaults
    QJsonObject threatAssessor;
    threatAssessor["assessmentIntervalMs"] = 500;
    threatAssessor["highThreatThreshold"] = 4;
    threatAssessor["autoSlewToHighestThreat"] = true;
    m_config["threatAssessor"] = threatAssessor;
    
    // Video defaults
    QJsonObject video;
    video["defaultFps"] = 30;
    video["maxStreams"] = 16;
    video["recordingPath"] = "recordings/";
    video["preBufferSeconds"] = 30;
    m_config["video"] = video;
    
    // Map defaults
    QJsonObject map;
    map["defaultCenterLat"] = 34.0522;
    map["defaultCenterLon"] = -118.2437;
    map["defaultZoom"] = 15;
    m_config["map"] = map;
    
    return true;
}

QVariant ConfigManager::value(const QString& key, const QVariant& defaultValue) const {
    QStringList parts = key.split('/');
    QJsonObject current = m_config;
    
    for (int i = 0; i < parts.size() - 1; ++i) {
        if (!current.contains(parts[i])) return defaultValue;
        current = current[parts[i]].toObject();
    }
    
    if (!current.contains(parts.last())) return defaultValue;
    return current[parts.last()].toVariant();
}

void ConfigManager::setValue(const QString& key, const QVariant& value) {
    QStringList parts = key.split('/');
    
    // Navigate/create path
    QJsonObject* current = &m_config;
    for (int i = 0; i < parts.size() - 1; ++i) {
        if (!current->contains(parts[i])) {
            current->insert(parts[i], QJsonObject());
        }
        QJsonObject child = (*current)[parts[i]].toObject();
        // This is simplified - in production, need proper nested update
    }
    
    (*current)[parts.last()] = QJsonValue::fromVariant(value);
    emit configChanged(key);
}

QJsonObject ConfigManager::section(const QString& name) const {
    return m_config.value(name).toObject();
}

void ConfigManager::setSection(const QString& name, const QJsonObject& section) {
    m_config[name] = section;
    emit configChanged(name);
}

} // namespace CounterUAS
