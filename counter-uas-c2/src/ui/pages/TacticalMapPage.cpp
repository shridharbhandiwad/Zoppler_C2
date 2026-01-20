#include "ui/pages/TacticalMapPage.h"
#include "ui/MapWidget.h"
#include "ui/SkyGuardTheme.h"
#include "core/TrackManager.h"
#include "core/ThreatAssessor.h"
#include "core/Track.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>

namespace CounterUAS {

// ==================== ThreatCard Implementation ====================

ThreatCard::ThreatCard(QWidget* parent)
    : QFrame(parent)
    , m_status("UNKNOWN")
{
    setupUI();
}

void ThreatCard::setupUI() {
    setObjectName("threatCard");
    setMinimumHeight(140);
    setMaximumHeight(160);
    setCursor(Qt::PointingHandCursor);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(8);
    
    // Header row: ID + Status badge
    QHBoxLayout* headerRow = new QHBoxLayout();
    headerRow->setSpacing(12);
    
    m_idLabel = new QLabel("UNKNOWN-001", this);
    m_idLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #ffffff; letter-spacing: 1px;"
    );
    headerRow->addWidget(m_idLabel);
    
    headerRow->addStretch();
    
    m_statusBadge = new QLabel("HOSTILE", this);
    m_statusBadge->setStyleSheet(
        "background-color: #ff3344; color: #ffffff; font-size: 10px; "
        "font-weight: bold; padding: 4px 12px; border-radius: 10px;"
    );
    headerRow->addWidget(m_statusBadge);
    
    layout->addLayout(headerRow);
    
    // Classification
    m_classLabel = new QLabel("DJI Mavic 3", this);
    m_classLabel->setStyleSheet(
        "font-size: 13px; color: #ffffff;"
    );
    layout->addWidget(m_classLabel);
    
    // Stats row
    QHBoxLayout* statsRow = new QHBoxLayout();
    statsRow->setSpacing(24);
    
    // Altitude
    QVBoxLayout* altCol = new QVBoxLayout();
    altCol->setSpacing(2);
    m_altLabel = new QLabel("ALT", this);
    m_altLabel->setStyleSheet("font-size: 10px; color: #667788; letter-spacing: 1px;");
    altCol->addWidget(m_altLabel);
    m_altValue = new QLabel("120m", this);
    m_altValue->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    altCol->addWidget(m_altValue);
    statsRow->addLayout(altCol);
    
    // Speed
    QVBoxLayout* spdCol = new QVBoxLayout();
    spdCol->setSpacing(2);
    m_spdLabel = new QLabel("SPD", this);
    m_spdLabel->setStyleSheet("font-size: 10px; color: #667788; letter-spacing: 1px;");
    spdCol->addWidget(m_spdLabel);
    m_spdValue = new QLabel("15.0m/s", this);
    m_spdValue->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    spdCol->addWidget(m_spdValue);
    statsRow->addLayout(spdCol);
    
    // Source
    QVBoxLayout* srcCol = new QVBoxLayout();
    srcCol->setSpacing(2);
    m_srcLabel = new QLabel("SRC", this);
    m_srcLabel->setStyleSheet("font-size: 10px; color: #667788; letter-spacing: 1px;");
    srcCol->addWidget(m_srcLabel);
    m_srcValue = new QLabel("Sentinel-1", this);
    m_srcValue->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    srcCol->addWidget(m_srcValue);
    statsRow->addLayout(srcCol);
    
    statsRow->addStretch();
    layout->addLayout(statsRow);
    
    // Progress bar
    m_progressBar = new QFrame(this);
    m_progressBar->setFixedHeight(4);
    m_progressBar->setStyleSheet(
        "background-color: #ff3344; border-radius: 2px;"
    );
    layout->addWidget(m_progressBar);
    
    updateStyle();
}

void ThreatCard::setTrackId(const QString& id) {
    m_trackId = id;
    m_idLabel->setText(id);
}

