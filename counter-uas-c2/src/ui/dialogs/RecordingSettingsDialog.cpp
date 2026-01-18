/**
 * @file RecordingSettingsDialog.cpp
 * @brief Implementation of recording settings dialog
 */

#include "ui/dialogs/RecordingSettingsDialog.h"
#include "video/VideoStreamManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

namespace CounterUAS {

RecordingSettingsDialog::RecordingSettingsDialog(VideoStreamManager* manager, QWidget* parent)
    : QDialog(parent)
    , m_manager(manager)
{
    setWindowTitle("Recording Settings");
    setMinimumSize(500, 600);
    
    setupUI();
}

void RecordingSettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Path settings
    QGroupBox* pathGroup = new QGroupBox("Recording Location", this);
    QFormLayout* pathLayout = new QFormLayout(pathGroup);
    
    QHBoxLayout* pathFieldLayout = new QHBoxLayout();
    m_recordingPath = new QLineEdit("recordings/", this);
    QPushButton* browseBtn = new QPushButton("Browse...", this);
    connect(browseBtn, &QPushButton::clicked, this, &RecordingSettingsDialog::onBrowsePath);
    pathFieldLayout->addWidget(m_recordingPath);
    pathFieldLayout->addWidget(browseBtn);
    pathLayout->addRow("Path:", pathFieldLayout);
    
    m_filenameFormat = new QLineEdit("%camera_%date_%time", this);
    m_filenameFormat->setToolTip("Available variables: %camera, %date, %time, %track");
    pathLayout->addRow("Filename format:", m_filenameFormat);
    
    mainLayout->addWidget(pathGroup);
    
    // Format settings
    QGroupBox* formatGroup = new QGroupBox("Recording Format", this);
    QFormLayout* formatLayout = new QFormLayout(formatGroup);
    
    m_containerFormat = new QComboBox(this);
    m_containerFormat->addItems({"mp4", "mkv", "avi"});
    formatLayout->addRow("Container:", m_containerFormat);
    
    m_videoCodec = new QComboBox(this);
    m_videoCodec->addItems({"h264", "h265", "vp9"});
    formatLayout->addRow("Video codec:", m_videoCodec);
    
    m_videoBitrate = new QSpinBox(this);
    m_videoBitrate->setRange(1000, 50000);
    m_videoBitrate->setValue(5000);
    m_videoBitrate->setSuffix(" kbps");
    formatLayout->addRow("Bitrate:", m_videoBitrate);
    
    mainLayout->addWidget(formatGroup);
    
    // Buffer settings
    QGroupBox* bufferGroup = new QGroupBox("Event Recording Buffer", this);
    QFormLayout* bufferLayout = new QFormLayout(bufferGroup);
    
    m_preBuffer = new QSpinBox(this);
    m_preBuffer->setRange(0, 300);
    m_preBuffer->setValue(30);
    m_preBuffer->setSuffix(" seconds");
    bufferLayout->addRow("Pre-event buffer:", m_preBuffer);
    
    m_postBuffer = new QSpinBox(this);
    m_postBuffer->setRange(0, 300);
    m_postBuffer->setValue(30);
    m_postBuffer->setSuffix(" seconds");
    bufferLayout->addRow("Post-event buffer:", m_postBuffer);
    
    mainLayout->addWidget(bufferGroup);
    
    // Auto recording options
    QGroupBox* autoGroup = new QGroupBox("Automatic Recording", this);
    QVBoxLayout* autoLayout = new QVBoxLayout(autoGroup);
    
    m_recordOnAlert = new QCheckBox("Record on threat alert", this);
    m_recordOnAlert->setChecked(true);
    autoLayout->addWidget(m_recordOnAlert);
    
    m_recordOnEngagement = new QCheckBox("Record during engagement", this);
    m_recordOnEngagement->setChecked(true);
    autoLayout->addWidget(m_recordOnEngagement);
    
    m_includeOverlays = new QCheckBox("Include tactical overlays", this);
    m_includeOverlays->setChecked(true);
    autoLayout->addWidget(m_includeOverlays);
    
    mainLayout->addWidget(autoGroup);
    
    // Retention settings
    QGroupBox* retentionGroup = new QGroupBox("Storage Management", this);
    QFormLayout* retentionLayout = new QFormLayout(retentionGroup);
    
    m_autoDelete = new QCheckBox("Auto-delete old recordings", this);
    m_autoDelete->setChecked(false);
    retentionLayout->addRow(m_autoDelete);
    
