#-------------------------------------------------
#
# Project: Counter-UAS Command and Control System
# Qt Version: 5.15.2
#
#-------------------------------------------------

QT       += core gui widgets network sql multimedia multimediawidgets concurrent serialport

# Qt 5.15.2 specific modules
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Optional modules (uncomment if available)
# QT += positioning location

TARGET = CounterUAS_C2
TEMPLATE = app

# C++17 standard
CONFIG += c++17

# Compiler warnings
CONFIG += warn_on

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated
DEFINES += QT_DEPRECATED_WARNINGS

# Windows specific settings - create GUI application (not console)
win32 {
    CONFIG += windows
    # Note: Uncomment RC_ICONS when app.ico is added to resources/
    # RC_ICONS = resources/app.ico
}

# Include directories
INCLUDEPATH += src

# Core module sources
SOURCES += \
    src/main.cpp \
    src/core/Track.cpp \
    src/core/TrackManager.cpp \
    src/core/ThreatAssessor.cpp \
    src/core/EngagementManager.cpp

# Sensor module sources
SOURCES += \
    src/sensors/SensorInterface.cpp \
    src/sensors/RadarSensor.cpp \
    src/sensors/RFDetector.cpp \
    src/sensors/CameraSystem.cpp

# Video module sources
SOURCES += \
    src/video/VideoStreamManager.cpp \
    src/video/VideoSource.cpp \
    src/video/RTSPVideoSource.cpp \
    src/video/GigEVideoSource.cpp \
    src/video/FileVideoSource.cpp \
    src/video/SimulationVideoSource.cpp \
    src/video/VideoDecoder.cpp \
    src/video/VideoOverlayRenderer.cpp \
    src/video/VideoRecorder.cpp \
    src/video/PTZController.cpp \
    src/video/CameraSlewController.cpp

# Effector module sources
SOURCES += \
    src/effectors/EffectorInterface.cpp \
    src/effectors/RFJammer.cpp \
    src/effectors/KineticInterceptor.cpp \
    src/effectors/DirectedEnergySystem.cpp

# Network module sources
SOURCES += \
    src/network/NetworkManager.cpp \
    src/network/MessageProtocol.cpp

# UI module sources
SOURCES += \
    src/ui/MainWindow.cpp \
    src/ui/MapWidget.cpp \
    src/ui/PPIDisplayWidget.cpp \
    src/ui/VideoDisplayWidget.cpp \
    src/ui/VideoGridWidget.cpp \
    src/ui/VideoControlBar.cpp \
    src/ui/PTZControlWidget.cpp \
    src/ui/TrackListWidget.cpp \
    src/ui/TrackDetailPanel.cpp \
    src/ui/SensorStatusPanel.cpp \
    src/ui/CameraStatusPanel.cpp \
    src/ui/EffectorControlPanel.cpp \
    src/ui/AlertQueue.cpp \
    src/ui/EngagementDialog.cpp

# Config module sources
SOURCES += \
    src/config/ConfigManager.cpp \
    src/config/CameraConfig.cpp \
    src/config/DatabaseManager.cpp

# Utils module sources
SOURCES += \
    src/utils/CoordinateUtils.cpp \
    src/utils/TimeUtils.cpp \
    src/utils/FrameBuffer.cpp \
    src/utils/Logger.cpp \
    src/utils/KalmanFilter.cpp

# Simulator module sources
SOURCES += \
    src/simulators/SensorSimulator.cpp \
    src/simulators/TrackSimulator.cpp \
    src/simulators/VideoSimulator.cpp \
    src/simulators/EffectorSimulator.cpp \
    src/simulators/SystemSimulationManager.cpp

# Dialog sources
SOURCES += \
    src/ui/dialogs/SensorConfigDialog.cpp \
    src/ui/dialogs/EffectorStatusDialog.cpp \
    src/ui/dialogs/SimulationSettingsDialog.cpp \
    src/ui/dialogs/RulesOfEngagementDialog.cpp \
    src/ui/dialogs/RecordingSettingsDialog.cpp

