#include "ui/pages/SensorNetworkPage.h"
#include "ui/SkyGuardTheme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>

namespace CounterUAS {

// ==================== SensorCard Implementation ====================

SensorCard::SensorCard(QWidget* parent)
    : QFrame(parent)
    , m_status("OFFLINE")
{
    setupUI();
}

void SensorCard::setupUI() {
    setObjectName("sensorCard");
    setMinimumSize(280, 200);
    setMaximumSize(320, 220);
    setCursor(Qt::PointingHandCursor);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 16, 20, 16);
    layout->setSpacing(12);
    
    // Header row: Icon + Name + Signal icon
    QHBoxLayout* headerRow = new QHBoxLayout();
    headerRow->setSpacing(12);
    
    // Sensor icon
    m_iconLabel = new QLabel("\xE2\x86\x97", this);  // Arrow symbol
    m_iconLabel->setStyleSheet(
        "font-size: 20px; color: #00ff88; "
        "background-color: #1a2d44; padding: 8px; border-radius: 6px;"
    );
    m_iconLabel->setFixedSize(40, 40);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    headerRow->addWidget(m_iconLabel);
    
    // Name and type
    QVBoxLayout* nameLayout = new QVBoxLayout();
    nameLayout->setSpacing(2);
    
    m_nameLabel = new QLabel("SENTINEL-1", this);
    m_nameLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #ffffff; letter-spacing: 1px;"
    );
    nameLayout->addWidget(m_nameLabel);
    
    m_typeLabel = new QLabel("RADAR SENSOR", this);
    m_typeLabel->setStyleSheet(
        "font-size: 10px; color: #667788; letter-spacing: 1px;"
    );
    nameLayout->addWidget(m_typeLabel);
    
    headerRow->addLayout(nameLayout);
    headerRow->addStretch();
    
    // Signal icon
    m_signalIcon = new QLabel("\xE2\x96\x88", this);  // Signal bars
    m_signalIcon->setStyleSheet("font-size: 18px; color: #00ff88;");
    headerRow->addWidget(m_signalIcon);
    
    layout->addLayout(headerRow);
    
    // Stats grid
    QGridLayout* statsGrid = new QGridLayout();
    statsGrid->setSpacing(8);
    statsGrid->setColumnStretch(1, 1);
    
    // Coverage
    m_coverageLabel = new QLabel("Coverage", this);
    m_coverageLabel->setStyleSheet("font-size: 10px; color: #667788;");
    statsGrid->addWidget(m_coverageLabel, 0, 0);
    
    m_coverageValue = new QLabel("5.0 km", this);
    m_coverageValue->setStyleSheet("font-size: 14px; font-weight: bold; color: #00d4ff;");
    statsGrid->addWidget(m_coverageValue, 1, 0);
    
    // Status
    m_statusLabel = new QLabel("Status", this);
    m_statusLabel->setStyleSheet("font-size: 10px; color: #667788;");
    statsGrid->addWidget(m_statusLabel, 0, 1);
    
    m_statusValue = new QLabel("ONLINE", this);
    m_statusValue->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #0d1a2d; "
        "background-color: #00ff88; padding: 4px 10px; border-radius: 8px;"
    );
    m_statusValue->setFixedHeight(24);
    statsGrid->addWidget(m_statusValue, 1, 1);
    
    layout->addLayout(statsGrid);
    
    // Coordinates and Azimuth
    QHBoxLayout* coordRow = new QHBoxLayout();
    coordRow->setSpacing(24);
    
    QVBoxLayout* coordCol = new QVBoxLayout();
    coordCol->setSpacing(2);
    m_coordsLabel = new QLabel("Coordinates", this);
    m_coordsLabel->setStyleSheet("font-size: 10px; color: #667788;");
    coordCol->addWidget(m_coordsLabel);
    m_coordsValue = new QLabel("34.052, -118.244", this);
    m_coordsValue->setStyleSheet("font-size: 12px; color: #ffffff;");
    coordCol->addWidget(m_coordsValue);
    coordRow->addLayout(coordCol);
    
    QVBoxLayout* azCol = new QVBoxLayout();
    azCol->setSpacing(2);
    m_azimuthLabel = new QLabel("Azimuth", this);
    m_azimuthLabel->setStyleSheet("font-size: 10px; color: #667788;");
    azCol->addWidget(m_azimuthLabel);
    m_azimuthValue = new QLabel("360\xC2\xB0", this);
    m_azimuthValue->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    azCol->addWidget(m_azimuthValue);
    coordRow->addLayout(azCol);
    
    coordRow->addStretch();
    layout->addLayout(coordRow);
    
    updateStyle();
}

