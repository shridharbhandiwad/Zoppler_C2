#include "ui/pages/ThreatDatabasePage.h"
#include "ui/SkyGuardTheme.h"
#include "core/TrackManager.h"
#include "core/Track.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QLabel>

namespace CounterUAS {

ThreatDatabasePage::ThreatDatabasePage(QWidget* parent)
    : QWidget(parent)
    , m_trackManager(nullptr)
    , m_table(nullptr)
{
    setupUI();
}

void ThreatDatabasePage::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(24);
    
    // Header section
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);
    
    // Icon and title
    QLabel* iconLabel = new QLabel("\xE2\x97\x89", this);
    iconLabel->setStyleSheet(
        "font-size: 32px; color: #ff3344; background-color: #111d2e; "
        "padding: 12px; border-radius: 8px;"
    );
    headerLayout->addWidget(iconLabel);
    
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    
    QLabel* titleLabel = new QLabel("THREAT DATABASE", this);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #ffffff; letter-spacing: 2px;"
    );
    titleLayout->addWidget(titleLabel);
    
    QLabel* subtitleLabel = new QLabel("Real-time tracking of all detected aerial objects", this);
    subtitleLabel->setStyleSheet(
        "font-size: 13px; color: #667788;"
    );
    titleLayout->addWidget(subtitleLabel);
    
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    // Simulate threat button
    m_simulateButton = new QPushButton("+ SIMULATE THREAT", this);
    m_simulateButton->setCursor(Qt::PointingHandCursor);
    m_simulateButton->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: 2px solid #00d4ff;"
        "   color: #00d4ff;"
        "   border-radius: 4px;"
        "   padding: 12px 24px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 212, 255, 0.15);"
        "}"
    );
    connect(m_simulateButton, &QPushButton::clicked,
            this, &ThreatDatabasePage::simulateThreatClicked);
    headerLayout->addWidget(m_simulateButton);
    
    mainLayout->addLayout(headerLayout);
    
    // Table
    m_table = new QTableWidget(this);
    m_table->setObjectName("threatTable");
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({
        "UID", "Classification", "Status", "Altitude", "Speed", "Heading", "First Seen"
    });
    
    // Table styling
    m_table->setStyleSheet(
        "QTableWidget {"
        "   background-color: #0d1a2d;"
        "   alternate-background-color: #111d2e;"
        "   border: none;"
        "   gridline-color: transparent;"
        "   selection-background-color: rgba(0, 212, 255, 0.15);"
        "   font-size: 13px;"
        "}"
        "QTableWidget::item {"
        "   padding: 16px 12px;"
        "   border-bottom: 1px solid #1a3344;"
        "   color: #ffffff;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: rgba(0, 212, 255, 0.15);"
        "}"
        "QHeaderView::section {"
        "   background-color: #0d1a2d;"
        "   color: #00d4ff;"
        "   font-weight: bold;"
        "   font-size: 11px;"
        "   padding: 16px 12px;"
        "   border: none;"
        "   border-bottom: 2px solid #1a3344;"
        "   text-transform: uppercase;"
        "}"
    );
    
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    
    // Set column widths
    m_table->setColumnWidth(0, 150);  // UID
    m_table->setColumnWidth(1, 150);  // Classification
    m_table->setColumnWidth(2, 120);  // Status
    m_table->setColumnWidth(3, 100);  // Altitude
    m_table->setColumnWidth(4, 100);  // Speed
    m_table->setColumnWidth(5, 100);  // Heading
    m_table->setColumnWidth(6, 150);  // First Seen
    
    connect(m_table, &QTableWidget::cellClicked,
            this, &ThreatDatabasePage::onTableRowClicked);
    
    mainLayout->addWidget(m_table);
}

void ThreatDatabasePage::setTrackManager(TrackManager* manager) {
    if (m_trackManager) {
        disconnect(m_trackManager, nullptr, this, nullptr);
    }
    
    m_trackManager = manager;
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated,
                this, &ThreatDatabasePage::onTrackCreated);
        connect(m_trackManager, &TrackManager::trackDropped,
                this, &ThreatDatabasePage::onTrackDropped);
        connect(m_trackManager, &TrackManager::trackUpdated,
                this, &ThreatDatabasePage::onTrackUpdated);
        
        refreshTable();
    }
}

void ThreatDatabasePage::refreshTable() {
    m_table->setRowCount(0);
    
    if (!m_trackManager) return;
    
    auto tracks = m_trackManager->allTracks();
    for (auto* track : tracks) {
        addTrackRow(track);
    }
}

