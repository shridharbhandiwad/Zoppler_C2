#include "ui/TrackListWidget.h"
#include "core/TrackManager.h"
#include <QVBoxLayout>
#include <QHeaderView>

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
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_tableView);
    
    m_model->setHorizontalHeaderLabels({"ID", "Class", "Threat", "Range", "Status"});
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->verticalHeader()->hide();
    
    connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &TrackListWidget::onSelectionChanged);
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated, this, &TrackListWidget::onTrackCreated);
        connect(m_trackManager, &TrackManager::trackUpdated, this, &TrackListWidget::onTrackUpdated);
        connect(m_trackManager, &TrackManager::trackDropped, this, &TrackListWidget::onTrackDropped);
    }
}

void TrackListWidget::setReferencePosition(const GeoPosition& pos) {
    m_referencePosition = pos;
    
    // Update range for all existing tracks
    for (int i = 0; i < m_model->rowCount(); ++i) {
        QString trackId = m_model->item(i, 0)->text();
        Track* track = m_trackManager->track(trackId);
        if (track) {
            double range = track->distanceTo(m_referencePosition);
            m_model->item(i, 3)->setText(formatRange(range));
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

void TrackListWidget::onTrackCreated(const QString& trackId) {
    Track* track = m_trackManager->track(trackId);
    if (!track) return;
    
    // Calculate range from reference position
    double range = track->distanceTo(m_referencePosition);
    
    QList<QStandardItem*> row;
    row << new QStandardItem(trackId);
    row << new QStandardItem(track->classificationString());
    row << new QStandardItem(QString::number(track->threatLevel()));
    row << new QStandardItem(formatRange(range));
    row << new QStandardItem(track->stateString());
    
    m_model->appendRow(row);
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
    
    // Calculate range from reference position
    double range = track->distanceTo(m_referencePosition);
    
    m_model->item(row, 1)->setText(track->classificationString());
    m_model->item(row, 2)->setText(QString::number(track->threatLevel()));
    m_model->item(row, 3)->setText(formatRange(range));
    m_model->item(row, 4)->setText(track->stateString());
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
