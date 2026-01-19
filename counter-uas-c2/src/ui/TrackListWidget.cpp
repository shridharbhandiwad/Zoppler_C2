#include "ui/TrackListWidget.h"
#include "core/TrackManager.h"
#include "utils/Logger.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QBrush>
#include <QtMath>

namespace CounterUAS {

TrackListWidget::TrackListWidget(TrackManager* trackManager, QWidget* parent)
    : QWidget(parent)
    , m_trackManager(trackManager)
    , m_tableView(new QTableView(this))
    , m_model(new QStandardItemModel(this))
{
    // Initialize default reference position (will be set by MainWindow)
    m_referencePosition.latitude = 0.0;
    m_referencePosition.longitude = 0.0;
    m_referencePosition.altitude = 0.0;
    
    // Set minimum size for visibility
    setMinimumSize(300, 120);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);
    
    // Add title label
    QLabel* titleLabel = new QLabel("Active Tracks", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 12px; color: #ccc; "
                             "background-color: #2a2a2a; padding: 4px; border-radius: 3px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    layout->addWidget(m_tableView);
    
    m_model->setHorizontalHeaderLabels({"ID", "Class", "Threat", "Range", "Azimuth", "Elevation", "Velocity", "Status"});
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->verticalHeader()->hide();
    
    // Configure header for better column sizing
    QHeaderView* header = m_tableView->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);  // ID
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);  // Class
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);  // Threat
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);  // Range
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);  // Azimuth
    header->setSectionResizeMode(5, QHeaderView::ResizeToContents);  // Elevation
    header->setSectionResizeMode(6, QHeaderView::ResizeToContents);  // Velocity
    header->setSectionResizeMode(7, QHeaderView::Stretch);           // Status
    
    // Style the table
    m_tableView->setStyleSheet(
        "QTableView { "
        "   background-color: #1a1a1a; "
        "   alternate-background-color: #252525; "
        "   color: #ddd; "
        "   gridline-color: #333; "
        "   selection-background-color: #3a6090; "
        "   selection-color: white; "
        "} "
        "QHeaderView::section { "
        "   background-color: #333; "
        "   color: #ccc; "
        "   padding: 4px; "
        "   border: 1px solid #444; "
        "} "
    );
    
    connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &TrackListWidget::onSelectionChanged);
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated, this, &TrackListWidget::onTrackCreated);
        connect(m_trackManager, &TrackManager::trackUpdated, this, &TrackListWidget::onTrackUpdated);
        connect(m_trackManager, &TrackManager::trackDropped, this, &TrackListWidget::onTrackDropped);
        Logger::instance().info("TrackListWidget", "Connected to TrackManager signals");
    } else {
        Logger::instance().warning("TrackListWidget", "No TrackManager provided");
    }
}

void TrackListWidget::setReferencePosition(const GeoPosition& pos) {
    m_referencePosition = pos;
    
    // Update range, azimuth, and elevation for all existing tracks
    for (int i = 0; i < m_model->rowCount(); ++i) {
        QString trackId = m_model->item(i, 0)->text();
        Track* track = m_trackManager->track(trackId);
        if (track) {
            GeoPosition trackPos = track->position();
            double range = track->distanceTo(m_referencePosition);
            double azimuth = calculateAzimuth(m_referencePosition, trackPos);
            double elevation = calculateElevation(m_referencePosition, trackPos);
            m_model->item(i, 3)->setText(formatRange(range));
            m_model->item(i, 4)->setText(formatAzimuth(azimuth));
            m_model->item(i, 5)->setText(formatElevation(elevation));
        }
    }
}

QString TrackListWidget::formatRange(double rangeMeters) const {
    if (rangeMeters < 1000.0) {
        return QString("%1 m").arg(rangeMeters, 0, 'f', 0);
    } else {
        return QString("%1 km").arg(rangeMeters / 1000.0, 0, 'f', 2);
    }
}

QString TrackListWidget::formatAzimuth(double azimuthDegrees) const {
    return QString("%1\u00B0").arg(azimuthDegrees, 0, 'f', 1);  // Unicode degree symbol
}

QString TrackListWidget::formatElevation(double elevationDegrees) const {
    return QString("%1\u00B0").arg(elevationDegrees, 0, 'f', 1);  // Unicode degree symbol
}

QString TrackListWidget::formatVelocity(double velocityMps) const {
    if (velocityMps < 1.0) {
        return QString("%1 m/s").arg(velocityMps, 0, 'f', 2);
    } else {
        return QString("%1 m/s").arg(velocityMps, 0, 'f', 1);
    }
}

