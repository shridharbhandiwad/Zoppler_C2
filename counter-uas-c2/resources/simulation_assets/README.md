# Simulation Video Assets

This directory contains video assets (images) for simulation playback in the Counter-UAS C2 system.

## Overview

The video simulation system supports multiple modes:

1. **Generated Mode** (Default): Procedurally generates frames with simulated:
   - Electro-Optical (EO) tracking camera view
   - Thermal/IR camera view
   - Radar PPI display

2. **Image Sequence Mode**: Plays a sequence of images from this directory as video frames

3. **Resources Mode**: Loads images from Qt resources (embedded in the application)

## Adding Custom Simulation Images

### For Image Sequence Mode

1. Create numbered image files (e.g., `frame_001.png`, `frame_002.png`, etc.)
2. Place them in this directory
3. Supported formats: PNG, JPG, JPEG, BMP, GIF
4. Recommended resolution: 1280x720 or 1920x1080
5. Recommended frame rate: 30 fps (will create ~1 second of video per 30 images)

### For Qt Resources Mode

1. Add your images to this directory
2. Edit `../resources.qrc` to include them:
   ```xml
   <qresource prefix="/simulation">
       <file>simulation_assets/frame_001.png</file>
       <file>simulation_assets/frame_002.png</file>
       ...
   </qresource>
   ```
3. Rebuild the application

## Simulation Scenarios

The generated mode supports different scenario types:

| Type | Description | Use Case |
|------|-------------|----------|
| 0 | EO/IR Tracking | Day/night optical tracking camera simulation |
| 1 | Thermal IR | FLIR/thermal imaging simulation |
| 2 | Radar Display | PPI scope radar display simulation |

## API Usage

### Starting Simulation with Default Cameras

```cpp
// In MainWindow or your controller
m_videoSimulator->setupDefaultCameras();
m_videoSimulator->start();
```

### Adding Custom Cameras

```cpp
VideoSimulator::SimulatedCamera customCam;
customCam.cameraId = "CUSTOM-001";
customCam.name = "Custom Camera";
customCam.scenarioType = 0;  // EO mode
customCam.enabled = true;

m_videoSimulator->addSimulatedCamera(customCam);
```

### Using Image Sequence

```cpp
SimulationVideoSource* source = new SimulationVideoSource("SEQ-001", this);
source->loadImageSequence("/path/to/images");
source->setLooping(true);
source->open(QUrl::fromLocalFile("/path/to/images"));
source->start();
```

## Frame Requirements

For best results with image sequences:

- **Resolution**: Match your target display resolution (e.g., 1280x720)
- **Format**: PNG for lossless quality, JPG for smaller file size
- **Naming**: Use sequential numbering with leading zeros (frame_001.png)
- **Content**: Include timestamps, crosshairs, and telemetry overlays in the images if not using the overlay system

## Telemetry Overlay

The simulation system can automatically add overlays including:

- Timestamp
- Camera ID
- Frame number
- PTZ position (Azimuth, Elevation, Zoom)
- GPS coordinates
- Recording indicator
- Target tracking box

To disable overlays:
```cpp
source->setShowOverlay(false);
```