void SensorCard::setSensorId(const QString& id) {
    m_sensorId = id;
}

void SensorCard::setSensorName(const QString& name) {
    m_nameLabel->setText(name);
}

void SensorCard::setSensorType(const QString& type) {
    m_typeLabel->setText(type.toUpper());
    
    // Update icon based on type
    if (type.toUpper().contains("RADAR")) {
        m_iconLabel->setText("\xE2\x86\x97");
        m_iconLabel->setStyleSheet(
            "font-size: 20px; color: #00ff88; "
            "background-color: #1a2d44; padding: 8px; border-radius: 6px;"
        );
    } else if (type.toUpper().contains("RF")) {
        m_iconLabel->setText("\xC2\xB7");
        m_iconLabel->setStyleSheet(
            "font-size: 28px; color: #00ff88; "
            "background-color: #1a2d44; padding: 8px; border-radius: 6px;"
        );
    } else if (type.toUpper().contains("CAMERA") || type.toUpper().contains("OPTICAL")) {
        m_iconLabel->setText("\xE2\x97\x89");
        m_iconLabel->setStyleSheet(
            "font-size: 20px; color: #00ff88; "
            "background-color: #1a2d44; padding: 8px; border-radius: 6px;"
        );
    }
}

void SensorCard::setCoverage(double coverageKm) {
    m_coverageValue->setText(QString("%1 km").arg(coverageKm, 0, 'f', 1));
}

void SensorCard::setStatus(const QString& status) {
    m_status = status.toUpper();
    m_statusValue->setText(m_status);
    updateStyle();
}

void SensorCard::setCoordinates(double lat, double lng) {
    m_coordsValue->setText(QString("%1, %2").arg(lat, 0, 'f', 3).arg(lng, 0, 'f', 3));
}

void SensorCard::setAzimuth(double degrees) {
    m_azimuthValue->setText(QString("%1\xC2\xB0").arg(static_cast<int>(degrees)));
}

void SensorCard::updateStyle() {
    QString borderColor = "#1a3344";
    QString statusBg = "#ffaa00";
    QString statusColor = "#0d1a2d";
    QString signalColor = "#ffaa00";
    
    if (m_status == "ONLINE") {
        statusBg = "#00ff88";
        signalColor = "#00ff88";
    } else if (m_status == "OFFLINE") {
        statusBg = "#ff3344";
        signalColor = "#ff3344";
    } else if (m_status == "DEGRADED") {
        statusBg = "#ffaa00";
        signalColor = "#ffaa00";
    }
    
    setStyleSheet(QString(
        "QFrame#sensorCard {"
        "   background-color: #111d2e;"
        "   border: 1px solid %1;"
        "   border-radius: 8px;"
        "}"
        "QFrame#sensorCard:hover {"
        "   background-color: #1a2d44;"
        "   border-color: #00d4ff;"
        "}"
    ).arg(borderColor));
    
    m_statusValue->setStyleSheet(QString(
        "font-size: 11px; font-weight: bold; color: %2; "
        "background-color: %1; padding: 4px 10px; border-radius: 8px;"
    ).arg(statusBg).arg(statusColor));
    
    m_signalIcon->setStyleSheet(QString("font-size: 18px; color: %1;").arg(signalColor));
}

void SensorCard::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_sensorId);
    }
    QFrame::mousePressEvent(event);
}

// ==================== SensorNetworkPage Implementation ====================

SensorNetworkPage::SensorNetworkPage(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupDefaultSensors();
}