double TrackListWidget::calculateAzimuth(const GeoPosition& from, const GeoPosition& to) const {
    // Calculate bearing from reference position to track
    double lat1 = qDegreesToRadians(from.latitude);
    double lat2 = qDegreesToRadians(to.latitude);
    double dLon = qDegreesToRadians(to.longitude - from.longitude);
    
    double y = std::sin(dLon) * std::cos(lat2);
    double x = std::cos(lat1) * std::sin(lat2) -
               std::sin(lat1) * std::cos(lat2) * std::cos(dLon);
    
    double bearing = qRadiansToDegrees(std::atan2(y, x));
    if (bearing < 0) bearing += 360.0;
    
    return bearing;
}

double TrackListWidget::calculateElevation(const GeoPosition& from, const GeoPosition& to) const {
    // Calculate horizontal distance using Haversine formula
    const double R = 6371000.0; // Earth radius in meters
    
    double lat1 = qDegreesToRadians(from.latitude);
    double lat2 = qDegreesToRadians(to.latitude);
    double dLat = lat2 - lat1;
    double dLon = qDegreesToRadians(to.longitude - from.longitude);
    
    double a = std::sin(dLat/2) * std::sin(dLat/2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon/2) * std::sin(dLon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    double horizontalDist = R * c;
    double verticalDist = to.altitude - from.altitude;
    
    // Calculate elevation angle (positive is above horizon, negative is below)
    if (horizontalDist < 0.001) {  // Very close, avoid division by near-zero
        return verticalDist > 0 ? 90.0 : (verticalDist < 0 ? -90.0 : 0.0);
    }
    
    return qRadiansToDegrees(std::atan2(verticalDist, horizontalDist));
}

void TrackListWidget::onTrackCreated(const QString& trackId) {
    Track* track = m_trackManager->track(trackId);
    if (!track) {
        Logger::instance().warning("TrackListWidget", 
            QString("onTrackCreated: Track %1 not found").arg(trackId));
        return;
    }
    
    // Calculate range, azimuth, and elevation from reference position
    GeoPosition trackPos = track->position();
    double range = track->distanceTo(m_referencePosition);
    double azimuth = calculateAzimuth(m_referencePosition, trackPos);
    double elevation = calculateElevation(m_referencePosition, trackPos);
    double velocity = track->velocity().speed();
    
    QList<QStandardItem*> row;
    
    // ID column
    QStandardItem* idItem = new QStandardItem(trackId);
    idItem->setData(trackId, Qt::UserRole);  // Store track ID for lookup
    row << idItem;
    
    // Classification column with color coding
    QStandardItem* classItem = new QStandardItem(track->classificationString());
    TrackClassification cls = track->classification();
    if (cls == TrackClassification::Hostile) {
        classItem->setForeground(QColor(255, 80, 80));  // Red for hostile
    } else if (cls == TrackClassification::Friendly) {
        classItem->setForeground(QColor(80, 200, 80));  // Green for friendly
    } else if (cls == TrackClassification::Neutral) {
        classItem->setForeground(QColor(200, 200, 80)); // Yellow for neutral
    } else {
        classItem->setForeground(QColor(180, 180, 180)); // Gray for pending/unknown
    }
    row << classItem;
    
    // Threat level column with color coding
    int threatLevel = track->threatLevel();
    QStandardItem* threatItem = new QStandardItem(QString::number(threatLevel));
    if (threatLevel >= 4) {
        threatItem->setForeground(QColor(255, 50, 50));   // High threat - red
        threatItem->setBackground(QColor(80, 20, 20));
    } else if (threatLevel >= 3) {
        threatItem->setForeground(QColor(255, 150, 50));  // Medium threat - orange
    } else if (threatLevel >= 2) {
        threatItem->setForeground(QColor(255, 255, 80));  // Low threat - yellow
    } else {
        threatItem->setForeground(QColor(150, 150, 150)); // Minimal threat - gray
    }
    threatItem->setTextAlignment(Qt::AlignCenter);
    row << threatItem;
    
    // Range column
    QStandardItem* rangeItem = new QStandardItem(formatRange(range));
    rangeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row << rangeItem;
    
    // Azimuth column
    QStandardItem* azimuthItem = new QStandardItem(formatAzimuth(azimuth));
    azimuthItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row << azimuthItem;
    
    // Elevation column
    QStandardItem* elevationItem = new QStandardItem(formatElevation(elevation));
    elevationItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row << elevationItem;
    
    // Velocity column
    QStandardItem* velocityItem = new QStandardItem(formatVelocity(velocity));
    velocityItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row << velocityItem;
    
    // Status column with color coding
    QStandardItem* statusItem = new QStandardItem(track->stateString());
    TrackState state = track->state();
    if (state == TrackState::Active) {
        statusItem->setForeground(QColor(80, 200, 80));   // Green for active
    } else if (state == TrackState::Coasting) {
        statusItem->setForeground(QColor(200, 200, 80)); // Yellow for coasting
    } else if (state == TrackState::Initiated) {
        statusItem->setForeground(QColor(100, 150, 255)); // Blue for initiated
    } else {
        statusItem->setForeground(QColor(120, 120, 120)); // Gray for dropped
    }
    row << statusItem;
    
    m_model->appendRow(row);
    
    Logger::instance().debug("TrackListWidget", 
        QString("Added track %1 to list (Class: %2, Threat: %3, Range: %4, Az: %5, El: %6, Vel: %7)")
            .arg(trackId)
            .arg(track->classificationString())
            .arg(threatLevel)
            .arg(formatRange(range))
            .arg(formatAzimuth(azimuth))
            .arg(formatElevation(elevation))
            .arg(formatVelocity(velocity)));
}

void TrackListWidget::onTrackUpdated(const QString& trackId) {
    updateTrackRow(trackId);
}

void TrackListWidget::onTrackDropped(const QString& trackId) {
    int row = findTrackRow(trackId);
    if (row >= 0) {
        m_model->removeRow(row);
    }
}

void TrackListWidget::onSelectionChanged() {
    QModelIndexList selected = m_tableView->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        QString trackId = m_model->item(selected.first().row(), 0)->text();
        emit trackSelected(trackId);
    }
}

