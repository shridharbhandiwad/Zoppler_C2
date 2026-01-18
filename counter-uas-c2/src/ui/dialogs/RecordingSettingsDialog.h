/**
 * @file RecordingSettingsDialog.h
 * @brief Dialog for video recording settings
 */

#ifndef RECORDINGSETTINGSDIALOG_H
#define RECORDINGSETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>

namespace CounterUAS {

class VideoStreamManager;

/**
 * @brief Recording settings configuration
 */
struct RecordingSettings {
    QString recordingPath = "recordings/";
    QString filenameFormat = "%camera_%date_%time";
    QString containerFormat = "mp4";
    QString videoCodec = "h264";
    int videoBitrate = 5000;  // kbps
    int preBufferSeconds = 30;
    int postBufferSeconds = 30;
    bool recordOnAlert = true;
    bool recordOnEngagement = true;
    bool includeOverlays = true;
    bool autoDeleteOld = false;
    int retentionDays = 30;
};

/**
 * @brief Dialog for configuring video recording
 */
class RecordingSettingsDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit RecordingSettingsDialog(VideoStreamManager* manager, QWidget* parent = nullptr);
    
    RecordingSettings getSettings() const;
    void setSettings(const RecordingSettings& settings);
    
signals:
    void settingsChanged(const RecordingSettings& settings);
    void startAllRecording();
    void stopAllRecording();
    
private slots:
    void onBrowsePath();
    void onApply();
    void onStartAll();
    void onStopAll();
    void onTakeSnapshot();
    
private:
    void setupUI();
    
    VideoStreamManager* m_manager;
    
    // Path settings
    QLineEdit* m_recordingPath;
    QLineEdit* m_filenameFormat;
    
    // Format settings
    QComboBox* m_containerFormat;
    QComboBox* m_videoCodec;
    QSpinBox* m_videoBitrate;
    
    // Buffer settings
    QSpinBox* m_preBuffer;
    QSpinBox* m_postBuffer;
    
    // Auto recording
    QCheckBox* m_recordOnAlert;
    QCheckBox* m_recordOnEngagement;
    QCheckBox* m_includeOverlays;
    
    // Retention
    QCheckBox* m_autoDelete;
    QSpinBox* m_retentionDays;
    
    // Camera list
    QListWidget* m_cameraList;
};

} // namespace CounterUAS

#endif // RECORDINGSETTINGSDIALOG_H
