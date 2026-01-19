#ifndef QUICKACTIONSPANEL_H
#define QUICKACTIONSPANEL_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

namespace CounterUAS {

/**
 * @brief QuickActionsPanel - A grouped panel for common operations
 * 
 * This panel provides quick access to frequently used actions organized
 * into logical groups for ease of use.
 * 
 * Groups:
 * - Simulation Control: Start/Stop/Pause/Reset
 * - Display: Map/PPI toggle, zoom controls
 * - Recording: Record/Snapshot controls
 * - Engagement: Quick engage actions
 */
class QuickActionsPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit QuickActionsPanel(QWidget* parent = nullptr);
    
    // Simulation state updates
    void setSimulationRunning(bool running);
    void setSimulationPaused(bool paused);
    void setRecordingActive(bool active);
    
    // Display state updates
    void setDisplayMode(int mode); // 0 = Map, 1 = PPI
    
signals:
    // Simulation control signals
    void startSimulation();
    void stopSimulation();
    void pauseSimulation();
    void resetSimulation();
    
    // Display control signals
    void displayModeChanged(int mode);
    void zoomIn();
    void zoomOut();
    void centerView();
    
    // Recording control signals
    void startRecording();
    void stopRecording();
    void takeSnapshot();
    
    // Engagement signals
    void engageSelected();
    void abortEngagement();
    
private:
    void setupUI();
    void createSimulationGroup();
    void createDisplayGroup();
    void createRecordingGroup();
    void createEngagementGroup();
    void updateSimulationButtons();
    
    // Group boxes
    QGroupBox* m_simGroup;
    QGroupBox* m_displayGroup;
    QGroupBox* m_recordGroup;
    QGroupBox* m_engageGroup;
    
    // Simulation controls
    QPushButton* m_startBtn;
    QPushButton* m_stopBtn;
    QPushButton* m_pauseBtn;
    QPushButton* m_resetBtn;
    QLabel* m_simStatusLabel;
    
    // Display controls
    QComboBox* m_displayModeCombo;
    QPushButton* m_zoomInBtn;
    QPushButton* m_zoomOutBtn;
    QPushButton* m_centerBtn;
    
    // Recording controls
    QPushButton* m_recordBtn;
    QPushButton* m_snapshotBtn;
    QLabel* m_recordStatusLabel;
    
    // Engagement controls
    QPushButton* m_engageBtn;
    QPushButton* m_abortBtn;
    
    // State
    bool m_simulationRunning = false;
    bool m_simulationPaused = false;
    bool m_recordingActive = false;
};

} // namespace CounterUAS

#endif // QUICKACTIONSPANEL_H