void SensorNetworkPage::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(24);
    
    // Header section
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);
    
    // Icon and title
    QLabel* iconLabel = new QLabel("\xC2\xB7", this);
    iconLabel->setStyleSheet(
        "font-size: 36px; color: #00d4ff; background-color: #111d2e; "
        "padding: 10px; border-radius: 8px;"
    );
    headerLayout->addWidget(iconLabel);
    
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    
    QLabel* titleLabel = new QLabel("SENSOR NETWORK", this);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #ffffff; letter-spacing: 2px;"
    );
    titleLayout->addWidget(titleLabel);
    
    QLabel* subtitleLabel = new QLabel("Management of Radar, RF, and Optical detection assets", this);
    subtitleLabel->setStyleSheet(
        "font-size: 13px; color: #667788;"
    );
    titleLayout->addWidget(subtitleLabel);
    
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    // Add sensor button
    m_addButton = new QPushButton("ADD SENSOR", this);
    m_addButton->setCursor(Qt::PointingHandCursor);
    m_addButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #00d4ff;"
        "   border: none;"
        "   color: #0d1a2d;"
        "   border-radius: 4px;"
        "   padding: 12px 24px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #00a8cc;"
        "}"
    );
    connect(m_addButton, &QPushButton::clicked,
            this, &SensorNetworkPage::addSensorClicked);
    headerLayout->addWidget(m_addButton);
    
    mainLayout->addLayout(headerLayout);
    
    // Sensor cards container
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: transparent; border: none; }"
    );
    
    QWidget* cardsContainer = new QWidget(this);
    cardsContainer->setStyleSheet("background-color: transparent;");
    m_cardsLayout = new QHBoxLayout(cardsContainer);
    m_cardsLayout->setContentsMargins(0, 0, 0, 0);
    m_cardsLayout->setSpacing(20);
    m_cardsLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    scrollArea->setWidget(cardsContainer);
    mainLayout->addWidget(scrollArea);
    
    mainLayout->addStretch();
}

void SensorNetworkPage::setupDefaultSensors() {
    // Add default sensors matching the reference design
    addSensor("SENTINEL-1", "SENTINEL-1", "RADAR SENSOR");
    updateSensorCoverage("SENTINEL-1", 5.0);
    updateSensorStatus("SENTINEL-1", "ONLINE");
    updateSensorCoordinates("SENTINEL-1", 34.052, -118.244);
    updateSensorAzimuth("SENTINEL-1", 360);
    
    addSensor("RF-SCANNER-ALPHA", "RF-SCANNER-ALPHA", "RF SENSOR");
    updateSensorCoverage("RF-SCANNER-ALPHA", 3.0);
    updateSensorStatus("RF-SCANNER-ALPHA", "ONLINE");
    updateSensorCoordinates("RF-SCANNER-ALPHA", 34.062, -118.254);
    updateSensorAzimuth("RF-SCANNER-ALPHA", 360);
}

void SensorNetworkPage::addSensor(const QString& id, const QString& name, const QString& type) {
    if (m_sensorCards.contains(id)) return;
    
    SensorCard* card = new SensorCard(this);
    card->setSensorId(id);
    card->setSensorName(name);
    card->setSensorType(type);
    
    connect(card, &SensorCard::clicked,
            this, &SensorNetworkPage::onSensorCardClicked);
    
    m_sensorCards[id] = card;
    m_cardsLayout->addWidget(card);
}

void SensorNetworkPage::updateSensorStatus(const QString& id, const QString& status) {
    if (m_sensorCards.contains(id)) {
        m_sensorCards[id]->setStatus(status);
    }
}

void SensorNetworkPage::updateSensorCoverage(const QString& id, double coverageKm) {
    if (m_sensorCards.contains(id)) {
        m_sensorCards[id]->setCoverage(coverageKm);
    }
}

void SensorNetworkPage::updateSensorCoordinates(const QString& id, double lat, double lng) {
    if (m_sensorCards.contains(id)) {
        m_sensorCards[id]->setCoordinates(lat, lng);
    }
}

void SensorNetworkPage::updateSensorAzimuth(const QString& id, double degrees) {
    if (m_sensorCards.contains(id)) {
        m_sensorCards[id]->setAzimuth(degrees);
    }
}

void SensorNetworkPage::onSensorCardClicked(const QString& sensorId) {
    emit sensorSelected(sensorId);
}

} // namespace CounterUAS
