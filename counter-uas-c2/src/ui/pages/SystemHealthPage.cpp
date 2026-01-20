#include "ui/pages/SystemHealthPage.h"
#include "ui/SkyGuardTheme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>

namespace CounterUAS {

// ==================== StatCard Implementation ====================

StatCard::StatCard(QWidget* parent)
    : QFrame(parent)
{
    setupUI();
}

void StatCard::setupUI() {
    setObjectName("statCard");
    setMinimumSize(180, 100);
    setMaximumHeight(120);
    
    setStyleSheet(
        "QFrame#statCard {"
        "   background-color: #111d2e;"
        "   border: 1px solid #00d4ff;"
        "   border-radius: 8px;"
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(4);
    
    // Header row: title + icon
    QHBoxLayout* headerRow = new QHBoxLayout();
    
    m_titleLabel = new QLabel("CPU LOAD", this);
    m_titleLabel->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #00d4ff; letter-spacing: 1px;"
    );
    headerRow->addWidget(m_titleLabel);
    headerRow->addStretch();
    
    m_iconLabel = new QLabel("\xE2\x9A\x99", this);
    m_iconLabel->setStyleSheet("font-size: 16px; color: #00d4ff;");
    headerRow->addWidget(m_iconLabel);
    
    layout->addLayout(headerRow);
    
    // Value
    m_valueLabel = new QLabel("12%", this);
    m_valueLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #00d4ff;"
    );
    layout->addWidget(m_valueLabel);
    
    // Subtitle
    m_subtitleLabel = new QLabel("32 Cores Active", this);
    m_subtitleLabel->setStyleSheet(
        "font-size: 11px; color: #667788;"
    );
    layout->addWidget(m_subtitleLabel);
}

void StatCard::setTitle(const QString& title) {
    m_titleLabel->setText(title);
}

void StatCard::setValue(const QString& value) {
    m_valueLabel->setText(value);
}

void StatCard::setSubtitle(const QString& subtitle) {
    m_subtitleLabel->setText(subtitle);
}

void StatCard::setIcon(const QString& icon) {
    m_iconLabel->setText(icon);
}

// ==================== ServiceStatusItem Implementation ====================

ServiceStatusItem::ServiceStatusItem(QWidget* parent)
    : QWidget(parent)
    , m_status("OFFLINE")
{
    setupUI();
}

void ServiceStatusItem::setupUI() {
    setMinimumHeight(60);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 8, 0, 8);
    layout->setSpacing(12);
    
    // Name and uptime
    QVBoxLayout* nameLayout = new QVBoxLayout();
    nameLayout->setSpacing(2);
    
    m_nameLabel = new QLabel("Core Processing Unit", this);
    m_nameLabel->setStyleSheet(
        "font-size: 13px; font-weight: bold; color: #ffffff;"
    );
    nameLayout->addWidget(m_nameLabel);
    
    m_uptimeLabel = new QLabel("Uptime: 14d 2h", this);
    m_uptimeLabel->setStyleSheet(
        "font-size: 11px; color: #667788;"
    );
    nameLayout->addWidget(m_uptimeLabel);
    
    layout->addLayout(nameLayout);
    layout->addStretch();
    
    // Status indicator
    m_statusIndicator = new QLabel("\xE2\x97\x8F", this);  // Filled circle
    m_statusIndicator->setStyleSheet("font-size: 12px; color: #00ff88;");
    layout->addWidget(m_statusIndicator);
    
    m_statusLabel = new QLabel("OPERATIONAL", this);
    m_statusLabel->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #00ff88;"
    );
    layout->addWidget(m_statusLabel);
    
    updateStyle();
}

void ServiceStatusItem::setServiceName(const QString& name) {
    m_nameLabel->setText(name);
}

void ServiceStatusItem::setUptime(const QString& uptime) {
    m_uptimeLabel->setText(QString("Uptime: %1").arg(uptime));
}

void ServiceStatusItem::setStatus(const QString& status) {
    m_status = status.toUpper();
    m_statusLabel->setText(m_status);
    updateStyle();
}

