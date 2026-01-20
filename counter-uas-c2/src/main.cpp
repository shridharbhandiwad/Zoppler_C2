/**
 * ZOPPLER - Counter-UAS Command & Control System
 * 
 * A real-time command and control application for Counter-UAS system
 * protecting military installations.
 * 
 * Features:
 * - Multi-sensor integration (Radar, RF, Camera)
 * - Track management with Kalman filtering
 * - Threat assessment and prioritization
 * - Video feed management with overlays
 * - Effector control (RF Jammer, Kinetic, Directed Energy)
 * - Modern tactical interface with ZOPPLER theme
 */

#include <QApplication>
#include <QStyleFactory>
#include <QSurfaceFormat>
#include "ui/SkyGuardMainWindow.h"
#include "ui/SkyGuardTheme.h"
#include "config/ConfigManager.h"
#include "config/DatabaseManager.h"
#include "utils/Logger.h"
#include "simulators/TrackSimulator.h"

using namespace CounterUAS;

int main(int argc, char *argv[])
{
    // Set OpenGL format for video rendering
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    
    QApplication app(argc, argv);
    app.setApplicationName("ZOPPLER C-UAS Command System");
    app.setApplicationVersion("2.4.1-ALPHA");
    app.setOrganizationName("Defense Systems");
    
    // Set dark fusion style as base
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Apply Zoppler dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(10, 22, 40));         // #0a1628
    darkPalette.setColor(QPalette::WindowText, QColor(255, 255, 255));  // #ffffff
    darkPalette.setColor(QPalette::Base, QColor(13, 26, 45));           // #0d1a2d
    darkPalette.setColor(QPalette::AlternateBase, QColor(17, 29, 46));  // #111d2e
    darkPalette.setColor(QPalette::ToolTipBase, QColor(26, 45, 68));    // #1a2d44
    darkPalette.setColor(QPalette::ToolTipText, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::Button, QColor(26, 45, 68));         // #1a2d44
    darkPalette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::BrightText, QColor(0, 212, 255));    // #00d4ff
    darkPalette.setColor(QPalette::Link, QColor(0, 212, 255));          // #00d4ff
    darkPalette.setColor(QPalette::Highlight, QColor(0, 212, 255));     // #00d4ff
    darkPalette.setColor(QPalette::HighlightedText, QColor(10, 22, 40));
    app.setPalette(darkPalette);
    
    // Initialize logger
    Logger::instance().setLogLevel(LogLevel::Debug);
    Logger::instance().setLogToConsole(true);
    Logger::instance().info("Main", "ZOPPLER C-UAS Command System starting...");
    
    // Load configuration
    ConfigManager::instance().loadDefaults();
    
    // Initialize database
    DatabaseManager::instance().initialize("data/counter_uas.db");
    
    // Create and show Zoppler main window
    SkyGuardMainWindow mainWindow;
    mainWindow.show();
    
    // Create track simulator for testing with enhanced drone simulation
    TrackSimulator simulator(mainWindow.trackManager());
    simulator.setMaxTargets(15);           // Allow more concurrent drones
    simulator.setSpawnInterval(3000);      // Spawn every 3 seconds
    simulator.setAutoSpawnEnabled(true);
    simulator.start();
    
    // Inject some initial drones for immediate display
    ManualTargetParams drone1;
    drone1.rangeM = 2000.0;
    drone1.bearingDeg = 45.0;
    drone1.altitudeM = 150.0;
    drone1.speedMps = 12.0;
    drone1.headingDeg = 225.0;  // Heading toward base
    drone1.classification = TrackClassification::Hostile;
    simulator.injectTarget(drone1);
    
    ManualTargetParams drone2;
    drone2.rangeM = 1800.0;
    drone2.bearingDeg = 120.0;
    drone2.altitudeM = 80.0;
    drone2.speedMps = 18.0;
    drone2.headingDeg = 300.0;
    drone2.classification = TrackClassification::Hostile;
    simulator.injectTarget(drone2);
    
    ManualTargetParams drone3;
    drone3.rangeM = 2500.0;
    drone3.bearingDeg = 270.0;
    drone3.altitudeM = 200.0;
    drone3.speedMps = 10.0;
    drone3.headingDeg = 90.0;
    drone3.classification = TrackClassification::Pending;
    simulator.injectTarget(drone3);
    
    // Start simulation automatically for demo
    mainWindow.startSimulation();
    
    Logger::instance().info("Main", "ZOPPLER system initialized successfully");
    
    int result = app.exec();
    
    // Cleanup
    simulator.stop();
    DatabaseManager::instance().close();
    
    Logger::instance().info("Main", "ZOPPLER system shutdown complete");
    
    return result;
}
