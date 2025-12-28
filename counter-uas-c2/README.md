# Counter-UAS Command & Control System

A real-time Command and Control (C2) application for Counter-UAS system protecting military installations. Built with Qt 6 and C++17.

## Features

### Core Capabilities
- **Multi-Sensor Integration**: Radar, RF detector, and camera system support
- **Track Management**: Kalman-filtered track correlation and lifecycle management
- **Threat Assessment**: Rule-based threat evaluation with configurable thresholds
- **Video Management**: Multi-stream video handling with overlays
- **Effector Control**: RF jammer, kinetic interceptor, and directed energy system support
- **Tactical Display**: Real-time map with track symbology and defended areas

### Technical Highlights
- 10+ Hz map and track update rate
- Support for 100+ simultaneous tracks
- Up to 16 camera streams (9 displayed simultaneously)
- Sub-300ms video latency target
- Role-based engagement authorization workflow
- SQLite database for track history and audit trail

## Requirements

### Build Requirements
- Qt 6.x (Core, Gui, Widgets, Network, Sql, Multimedia, MultimediaWidgets, OpenGLWidgets, Concurrent, StateMachine)
- CMake 3.16+
- C++17 compatible compiler
- Linux (primary) or Windows (secondary)

### Optional
- Qt Location module for enhanced map features
- FFmpeg/GStreamer for extended video codec support

## Building

```bash
# Clone the repository
git clone <repository-url>
cd counter-uas-c2

# Build
chmod +x build.sh
./build.sh Release

# Or manually with CMake
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

## Running

```bash
./build/CounterUAS_C2
```

The application will start with a simulated environment including:
- Simulated radar tracks approaching the defended area
- Default defended asset configuration
- Dark tactical UI theme

## Configuration

Configuration files are located in the `config/` directory:

- `system.json` - System-wide settings
- `cameras.json` - Camera stream definitions
- `sensors.json` - Sensor configurations
- `effectors.json` - Effector definitions
- `defended_assets.json` - Protected areas and no-fire zones

## Architecture

```
src/
├── core/          # Track management, threat assessment, engagement
├── sensors/       # Sensor interfaces (Radar, RF, Camera)
├── video/         # Video stream management and overlays
├── effectors/     # Effector control (Jammer, Kinetic, DE)
├── network/       # Communication protocols
├── ui/            # Qt widgets and panels
├── config/        # Configuration management
├── utils/         # Utilities (logging, coordinates, filters)
└── simulators/    # Test data generators
```

## Key Classes

- `TrackManager` - Track correlation and lifecycle management
- `ThreatAssessor` - Rule-based threat evaluation
- `EngagementManager` - Engagement workflow state machine
- `VideoStreamManager` - Multi-source video handling
- `MapWidget` - Tactical map display
- `VideoDisplayWidget` - Video feed with overlays

## Keyboard Shortcuts

- `F11` - Toggle full-screen video
- `Ctrl+O` - Load configuration
- `Ctrl+S` - Save configuration
- `Ctrl+Q` - Exit application

## License

Proprietary - Defense Systems

## Contact

For support and inquiries, contact the development team.