void ServiceStatusItem::updateStyle() {
    QString color = "#00ff88";
    
    if (m_status == "PROCESSING" || m_status == "BUSY") {
        color = "#ff8800";
    } else if (m_status == "OFFLINE" || m_status == "ERROR") {
        color = "#ff3344";
    } else if (m_status == "DEGRADED" || m_status == "WARNING") {
        color = "#ffaa00";
    }
    
    m_statusIndicator->setStyleSheet(QString("font-size: 12px; color: %1;").arg(color));
    m_statusLabel->setStyleSheet(QString("font-size: 12px; font-weight: bold; color: %1;").arg(color));
}

// ==================== NetworkNodeItem Implementation ====================

NetworkNodeItem::NetworkNodeItem(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void NetworkNodeItem::setupUI() {
    setMinimumHeight(50);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 6, 0, 6);
    layout->setSpacing(12);
    
    // Node ID badge
    m_idBadge = new QLabel("N1", this);
    m_idBadge->setFixedSize(32, 32);
    m_idBadge->setAlignment(Qt::AlignCenter);
    m_idBadge->setStyleSheet(
        "background-color: #1a2d44; color: #ffffff; "
        "font-size: 11px; font-weight: bold; border-radius: 4px;"
    );
    layout->addWidget(m_idBadge);
    
    // Name and IP
    QVBoxLayout* nameLayout = new QVBoxLayout();
    nameLayout->setSpacing(0);
    
    m_nameLabel = new QLabel("Remote Node Alpha-1", this);
    m_nameLabel->setStyleSheet("font-size: 12px; color: #ffffff;");
    nameLayout->addWidget(m_nameLabel);
    
    m_ipLabel = new QLabel("192.168.10.101", this);
    m_ipLabel->setStyleSheet("font-size: 10px; color: #667788;");
    nameLayout->addWidget(m_ipLabel);
    
    layout->addLayout(nameLayout);
    layout->addStretch();
    
    // Latency
    QVBoxLayout* latencyLayout = new QVBoxLayout();
    latencyLayout->setSpacing(0);
    latencyLayout->setAlignment(Qt::AlignRight);
    
    m_latencyValue = new QLabel("12ms", this);
    m_latencyValue->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    m_latencyValue->setAlignment(Qt::AlignRight);
    latencyLayout->addWidget(m_latencyValue);
    
    m_latencyLabel = new QLabel("LATENCY", this);
    m_latencyLabel->setStyleSheet("font-size: 9px; color: #667788;");
    m_latencyLabel->setAlignment(Qt::AlignRight);
    latencyLayout->addWidget(m_latencyLabel);
    
    layout->addLayout(latencyLayout);
}

void NetworkNodeItem::setNodeId(const QString& id) {
    m_idBadge->setText(id);
}

void NetworkNodeItem::setNodeName(const QString& name) {
    m_nameLabel->setText(name);
}

void NetworkNodeItem::setIpAddress(const QString& ip) {
    m_ipLabel->setText(ip);
}

void NetworkNodeItem::setLatency(int latencyMs) {
    m_latencyValue->setText(QString("%1ms").arg(latencyMs));
    
    // Color code latency
    QString color = "#00ff88";  // Good
    if (latencyMs > 100) {
        color = "#ff3344";  // Bad
    } else if (latencyMs > 50) {
        color = "#ffaa00";  // Warning
    }
    
    m_latencyValue->setStyleSheet(QString("font-size: 14px; font-weight: bold; color: %1;").arg(color));
}

// ==================== SystemHealthPage Implementation ====================