# Core module headers
HEADERS += \
    src/core/Track.h \
    src/core/TrackManager.h \
    src/core/ThreatAssessor.h \
    src/core/EngagementManager.h

# Sensor module headers
HEADERS += \
    src/sensors/SensorInterface.h \
    src/sensors/RadarSensor.h \
    src/sensors/RFDetector.h \
    src/sensors/CameraSystem.h

# Video module headers
HEADERS += \
    src/video/VideoStreamManager.h \
    src/video/VideoSource.h \
    src/video/RTSPVideoSource.h \
    src/video/GigEVideoSource.h \
    src/video/FileVideoSource.h \
    src/video/SimulationVideoSource.h \
    src/video/VideoDecoder.h \
    src/video/VideoOverlayRenderer.h \
    src/video/VideoRecorder.h \
    src/video/PTZController.h \
    src/video/CameraSlewController.h

# Effector module headers
HEADERS += \
    src/effectors/EffectorInterface.h \
    src/effectors/RFJammer.h \
    src/effectors/KineticInterceptor.h \
    src/effectors/DirectedEnergySystem.h

# Network module headers
HEADERS += \
    src/network/NetworkManager.h \
    src/network/MessageProtocol.h

# UI module headers
HEADERS += \
    src/ui/MainWindow.h \
    src/ui/MapWidget.h \
    src/ui/PPIDisplayWidget.h \
    src/ui/VideoDisplayWidget.h \
    src/ui/VideoGridWidget.h \
    src/ui/VideoControlBar.h \
    src/ui/PTZControlWidget.h \
    src/ui/TrackListWidget.h \
    src/ui/TrackDetailPanel.h \
    src/ui/SensorStatusPanel.h \
    src/ui/CameraStatusPanel.h \
    src/ui/EffectorControlPanel.h \
    src/ui/AlertQueue.h \
    src/ui/EngagementDialog.h

# Config module headers
HEADERS += \
    src/config/ConfigManager.h \
    src/config/CameraConfig.h \
    src/config/DatabaseManager.h

# Utils module headers
HEADERS += \
    src/utils/CoordinateUtils.h \
    src/utils/TimeUtils.h \
    src/utils/FrameBuffer.h \
    src/utils/Logger.h \
    src/utils/KalmanFilter.h

# Simulator module headers
HEADERS += \
    src/simulators/SensorSimulator.h \
    src/simulators/TrackSimulator.h \
    src/simulators/VideoSimulator.h \
    src/simulators/EffectorSimulator.h \
    src/simulators/SystemSimulationManager.h

# Dialog headers
HEADERS += \
    src/ui/dialogs/SensorConfigDialog.h \
    src/ui/dialogs/EffectorStatusDialog.h \
    src/ui/dialogs/SimulationSettingsDialog.h \
    src/ui/dialogs/RulesOfEngagementDialog.h \
    src/ui/dialogs/RecordingSettingsDialog.h

# Resources
RESOURCES += \
    resources/resources.qrc

# Copy config files to build directory
CONFIG_FILES = \
    config/cameras.json \
    config/defended_assets.json \
    config/effectors.json \
    config/sensors.json \
    config/system.json

# Post-build copy for config files
win32 {
    QMAKE_POST_LINK += $$quote(if not exist \"$$OUT_PWD/config\" mkdir \"$$OUT_PWD/config\"$$escape_expand(\n\t))
    for(FILE, CONFIG_FILES) {
        QMAKE_POST_LINK += $$quote(copy /Y \"$$PWD/$$FILE\" \"$$OUT_PWD/config/\"$$escape_expand(\n\t))
    }
}
unix {
    QMAKE_POST_LINK += $$quote(mkdir -p $$OUT_PWD/config && cp -r $$PWD/config/* $$OUT_PWD/config/)
}

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Release/Debug configuration
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

# Platform-specific libraries
win32 {
    LIBS += -lopengl32
}
unix:!macx {
    LIBS += -lGL
}
macx {
    LIBS += -framework OpenGL
}