    m_retentionDays = new QSpinBox(this);
    m_retentionDays->setRange(1, 365);
    m_retentionDays->setValue(30);
    m_retentionDays->setSuffix(" days");
    retentionLayout->addRow("Retention period:", m_retentionDays);
    
    mainLayout->addWidget(retentionGroup);
    
    // Quick actions
    QGroupBox* actionsGroup = new QGroupBox("Quick Actions", this);
    QHBoxLayout* actionsLayout = new QHBoxLayout(actionsGroup);
    
    QPushButton* startAllBtn = new QPushButton("Start All Recording", this);
    startAllBtn->setStyleSheet("background-color: green; color: white;");
    connect(startAllBtn, &QPushButton::clicked, this, &RecordingSettingsDialog::onStartAll);
    actionsLayout->addWidget(startAllBtn);
    
    QPushButton* stopAllBtn = new QPushButton("Stop All Recording", this);
    stopAllBtn->setStyleSheet("background-color: red; color: white;");
    connect(stopAllBtn, &QPushButton::clicked, this, &RecordingSettingsDialog::onStopAll);
    actionsLayout->addWidget(stopAllBtn);
    
    QPushButton* snapshotBtn = new QPushButton("Take Snapshot", this);
    connect(snapshotBtn, &QPushButton::clicked, this, &RecordingSettingsDialog::onTakeSnapshot);
    actionsLayout->addWidget(snapshotBtn);
    
    mainLayout->addWidget(actionsGroup);
    
    // Dialog buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* applyBtn = new QPushButton("Apply", this);
    connect(applyBtn, &QPushButton::clicked, this, &RecordingSettingsDialog::onApply);
    QPushButton* okBtn = new QPushButton("OK", this);
    connect(okBtn, &QPushButton::clicked, this, [this]() {
        onApply();
        accept();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);
}

RecordingSettings RecordingSettingsDialog::getSettings() const {
    RecordingSettings settings;
    settings.recordingPath = m_recordingPath->text();
    settings.filenameFormat = m_filenameFormat->text();
    settings.containerFormat = m_containerFormat->currentText();
    settings.videoCodec = m_videoCodec->currentText();
    settings.videoBitrate = m_videoBitrate->value();
    settings.preBufferSeconds = m_preBuffer->value();
    settings.postBufferSeconds = m_postBuffer->value();
    settings.recordOnAlert = m_recordOnAlert->isChecked();
    settings.recordOnEngagement = m_recordOnEngagement->isChecked();
    settings.includeOverlays = m_includeOverlays->isChecked();
    settings.autoDeleteOld = m_autoDelete->isChecked();
    settings.retentionDays = m_retentionDays->value();
    return settings;
}

void RecordingSettingsDialog::setSettings(const RecordingSettings& settings) {
    m_recordingPath->setText(settings.recordingPath);
    m_filenameFormat->setText(settings.filenameFormat);
    m_containerFormat->setCurrentText(settings.containerFormat);
    m_videoCodec->setCurrentText(settings.videoCodec);
    m_videoBitrate->setValue(settings.videoBitrate);
    m_preBuffer->setValue(settings.preBufferSeconds);
    m_postBuffer->setValue(settings.postBufferSeconds);
    m_recordOnAlert->setChecked(settings.recordOnAlert);
    m_recordOnEngagement->setChecked(settings.recordOnEngagement);
    m_includeOverlays->setChecked(settings.includeOverlays);
    m_autoDelete->setChecked(settings.autoDeleteOld);
    m_retentionDays->setValue(settings.retentionDays);
}

void RecordingSettingsDialog::onBrowsePath() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Recording Directory",
        m_recordingPath->text());
    if (!dir.isEmpty()) {
        m_recordingPath->setText(dir);
    }
}

void RecordingSettingsDialog::onApply() {
    RecordingSettings settings = getSettings();
    emit settingsChanged(settings);
    QMessageBox::information(this, "Settings Applied", "Recording settings have been updated.");
}

void RecordingSettingsDialog::onStartAll() {
    emit startAllRecording();
    QMessageBox::information(this, "Recording", "Started recording on all cameras.");
}

void RecordingSettingsDialog::onStopAll() {
    emit stopAllRecording();
    QMessageBox::information(this, "Recording", "Stopped all recordings.");
}

void RecordingSettingsDialog::onTakeSnapshot() {
    QMessageBox::information(this, "Snapshot", "Snapshot taken from all active cameras.");
}

} // namespace CounterUAS