void ThreatDatabasePage::addTrackRow(Track* track) {
    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setRowHeight(row, 60);
    
    // UID
    QTableWidgetItem* uidItem = new QTableWidgetItem(track->trackId());
    uidItem->setData(Qt::UserRole, track->trackId());
    uidItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    uidItem->setFont(QFont("Consolas", 12, QFont::Bold));
    m_table->setItem(row, 0, uidItem);
    
    // Classification
    QString classification;
    switch (track->classification()) {
        case TrackClassification::Hostile:
            classification = "DJI Mavic 3";
            break;
        case TrackClassification::Friendly:
            classification = "Fixed Wing";
            break;
        case TrackClassification::Pending:
            classification = "DJI Mavic 3";
            break;
        default:
            classification = "Unknown";
    }
    QTableWidgetItem* classItem = new QTableWidgetItem(classification);
    classItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_table->setItem(row, 1, classItem);
    
    // Status (as widget with badge)
    m_table->setCellWidget(row, 2, createStatusBadge(
        track->classification() == TrackClassification::Hostile ? "HOSTILE" :
        track->classification() == TrackClassification::Friendly ? "FRIENDLY" : "PENDING"
    ));
    
    // Altitude
    QTableWidgetItem* altItem = new QTableWidgetItem(
        QString("%1 m").arg(static_cast<int>(track->position().altitude))
    );
    altItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_table->setItem(row, 3, altItem);
    
    // Speed
    QTableWidgetItem* spdItem = new QTableWidgetItem(
        QString("%1 m/s").arg(track->velocity().speed(), 0, 'f', 1)
    );
    spdItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_table->setItem(row, 4, spdItem);
    
    // Heading
    QTableWidgetItem* hdgItem = new QTableWidgetItem(
        QString("%1\xC2\xB0").arg(static_cast<int>(track->velocity().heading()))
    );
    hdgItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_table->setItem(row, 5, hdgItem);
    
    // First Seen
    QTableWidgetItem* timeItem = new QTableWidgetItem(
        track->createdTime().toString("h:mm:ss AP")
    );
    timeItem->setTextAlignment(Qt::AlignCenter);
    m_table->setItem(row, 6, timeItem);
}

void ThreatDatabasePage::updateTrackRow(const QString& trackId) {
    int row = findRowByTrackId(trackId);
    if (row < 0 || !m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) return;
    
    // Update altitude
    m_table->item(row, 3)->setText(
        QString("%1 m").arg(static_cast<int>(track->position().altitude))
    );
    
    // Update speed
    m_table->item(row, 4)->setText(
        QString("%1 m/s").arg(track->velocity().speed(), 0, 'f', 1)
    );
    
    // Update heading
    m_table->item(row, 5)->setText(
        QString("%1\xC2\xB0").arg(static_cast<int>(track->velocity().heading()))
    );
    
    // Update status badge
    m_table->setCellWidget(row, 2, createStatusBadge(
        track->classification() == TrackClassification::Hostile ? "HOSTILE" :
        track->classification() == TrackClassification::Friendly ? "FRIENDLY" : "PENDING"
    ));
}

int ThreatDatabasePage::findRowByTrackId(const QString& trackId) {
    for (int i = 0; i < m_table->rowCount(); ++i) {
        QTableWidgetItem* item = m_table->item(i, 0);
        if (item && item->data(Qt::UserRole).toString() == trackId) {
            return i;
        }
    }
    return -1;
}

QWidget* ThreatDatabasePage::createStatusBadge(const QString& status) {
    QWidget* container = new QWidget();
    container->setStyleSheet("background-color: transparent;");
    QHBoxLayout* layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    
    QLabel* badge = new QLabel(status, container);
    badge->setAlignment(Qt::AlignCenter);
    
    QString bgColor = "#ffaa00";  // Default pending
    if (status == "HOSTILE") {
        bgColor = "#ff3344";
    } else if (status == "FRIENDLY") {
        bgColor = "#00cc66";
    }
    
    badge->setStyleSheet(QString(
        "background-color: %1; color: #ffffff; font-size: 10px; "
        "font-weight: bold; padding: 6px 14px; border-radius: 12px;"
    ).arg(bgColor));
    badge->setFixedSize(90, 26);
    
    layout->addWidget(badge, 0, Qt::AlignLeft);
    layout->addStretch();
    
    return container;
}

void ThreatDatabasePage::onTableRowClicked(int row, int column) {
    Q_UNUSED(column)
    
    QTableWidgetItem* item = m_table->item(row, 0);
    if (item) {
        QString trackId = item->data(Qt::UserRole).toString();
        emit trackSelected(trackId);
    }
}

void ThreatDatabasePage::onTrackCreated(const QString& trackId) {
    if (!m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (track) {
        addTrackRow(track);
    }
}

void ThreatDatabasePage::onTrackDropped(const QString& trackId) {
    int row = findRowByTrackId(trackId);
    if (row >= 0) {
        m_table->removeRow(row);
    }
}

void ThreatDatabasePage::onTrackUpdated(const QString& trackId) {
    updateTrackRow(trackId);
}

} // namespace CounterUAS