SystemHealthPage::SystemHealthPage(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void SystemHealthPage::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(24);
    
    // Header section
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);
    
    // Icon
    QLabel* iconLabel = new QLabel("\xE2\x86\x97", this);
    iconLabel->setStyleSheet(
        "font-size: 28px; color: #00ff88; background-color: #111d2e; "
        "padding: 10px; border-radius: 8px;"
    );
    headerLayout->addWidget(iconLabel);
    
    // Title
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    
    QLabel* titleLabel = new QLabel("SYSTEM HEALTH", this);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #ffffff; letter-spacing: 2px;"
    );
    titleLayout->addWidget(titleLabel);
    
    QLabel* subtitleLabel = new QLabel("Hardware status, resource usage, and network connectivity", this);
    subtitleLabel->setStyleSheet(
        "font-size: 13px; color: #667788;"
    );
    titleLayout->addWidget(subtitleLabel);
    
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    mainLayout->addLayout(headerLayout);
    
    setupStatsRow();
    
    // Bottom section: Services + Network
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(24);
    
    setupServiceStatus();
    setupNetworkNodes();
    
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();
}

void SystemHealthPage::setupStatsRow() {
    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(16);
    
    // CPU Load
    m_cpuCard = new StatCard(this);
    m_cpuCard->setTitle("CPU LOAD");
    m_cpuCard->setValue("12%");
    m_cpuCard->setSubtitle("32 Cores Active");
    m_cpuCard->setIcon("\xE2\x9A\x99");
    statsLayout->addWidget(m_cpuCard);
    
    // Memory
    m_memoryCard = new StatCard(this);
    m_memoryCard->setTitle("MEMORY");
    m_memoryCard->setValue("4.2GB");
    m_memoryCard->setSubtitle("32GB Total");
    m_memoryCard->setIcon("\xE2\x96\xA1");
    statsLayout->addWidget(m_memoryCard);
    
    // Storage
    m_storageCard = new StatCard(this);
    m_storageCard->setTitle("STORAGE");
    m_storageCard->setValue("45%");
    m_storageCard->setSubtitle("12TB RAID 5");
    m_storageCard->setIcon("\xE2\x96\xA1");
    statsLayout->addWidget(m_storageCard);
    
    // Uplink
    m_uplinkCard = new StatCard(this);
    m_uplinkCard->setTitle("UPLINK");
    m_uplinkCard->setValue("1.2Gbps");
    m_uplinkCard->setSubtitle("Satellite Link Primary");
    m_uplinkCard->setIcon("\xE2\x87\x84");
    statsLayout->addWidget(m_uplinkCard);
    
    statsLayout->addStretch();
    
    static_cast<QVBoxLayout*>(layout())->addLayout(statsLayout);
}

void SystemHealthPage::setupServiceStatus() {
    QFrame* servicesSection = new QFrame(this);
    servicesSection->setStyleSheet(
        "QFrame { background-color: #111d2e; border: 1px solid #1a3344; border-radius: 8px; }"
    );
    
    QVBoxLayout* sectionLayout = new QVBoxLayout(servicesSection);
    sectionLayout->setContentsMargins(20, 16, 20, 16);
    sectionLayout->setSpacing(8);
    
    // Header
    QHBoxLayout* headerRow = new QHBoxLayout();
    QLabel* icon = new QLabel("\xE2\x96\xA1", this);
    icon->setStyleSheet("font-size: 16px; color: #00d4ff;");
    headerRow->addWidget(icon);
    
    QLabel* title = new QLabel("SERVICE STATUS", this);
    title->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #00d4ff; letter-spacing: 1px;"
    );
    headerRow->addWidget(title);
    headerRow->addStretch();
    sectionLayout->addLayout(headerRow);
    
    // Services
    m_servicesLayout = new QVBoxLayout();
    m_servicesLayout->setSpacing(4);
    
    // Add default services
    ServiceStatusItem* cpu = new ServiceStatusItem(this);
    cpu->setServiceName("Core Processing Unit");
    cpu->setUptime("14d 2h");
    cpu->setStatus("OPERATIONAL");
    m_servicesLayout->addWidget(cpu);
    m_serviceItems["CPU"] = cpu;
    
    ServiceStatusItem* fusion = new ServiceStatusItem(this);
    fusion->setServiceName("Sensor Fusion Engine");
    fusion->setUptime("14d 2h");
    fusion->setStatus("OPERATIONAL");
    m_servicesLayout->addWidget(fusion);
    m_serviceItems["FUSION"] = fusion;
    
    ServiceStatusItem* ai = new ServiceStatusItem(this);
    ai->setServiceName("Threat Analysis AI");
    ai->setUptime("14d 2h");
    ai->setStatus("PROCESSING");
    m_servicesLayout->addWidget(ai);
    m_serviceItems["AI"] = ai;
    
    sectionLayout->addLayout(m_servicesLayout);
    sectionLayout->addStretch();
    
    static_cast<QHBoxLayout*>(static_cast<QVBoxLayout*>(layout())->itemAt(
        static_cast<QVBoxLayout*>(layout())->count() - 1)->layout())->addWidget(servicesSection);
}

