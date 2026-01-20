/**
 * SKYGUARD - Counter-UAS Command & Control System
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
 * - Modern tactical interface with SKYGUARD theme
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
    app.setApplicationName("SKYGUARD C-UAS Command System");
    app.setApplicationVersion("2.4.1-ALPHA");
    app.setOrganizationName("Defense Systems");
    
    // Set dark fusion style as base
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Apply SkyGuard dark palette
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
    Logger::instance().info("Main", "SKYGUARD C-UAS Command System starting...");
    
    // Load configuration
    ConfigManager::instance().loadDefaults();
    
    // Initialize database
    DatabaseManager::instance().initialize("data/counter_uas.db");
    
    // Create and show SkyGuard main window
    SkyGuardMainWindow mainWindow;
    mainWindow.show();
    
    // Create track simulator for testing
    TrackSimulator simulator(mainWindow.trackManager());
    simulator.start();
    
    // Start simulation automatically for demo
    mainWindow.startSimulation();
    
    Logger::instance().info("Main", "SKYGUARD system initialized successfully");
    
    int result = app.exec();
    
    // Cleanup
    simulator.stop();
    DatabaseManager::instance().close();
    
    Logger::instance().info("Main", "SKYGUARD system shutdown complete");
    
    return result;
}