void ThreatCard::setClassification(const QString& type) {
    m_classLabel->setText(type);
}

void ThreatCard::setStatus(const QString& status) {
    m_status = status.toUpper();
    m_statusBadge->setText(m_status);
    updateStyle();
}

void ThreatCard::setAltitude(double altitudeM) {
    m_altValue->setText(QString("%1m").arg(static_cast<int>(altitudeM)));
}

void ThreatCard::setSpeed(double speedMps) {
    m_spdValue->setText(QString("%1m/s").arg(speedMps, 0, 'f', 1));
}

void ThreatCard::setSource(const QString& source) {
    m_srcValue->setText(source);
}

void ThreatCard::setProgressValue(double progress) {
    // Progress bar width as percentage (visual indicator)
    int maxWidth = width() - 32;
    int barWidth = static_cast<int>(maxWidth * qBound(0.0, progress, 1.0));
    m_progressBar->setFixedWidth(barWidth > 0 ? barWidth : maxWidth);
}

void ThreatCard::updateStyle() {
    QString borderColor = "#ff3344";  // Default hostile
    QString badgeBg = "#ff3344";
    
    if (m_status == "FRIENDLY") {
        borderColor = "#00cc66";
        badgeBg = "#00cc66";
    } else if (m_status == "PENDING" || m_status == "UNKNOWN") {
        borderColor = "#ffaa00";
        badgeBg = "#ffaa00";
    }
    
    setStyleSheet(QString(
        "QFrame#threatCard {"
        "   background-color: #111d2e;"
        "   border: 1px solid #1a3344;"
        "   border-radius: 8px;"
        "   border-left: 4px solid %1;"
        "}"
        "QFrame#threatCard:hover {"
        "   background-color: #1a2d44;"
        "}"
    ).arg(borderColor));
    
    m_statusBadge->setStyleSheet(QString(
        "background-color: %1; color: #ffffff; font-size: 10px; "
        "font-weight: bold; padding: 4px 12px; border-radius: 10px;"
    ).arg(badgeBg));
    
    m_progressBar->setStyleSheet(QString(
        "background-color: %1; border-radius: 2px;"
    ).arg(borderColor));
}

void ThreatCard::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_trackId);
    }
    QFrame::mousePressEvent(event);
}

// ==================== AlertItem Implementation ====================

AlertItem::AlertItem(QWidget* parent)
    : QFrame(parent)
{
    setupUI();
}

void AlertItem::setupUI() {
    setObjectName("alertItem");
    setMinimumHeight(60);
    
    setStyleSheet(
        "QFrame#alertItem {"
        "   background-color: #111d2e;"
        "   border: 1px solid #1a3344;"
        "   border-radius: 6px;"
        "   border-left: 3px solid #3388ff;"
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(4);
    
    // Header row
    QHBoxLayout* headerRow = new QHBoxLayout();
    headerRow->setSpacing(8);
    
    m_timeLabel = new QLabel("12:33:44 AM", this);
    m_timeLabel->setStyleSheet("font-size: 11px; color: #667788;");
    headerRow->addWidget(m_timeLabel);
    
    headerRow->addStretch();
    
    m_severityBadge = new QLabel("INFO", this);
    m_severityBadge->setStyleSheet(
        "background-color: #3388ff; color: #ffffff; font-size: 9px; "
        "font-weight: bold; padding: 2px 8px; border-radius: 6px;"
    );
    headerRow->addWidget(m_severityBadge);
    
    layout->addLayout(headerRow);
    
    // Message
    m_messageLabel = new QLabel("Alert message here", this);
    m_messageLabel->setStyleSheet("font-size: 12px; color: #ffffff;");
    m_messageLabel->setWordWrap(true);
    layout->addWidget(m_messageLabel);
}

void AlertItem::setTime(const QString& time) {
    m_timeLabel->setText(time);
}

void AlertItem::setSeverity(const QString& severity) {
    QString sev = severity.toUpper();
    m_severityBadge->setText(sev);
    
    QString badgeBg = "#3388ff";
    QString borderColor = "#3388ff";
    
    if (sev == "CRITICAL" || sev == "DANGER") {
        badgeBg = "#ff3344";
        borderColor = "#ff3344";
    } else if (sev == "WARNING") {
        badgeBg = "#ffaa00";
        borderColor = "#ffaa00";
    }
    
    m_severityBadge->setStyleSheet(QString(
        "background-color: %1; color: #ffffff; font-size: 9px; "
        "font-weight: bold; padding: 2px 8px; border-radius: 6px;"
    ).arg(badgeBg));
    
    setStyleSheet(QString(
        "QFrame#alertItem {"
        "   background-color: #111d2e;"
        "   border: 1px solid #1a3344;"
        "   border-radius: 6px;"
        "   border-left: 3px solid %1;"
        "}"
    ).arg(borderColor));
}

void AlertItem::setMessage(const QString& message) {
    m_messageLabel->setText(message);
}

// ==================== TacticalMapPage Implementation ====================

TacticalMapPage::TacticalMapPage(QWidget* parent)
    : QWidget(parent)
    , m_mapWidget(nullptr)
    , m_trackManager(nullptr)
    , m_threatAssessor(nullptr)
{
    setupUI();
}

void TacticalMapPage::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    setupMapSection();
    setupRightPanel();
}