void TrackListWidget::updateTrackRow(const QString& trackId) {
    int row = findTrackRow(trackId);
    if (row < 0) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) return;
    
    // Calculate range, azimuth, elevation, and velocity from reference position
    GeoPosition trackPos = track->position();
    double range = track->distanceTo(m_referencePosition);
    double azimuth = calculateAzimuth(m_referencePosition, trackPos);
    double elevation = calculateElevation(m_referencePosition, trackPos);
    double velocity = track->velocity().speed();
    
    // Update classification with color coding
    QStandardItem* classItem = m_model->item(row, 1);
    classItem->setText(track->classificationString());
    TrackClassification cls = track->classification();
    if (cls == TrackClassification::Hostile) {
        classItem->setForeground(QColor(255, 80, 80));
    } else if (cls == TrackClassification::Friendly) {
        classItem->setForeground(QColor(80, 200, 80));
    } else if (cls == TrackClassification::Neutral) {
        classItem->setForeground(QColor(200, 200, 80));
    } else {
        classItem->setForeground(QColor(180, 180, 180));
    }
    
    // Update threat level with color coding
    int threatLevel = track->threatLevel();
    QStandardItem* threatItem = m_model->item(row, 2);
    threatItem->setText(QString::number(threatLevel));
    if (threatLevel >= 4) {
        threatItem->setForeground(QColor(255, 50, 50));
        threatItem->setBackground(QColor(80, 20, 20));
    } else if (threatLevel >= 3) {
        threatItem->setForeground(QColor(255, 150, 50));
        threatItem->setBackground(QBrush());  // Clear background
    } else if (threatLevel >= 2) {
        threatItem->setForeground(QColor(255, 255, 80));
        threatItem->setBackground(QBrush());
    } else {
        threatItem->setForeground(QColor(150, 150, 150));
        threatItem->setBackground(QBrush());
    }
    
    // Update range
    m_model->item(row, 3)->setText(formatRange(range));
    
    // Update azimuth
    m_model->item(row, 4)->setText(formatAzimuth(azimuth));
    
    // Update elevation
    m_model->item(row, 5)->setText(formatElevation(elevation));
    
    // Update velocity
    m_model->item(row, 6)->setText(formatVelocity(velocity));
    
    // Update status with color coding
    QStandardItem* statusItem = m_model->item(row, 7);
    statusItem->setText(track->stateString());
    TrackState state = track->state();
    if (state == TrackState::Active) {
        statusItem->setForeground(QColor(80, 200, 80));
    } else if (state == TrackState::Coasting) {
        statusItem->setForeground(QColor(200, 200, 80));
    } else if (state == TrackState::Initiated) {
        statusItem->setForeground(QColor(100, 150, 255));
    } else {
        statusItem->setForeground(QColor(120, 120, 120));
    }
}

int TrackListWidget::findTrackRow(const QString& trackId) {
    for (int i = 0; i < m_model->rowCount(); ++i) {
        if (m_model->item(i, 0)->text() == trackId) {
            return i;
        }
    }
    return -1;
}

} // namespace CounterUAS
