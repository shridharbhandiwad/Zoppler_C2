#ifndef TRACKLISTWIDGET_H
#define TRACKLISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include "core/Track.h"

namespace CounterUAS {

class TrackManager;

class TrackListWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit TrackListWidget(TrackManager* trackManager, QWidget* parent = nullptr);
    
    // Set reference position for range calculation (typically the defended asset)
    void setReferencePosition(const GeoPosition& pos);
    GeoPosition referencePosition() const { return m_referencePosition; }
    
signals:
    void trackSelected(const QString& trackId);
    void trackDoubleClicked(const QString& trackId);
    
private slots:
    void onTrackCreated(const QString& trackId);
    void onTrackUpdated(const QString& trackId);
    void onTrackDropped(const QString& trackId);
    void onSelectionChanged();
    
private:
    void updateTrackRow(const QString& trackId);
    int findTrackRow(const QString& trackId);
    QString formatRange(double rangeMeters) const;
    QString formatAzimuth(double azimuthDegrees) const;
    QString formatElevation(double elevationDegrees) const;
    QString formatVelocity(double velocityMps) const;
    double calculateAzimuth(const GeoPosition& from, const GeoPosition& to) const;
    double calculateElevation(const GeoPosition& from, const GeoPosition& to) const;
    
    TrackManager* m_trackManager;
    QTableView* m_tableView;
    QStandardItemModel* m_model;
    GeoPosition m_referencePosition;  // Reference point for range calculation
};

} // namespace CounterUAS

#endif // TRACKLISTWIDGET_H