void TacticalMapPage::setupMapSection() {
    QWidget* mapSection = new QWidget(this);
    mapSection->setStyleSheet("background-color: #0a1628;");
    QVBoxLayout* mapLayout = new QVBoxLayout(mapSection);
    mapLayout->setContentsMargins(0, 0, 0, 0);
    mapLayout->setSpacing(0);
    
    // Status cards row
    QWidget* statusCardsWidget = new QWidget(this);
    statusCardsWidget->setStyleSheet("background-color: transparent;");
    statusCardsWidget->setFixedHeight(80);
    QHBoxLayout* cardsLayout = new QHBoxLayout(statusCardsWidget);
    cardsLayout->setContentsMargins(16, 12, 16, 12);
    cardsLayout->setSpacing(12);
    
    // Active Threats card
    QFrame* threatsCard = new QFrame(this);
    threatsCard->setStyleSheet(
        "QFrame { background-color: #111d2e; border: 1px solid #1a3344; border-radius: 8px; }"
    );
    QHBoxLayout* threatsCardLayout = new QHBoxLayout(threatsCard);
    threatsCardLayout->setContentsMargins(16, 8, 16, 8);
    
    QLabel* threatsIcon = new QLabel("\xE2\x97\x89", this);  // Circled dot
    threatsIcon->setStyleSheet("font-size: 18px; color: #ff3344;");
    threatsCardLayout->addWidget(threatsIcon);
    
    QVBoxLayout* threatsTextLayout = new QVBoxLayout();
    threatsTextLayout->setSpacing(0);
    QLabel* threatsLabel = new QLabel("ACTIVE THREATS", this);
    threatsLabel->setStyleSheet("font-size: 10px; color: #ff3344; font-weight: bold; letter-spacing: 1px;");
    threatsTextLayout->addWidget(threatsLabel);
    m_activeThreatsValue = new QLabel("2", this);
    m_activeThreatsValue->setStyleSheet("font-size: 24px; color: #ff3344; font-weight: bold;");
    threatsTextLayout->addWidget(m_activeThreatsValue);
    threatsCardLayout->addLayout(threatsTextLayout);
    
    cardsLayout->addWidget(threatsCard);
    
    // Sensor Net card
    QFrame* sensorCard = new QFrame(this);
    sensorCard->setStyleSheet(
        "QFrame { background-color: #111d2e; border: 1px solid #1a3344; border-radius: 8px; }"
    );
    QHBoxLayout* sensorCardLayout = new QHBoxLayout(sensorCard);
    sensorCardLayout->setContentsMargins(16, 8, 16, 8);
    
    QLabel* sensorIcon = new QLabel("\xC2\xB7", this);
    sensorIcon->setStyleSheet("font-size: 24px; color: #00ff88;");
    sensorCardLayout->addWidget(sensorIcon);
    
    QVBoxLayout* sensorTextLayout = new QVBoxLayout();
    sensorTextLayout->setSpacing(0);
    QLabel* sensorLabel = new QLabel("SENSOR NET", this);
    sensorLabel->setStyleSheet("font-size: 10px; color: #00ff88; font-weight: bold; letter-spacing: 1px;");
    sensorTextLayout->addWidget(sensorLabel);
    m_sensorNetValue = new QLabel("2/2", this);
    m_sensorNetValue->setStyleSheet("font-size: 24px; color: #00ff88; font-weight: bold;");
    sensorTextLayout->addWidget(m_sensorNetValue);
    sensorCardLayout->addLayout(sensorTextLayout);
    
    cardsLayout->addWidget(sensorCard);
    
    // Coordinates card
    QFrame* coordCard = new QFrame(this);
    coordCard->setStyleSheet(
        "QFrame { background-color: #111d2e; border: 1px solid #1a3344; border-radius: 8px; }"
    );
    QVBoxLayout* coordCardLayout = new QVBoxLayout(coordCard);
    coordCardLayout->setContentsMargins(16, 8, 16, 8);
    coordCardLayout->setSpacing(2);
    
    QHBoxLayout* latRow = new QHBoxLayout();
    QLabel* latLabel = new QLabel("LAT:", this);
    latLabel->setStyleSheet("font-size: 10px; color: #00d4ff; font-weight: bold;");
    latRow->addWidget(latLabel);
    m_latValue = new QLabel("51.5050", this);
    m_latValue->setStyleSheet("font-size: 12px; color: #ffffff; font-family: 'Consolas', monospace;");
    latRow->addWidget(m_latValue);
    latRow->addStretch();
    coordCardLayout->addLayout(latRow);
    
    QHBoxLayout* lngRow = new QHBoxLayout();
    QLabel* lngLabel = new QLabel("LNG:", this);
    lngLabel->setStyleSheet("font-size: 10px; color: #00d4ff; font-weight: bold;");
    lngRow->addWidget(lngLabel);
    m_lngValue = new QLabel("-0.0900", this);
    m_lngValue->setStyleSheet("font-size: 12px; color: #ffffff; font-family: 'Consolas', monospace;");
    lngRow->addWidget(m_lngValue);
    lngRow->addStretch();
    coordCardLayout->addLayout(lngRow);
    
    QHBoxLayout* zoomRow = new QHBoxLayout();
    QLabel* zoomLabel = new QLabel("ZOOM:", this);
    zoomLabel->setStyleSheet("font-size: 10px; color: #667788; font-weight: bold;");
    zoomRow->addWidget(zoomLabel);
    m_zoomValue = new QLabel("13", this);
    m_zoomValue->setStyleSheet("font-size: 12px; color: #ffffff; font-family: 'Consolas', monospace;");
    zoomRow->addWidget(m_zoomValue);
    zoomRow->addStretch();
    coordCardLayout->addLayout(zoomRow);
    
    cardsLayout->addWidget(coordCard);
    cardsLayout->addStretch();
    
    mapLayout->addWidget(statusCardsWidget);
    
    // Map widget
    m_mapWidget = new MapWidget(this);
    m_mapWidget->setStyleSheet("border: none;");
    mapLayout->addWidget(m_mapWidget, 1);
    
    static_cast<QHBoxLayout*>(layout())->addWidget(mapSection, 1);
}

