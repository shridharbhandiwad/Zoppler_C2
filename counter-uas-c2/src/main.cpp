/**
 * Counter-UAS Command & Control System
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
 * - Modern Qt-based tactical interface
 */

#include <QApplication>
#include <QStyleFactory>
#include <QSurfaceFormat>
#include "ui/MainWindow.h"
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
    app.setApplicationName("Counter-UAS C2");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Defense Systems");
    
    // Set dark fusion style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
    
    // Initialize logger
    Logger::instance().setLogLevel(LogLevel::Debug);
    Logger::instance().setLogToConsole(true);
    Logger::instance().info("Main", "Counter-UAS C2 System starting...");
    
    // Load configuration
    ConfigManager::instance().loadDefaults();
    
    // Initialize database
    DatabaseManager::instance().initialize("data/counter_uas.db");
    
    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    // Create track simulator for testing
    TrackSimulator simulator(mainWindow.trackManager());
    simulator.start();
    
    // Start simulation automatically for demo
    mainWindow.startSimulation();
    
    Logger::instance().info("Main", "System initialized successfully");
    
    int result = app.exec();
    
    // Cleanup
    simulator.stop();
    DatabaseManager::instance().close();
    
    Logger::instance().info("Main", "System shutdown complete");
    
    return result;
}
