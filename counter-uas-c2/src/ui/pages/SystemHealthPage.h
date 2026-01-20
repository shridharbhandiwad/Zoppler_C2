#ifndef SYSTEMHEALTHPAGE_H
#define SYSTEMHEALTHPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QMap>

namespace CounterUAS {

/**
 * StatCard - Widget displaying a system statistic
 */
class StatCard : public QFrame {
    Q_OBJECT
    
public:
    explicit StatCard(QWidget* parent = nullptr);
    
    void setTitle(const QString& title);
    void setValue(const QString& value);
    void setSubtitle(const QString& subtitle);
    void setIcon(const QString& icon);
    
private:
    void setupUI();
    
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QLabel* m_valueLabel;
    QLabel* m_subtitleLabel;
};

/**
 * ServiceStatusItem - Widget displaying a service status
 */
class ServiceStatusItem : public QWidget {
    Q_OBJECT
    
public:
    explicit ServiceStatusItem(QWidget* parent = nullptr);
    
    void setServiceName(const QString& name);
    void setUptime(const QString& uptime);
    void setStatus(const QString& status);  // OPERATIONAL, PROCESSING, OFFLINE
    
private:
    void setupUI();
    void updateStyle();
    
    QString m_status;
    QLabel* m_nameLabel;
    QLabel* m_uptimeLabel;
    QLabel* m_statusIndicator;
    QLabel* m_statusLabel;
};

/**
 * NetworkNodeItem - Widget displaying a network node
 */
class NetworkNodeItem : public QWidget {
    Q_OBJECT
    
public:
    explicit NetworkNodeItem(QWidget* parent = nullptr);
    
    void setNodeId(const QString& id);
    void setNodeName(const QString& name);
    void setIpAddress(const QString& ip);
    void setLatency(int latencyMs);
    
private:
    void setupUI();
    
    QLabel* m_idBadge;
    QLabel* m_nameLabel;
    QLabel* m_ipLabel;
    QLabel* m_latencyValue;
    QLabel* m_latencyLabel;
};

/**
 * SystemHealthPage - System Health monitoring view
 * 
 * Shows:
 * - Hardware stats (CPU, Memory, Storage, Uplink)
 * - Service status
 * - Network nodes
 */
class SystemHealthPage : public QWidget {
    Q_OBJECT
    
public:
    explicit SystemHealthPage(QWidget* parent = nullptr);
    ~SystemHealthPage() override = default;
    
public slots:
    void updateCpuLoad(int percent);
    void updateMemory(double usedGB, double totalGB);
    void updateStorage(int percent, const QString& type);
    void updateUplink(double speedGbps, const QString& type);
    void updateServiceStatus(const QString& service, const QString& status, const QString& uptime);
    void updateNetworkNode(const QString& nodeId, const QString& name, 
                          const QString& ip, int latencyMs);
    
private:
    void setupUI();
    void setupStatsRow();
    void setupServiceStatus();
    void setupNetworkNodes();
    
    // Stat cards
    StatCard* m_cpuCard;
    StatCard* m_memoryCard;
    StatCard* m_storageCard;
    StatCard* m_uplinkCard;
    
    // Service items
    QVBoxLayout* m_servicesLayout;
    QMap<QString, ServiceStatusItem*> m_serviceItems;
    
    // Network nodes
    QVBoxLayout* m_nodesLayout;
    QMap<QString, NetworkNodeItem*> m_nodeItems;
};

} // namespace CounterUAS

#endif // SYSTEMHEALTHPAGE_H