void TacticalMapPage::setupRightPanel() {
    QWidget* rightPanel = new QWidget(this);
    rightPanel->setFixedWidth(340);
    rightPanel->setStyleSheet("background-color: #0d1a2d; border-left: 1px solid #1a3344;");
    
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(16, 16, 16, 16);
    rightLayout->setSpacing(16);
    
    // Threats header
    QHBoxLayout* threatsHeader = new QHBoxLayout();
    threatsHeader->setSpacing(8);
    
    QLabel* threatIcon = new QLabel("\xE2\x97\x89", this);
    threatIcon->setStyleSheet("font-size: 16px; color: #ff3344;");
    threatsHeader->addWidget(threatIcon);
    
    QLabel* threatTitle = new QLabel("THREATS (2)", this);
    threatTitle->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #ff3344; letter-spacing: 1px;"
    );
    threatsHeader->addWidget(threatTitle);
    threatsHeader->addStretch();
    
    QPushButton* viewAllBtn = new QPushButton("VIEW ALL", this);
    viewAllBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #667788;"
        "   border: none;"
        "   font-size: 11px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { color: #00d4ff; }"
    );
    viewAllBtn->setCursor(Qt::PointingHandCursor);
    connect(viewAllBtn, &QPushButton::clicked, this, &TacticalMapPage::viewAllThreats);
    threatsHeader->addWidget(viewAllBtn);
    
    rightLayout->addLayout(threatsHeader);
    
    // Threat cards scroll area
    m_threatScrollArea = new QScrollArea(this);
    m_threatScrollArea->setWidgetResizable(true);
    m_threatScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_threatScrollArea->setStyleSheet(
        "QScrollArea { background-color: transparent; border: none; }"
    );
    
    QWidget* threatCardsContainer = new QWidget(this);
    threatCardsContainer->setStyleSheet("background-color: transparent;");
    m_threatCardsLayout = new QVBoxLayout(threatCardsContainer);
    m_threatCardsLayout->setContentsMargins(0, 0, 0, 0);
    m_threatCardsLayout->setSpacing(12);
    m_threatCardsLayout->addStretch();
    
    m_threatScrollArea->setWidget(threatCardsContainer);
    rightLayout->addWidget(m_threatScrollArea, 1);
    
    // Recent Alerts section
    QHBoxLayout* alertsHeader = new QHBoxLayout();
    alertsHeader->setSpacing(8);
    
    QLabel* alertIcon = new QLabel("\xE2\x9A\xA0", this);
    alertIcon->setStyleSheet("font-size: 14px; color: #ffffff;");
    alertsHeader->addWidget(alertIcon);
    
    QLabel* alertTitle = new QLabel("RECENT ALERTS", this);
    alertTitle->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #ffffff; letter-spacing: 1px;"
    );
    alertsHeader->addWidget(alertTitle);
    alertsHeader->addStretch();
    
    rightLayout->addLayout(alertsHeader);
    
    // Alerts container
    QWidget* alertsContainer = new QWidget(this);
    alertsContainer->setStyleSheet("background-color: transparent;");
    m_alertsLayout = new QVBoxLayout(alertsContainer);
    m_alertsLayout->setContentsMargins(0, 0, 0, 0);
    m_alertsLayout->setSpacing(8);
    
    // Add sample alerts
    AlertItem* alert1 = new AlertItem(this);
    alert1->setTime("12:33:44 AM");
    alert1->setSeverity("INFO");
    alert1->setMessage("Friendly asset entering monitored airspace");
    m_alertsLayout->addWidget(alert1);
    
    AlertItem* alert2 = new AlertItem(this);
    alert2->setTime("12:33:44 AM");
    alert2->setSeverity("CRITICAL");
    alert2->setMessage("Unauthorized drone detected in Sector 4");
    m_alertsLayout->addWidget(alert2);
    
    rightLayout->addWidget(alertsContainer);
    
    static_cast<QHBoxLayout*>(layout())->addWidget(rightPanel);
}

