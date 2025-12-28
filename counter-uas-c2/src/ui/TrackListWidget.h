#ifndef TRACKLISTWIDGET_H
#define TRACKLISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

namespace CounterUAS {

class TrackManager;

class TrackListWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit TrackListWidget(TrackManager* trackManager, QWidget* parent = nullptr);
    
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
    
    TrackManager* m_trackManager;
    QTableView* m_tableView;
    QStandardItemModel* m_model;
};

} // namespace CounterUAS

#endif // TRACKLISTWIDGET_H