void SystemHealthPage::setupNetworkNodes() {
    QFrame* nodesSection = new QFrame(this);
    nodesSection->setStyleSheet(
        "QFrame { background-color: #111d2e; border: 1px solid #1a3344; border-radius: 8px; }"
    );
    
    QVBoxLayout* sectionLayout = new QVBoxLayout(nodesSection);
    sectionLayout->setContentsMargins(20, 16, 20, 16);
    sectionLayout->setSpacing(8);
    
    // Header
    QHBoxLayout* headerRow = new QHBoxLayout();
    QLabel* icon = new QLabel("\xE2\x87\x84", this);
    icon->setStyleSheet("font-size: 16px; color: #00d4ff;");
    headerRow->addWidget(icon);
    
    QLabel* title = new QLabel("NETWORK NODES", this);
    title->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #00d4ff; letter-spacing: 1px;"
    );
    headerRow->addWidget(title);
    headerRow->addStretch();
    sectionLayout->addLayout(headerRow);
    
    // Nodes
    m_nodesLayout = new QVBoxLayout();
    m_nodesLayout->setSpacing(4);
    
    // Add default nodes
    for (int i = 1; i <= 4; ++i) {
        NetworkNodeItem* node = new NetworkNodeItem(this);
        node->setNodeId(QString("N%1").arg(i));
        node->setNodeName(QString("Remote Node Alpha-%1").arg(i));
        node->setIpAddress(QString("192.168.10.%1").arg(100 + i));
        node->setLatency(12);
        m_nodesLayout->addWidget(node);
        m_nodeItems[QString("N%1").arg(i)] = node;
    }
    
    sectionLayout->addLayout(m_nodesLayout);
    sectionLayout->addStretch();
    
    static_cast<QHBoxLayout*>(static_cast<QVBoxLayout*>(layout())->itemAt(
        static_cast<QVBoxLayout*>(layout())->count() - 1)->layout())->addWidget(nodesSection);
}

void SystemHealthPage::updateCpuLoad(int percent) {
    m_cpuCard->setValue(QString("%1%").arg(percent));
}

void SystemHealthPage::updateMemory(double usedGB, double totalGB) {
    m_memoryCard->setValue(QString("%1GB").arg(usedGB, 0, 'f', 1));
    m_memoryCard->setSubtitle(QString("%1GB Total").arg(static_cast<int>(totalGB)));
}

void SystemHealthPage::updateStorage(int percent, const QString& type) {
    m_storageCard->setValue(QString("%1%").arg(percent));
    m_storageCard->setSubtitle(type);
}

void SystemHealthPage::updateUplink(double speedGbps, const QString& type) {
    m_uplinkCard->setValue(QString("%1Gbps").arg(speedGbps, 0, 'f', 1));
    m_uplinkCard->setSubtitle(type);
}

void SystemHealthPage::updateServiceStatus(const QString& service, const QString& status, const QString& uptime) {
    if (m_serviceItems.contains(service)) {
        m_serviceItems[service]->setStatus(status);
        m_serviceItems[service]->setUptime(uptime);
    }
}

void SystemHealthPage::updateNetworkNode(const QString& nodeId, const QString& name,
                                         const QString& ip, int latencyMs) {
    if (m_nodeItems.contains(nodeId)) {
        m_nodeItems[nodeId]->setNodeName(name);
        m_nodeItems[nodeId]->setIpAddress(ip);
        m_nodeItems[nodeId]->setLatency(latencyMs);
    }
}

} // namespace CounterUAS