void TacticalMapPage::setTrackManager(TrackManager* manager) {
    if (m_trackManager) {
        disconnect(m_trackManager, nullptr, this, nullptr);
    }
    
    m_trackManager = manager;
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated,
                this, &TacticalMapPage::onTrackCreated);
        connect(m_trackManager, &TrackManager::trackDropped,
                this, &TacticalMapPage::onTrackDropped);
        connect(m_trackManager, &TrackManager::trackUpdated,
                this, &TacticalMapPage::refreshThreatCards);
    }
}

void TacticalMapPage::setThreatAssessor(ThreatAssessor* assessor) {
    m_threatAssessor = assessor;
}

void TacticalMapPage::refreshThreatCards() {
    if (!m_trackManager) return;
    
    // Update existing cards
    for (auto it = m_threatCards.begin(); it != m_threatCards.end(); ++it) {
        Track* track = m_trackManager->track(it.key());
        if (track) {
            ThreatCard* card = it.value();
            card->setAltitude(track->position().altitude);
            card->setSpeed(track->velocity().speed());
            
            QString classification;
            switch (track->classification()) {
                case TrackClassification::Hostile:
                    card->setStatus("HOSTILE");
                    classification = "Unknown UAS";
                    break;
                case TrackClassification::Friendly:
                    card->setStatus("FRIENDLY");
                    classification = "Friendly Asset";
                    break;
                case TrackClassification::Pending:
                    card->setStatus("PENDING");
                    classification = "Pending ID";
                    break;
                default:
                    card->setStatus("UNKNOWN");
                    classification = "Unknown";
            }
            card->setClassification(classification);
        }
    }
    
    updateStatusCards();
}

