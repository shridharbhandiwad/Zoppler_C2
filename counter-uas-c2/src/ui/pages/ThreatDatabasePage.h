#ifndef THREATDATABASEPAGE_H
#define THREATDATABASEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace CounterUAS {

class TrackManager;
class Track;

/**
 * ThreatDatabasePage - Threat List/Database view
 * 
 * Shows a table of all detected aerial objects with:
 * - UID
 * - Classification
 * - Status (HOSTILE/FRIENDLY)
 * - Altitude
 * - Speed
 * - Heading
 * - First Seen timestamp
 */
class ThreatDatabasePage : public QWidget {
    Q_OBJECT
    
public:
    explicit ThreatDatabasePage(QWidget* parent = nullptr);
    ~ThreatDatabasePage() override = default;
    
    void setTrackManager(TrackManager* manager);
    
public slots:
    void refreshTable();
    
signals:
    void trackSelected(const QString& trackId);
    void simulateThreatClicked();
    
private slots:
    void onTableRowClicked(int row, int column);
    void onTrackCreated(const QString& trackId);
    void onTrackDropped(const QString& trackId);
    void onTrackUpdated(const QString& trackId);
    
private:
    void setupUI();
    void addTrackRow(Track* track);
    void updateTrackRow(const QString& trackId);
    int findRowByTrackId(const QString& trackId);
    QWidget* createStatusBadge(const QString& status);
    
    TrackManager* m_trackManager;
    QTableWidget* m_table;
    QPushButton* m_simulateButton;
};

} // namespace CounterUAS

#endif // THREATDATABASEPAGE_H