void TacticalMapPage::refreshAlerts() {
    // Clear existing alerts and recreate from threat assessor
    // Implementation depends on ThreatAssessor alert data
}

void TacticalMapPage::updateStatusCards() {
    if (!m_trackManager) return;
    
    int threatCount = 0;
    auto tracks = m_trackManager->allTracks();
    for (const auto& track : tracks) {
        if (track->classification() == TrackClassification::Hostile ||
            track->classification() == TrackClassification::Pending) {
            threatCount++;
        }
    }
    
    m_activeThreatsValue->setText(QString::number(threatCount));
}

void TacticalMapPage::updateCoordinates(double lat, double lng, int zoom) {
    m_latValue->setText(QString::number(lat, 'f', 4));
    m_lngValue->setText(QString::number(lng, 'f', 4));
    m_zoomValue->setText(QString::number(zoom));
}

void TacticalMapPage::onThreatCardClicked(const QString& trackId) {
    emit trackSelected(trackId);
}

void TacticalMapPage::onTrackCreated(const QString& trackId) {
    if (!m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) return;
    
    // Create and add new threat card
    ThreatCard* card = createThreatCard(track);
    m_threatCards[trackId] = card;
    
    // Insert before the stretch item
    m_threatCardsLayout->insertWidget(m_threatCardsLayout->count() - 1, card);
    
    updateStatusCards();
}

void TacticalMapPage::onTrackDropped(const QString& trackId) {
    if (m_threatCards.contains(trackId)) {
        ThreatCard* card = m_threatCards.take(trackId);
        m_threatCardsLayout->removeWidget(card);
        card->deleteLater();
    }
    
    updateStatusCards();
}

ThreatCard* TacticalMapPage::createThreatCard(Track* track) {
    ThreatCard* card = new ThreatCard(this);
    card->setTrackId(track->trackId());
    card->setAltitude(track->position().altitude);
    card->setSpeed(track->velocity().speed());
    card->setSource("Sentinel-1");
    
    QString classification;
    switch (track->classification()) {
        case TrackClassification::Hostile:
            card->setStatus("HOSTILE");
            classification = "DJI Mavic 3";
            break;
        case TrackClassification::Friendly:
            card->setStatus("FRIENDLY");
            classification = "Fixed Wing";
            break;
        case TrackClassification::Pending:
            card->setStatus("PENDING");
            classification = "Pending ID";
            break;
        default:
            card->setStatus("UNKNOWN");
            classification = "Unknown";
    }
    card->setClassification(classification);
    
    connect(card, &ThreatCard::clicked,
            this, &TacticalMapPage::onThreatCardClicked);
    
    return card;
}

} // namespace CounterUAS
