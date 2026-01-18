# Counter-UAS Command and Control (C2) System
## Comprehensive Design and Implementation Document

**Version:** 1.0  
**Document Classification:** Technical Reference  
**System:** Counter-Unmanned Aircraft Systems Command and Control

---

## Table of Contents

1. [Executive Summary](#1-executive-summary)
2. [System Architecture Overview](#2-system-architecture-overview)
3. [Core Data Structures and Types](#3-core-data-structures-and-types)
4. [Track Management Subsystem](#4-track-management-subsystem)
5. [Threat Assessment Module](#5-threat-assessment-module)
6. [Engagement Management Subsystem](#6-engagement-management-subsystem)
7. [Sensor Integration Layer](#7-sensor-integration-layer)
8. [Effector Control Systems](#8-effector-control-systems)
9. [Video Management Subsystem](#9-video-management-subsystem)
10. [Network Communications](#10-network-communications)
11. [Simulation Framework](#11-simulation-framework)
12. [User Interface Design](#12-user-interface-design)
13. [Configuration Management](#13-configuration-management)
14. [Filter and Algorithm Implementations](#14-filter-and-algorithm-implementations)
15. [System Thresholds and Constants](#15-system-thresholds-and-constants)
16. [Performance Specifications](#16-performance-specifications)
17. [Appendices](#17-appendices)

---

## 1. Executive Summary

### 1.1 Purpose

The Counter-UAS Command and Control (C2) System is a real-time military-grade software application designed to protect critical installations from unauthorized unmanned aircraft systems (UAS/drones). The system integrates multiple sensor types, processes track data, assesses threats, and coordinates effector engagement to neutralize hostile targets.

### 1.2 Technical Foundation

- **Programming Language:** C++17
- **GUI Framework:** Qt 6.x
- **Build System:** CMake 3.16+
- **Target Platform:** Linux (primary), Windows (secondary)
- **Architecture:** Event-driven, multi-threaded with Qt signal/slot mechanism

### 1.3 Core Capabilities

| Capability | Specification |
|------------|---------------|
| Update Rate | 10+ Hz map and track updates |
| Track Capacity | 100+ simultaneous tracks |
| Video Streams | Up to 16 streams (9 displayed) |
| Video Latency | Sub-300ms target |
| Engagement Workflow | Role-based authorization |
| Data Persistence | SQLite database |

---

## 2. System Architecture Overview

### 2.1 High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        USER INTERFACE LAYER                         │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌───────────┐ │
│  │ MapWidget│ │PPIDisplay│ │VideoGrid │ │TrackList │ │EffectorCtl│ │
│  └──────────┘ └──────────┘ └──────────┘ └──────────┘ └───────────┘ │
└─────────────────────────────────────────────────────────────────────┘
                                  │
┌─────────────────────────────────────────────────────────────────────┐
│                      CORE PROCESSING LAYER                          │
│  ┌────────────────┐  ┌─────────────────┐  ┌───────────────────┐    │
│  │  TrackManager  │  │  ThreatAssessor │  │EngagementManager  │    │
│  │  (Correlation) │  │  (Rule Engine)  │  │(State Machine)    │    │
│  └────────────────┘  └─────────────────┘  └───────────────────┘    │
└─────────────────────────────────────────────────────────────────────┘
                                  │
┌─────────────────────────────────────────────────────────────────────┐
│                    INTEGRATION LAYER                                │
│  ┌────────────────────────────┐  ┌────────────────────────────┐    │
│  │    Sensor Interfaces       │  │    Effector Interfaces     │    │
│  │  ┌──────┐ ┌────┐ ┌──────┐ │  │ ┌──────┐ ┌───────┐ ┌────┐  │    │
│  │  │Radar │ │RF  │ │Camera│ │  │ │Jammer│ │Kinetic│ │ DE │  │    │
│  │  └──────┘ └────┘ └──────┘ │  │ └──────┘ └───────┘ └────┘  │    │
│  └────────────────────────────┘  └────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────┘
                                  │
┌─────────────────────────────────────────────────────────────────────┐
│                    NETWORK/COMMUNICATIONS LAYER                     │
│  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐  │
│  │ NetworkManager   │  │ MessageProtocol  │  │ VideoStreaming   │  │
│  └──────────────────┘  └──────────────────┘  └──────────────────┘  │
└─────────────────────────────────────────────────────────────────────┘
```

### 2.2 Namespace Organization

All system components reside within the `CounterUAS` namespace to prevent naming conflicts and provide logical organization:

```cpp
namespace CounterUAS {
    // Core classes
    class Track;
    class TrackManager;
    class ThreatAssessor;
    class EngagementManager;
    
    // Sensor interfaces
    class SensorInterface;
    class RadarSensor;
    class RFDetector;
    class CameraSystem;
    
    // Effector interfaces
    class EffectorInterface;
    class RFJammer;
    class KineticInterceptor;
    class DirectedEnergySystem;
    
    // Supporting utilities
    class KalmanFilter2D;
    class CoordinateUtils;
    class Logger;
}
```

### 2.3 Module Dependencies

```
                    ConfigManager
                         │
           ┌─────────────┼─────────────┐
           ▼             ▼             ▼
      TrackManager ─► ThreatAssessor ─► EngagementManager
           │             │                    │
           ▼             ▼                    ▼
    SensorInterface  DefendedAssets    EffectorInterface
           │                                  │
           ▼                                  ▼
    RadarSensor          VideoStreamManager  RFJammer
    RFDetector                               KineticInterceptor
    CameraSystem                             DirectedEnergySystem
```

---

## 3. Core Data Structures and Types

### 3.1 Geographic Position Structure

The `GeoPosition` structure represents a three-dimensional geographic coordinate:

```cpp
struct GeoPosition {
    double latitude = 0.0;   // Degrees (-90 to +90)
    double longitude = 0.0;  // Degrees (-180 to +180)
    double altitude = 0.0;   // Meters above ground level (AGL)
    
    bool isValid() const {
        return latitude >= -90.0 && latitude <= 90.0 &&
               longitude >= -180.0 && longitude <= 180.0;
    }
    
    QJsonObject toJson() const;
    static GeoPosition fromJson(const QJsonObject& json);
};
```

**Validation Rules:**
- Latitude must be within [-90.0, +90.0] degrees
- Longitude must be within [-180.0, +180.0] degrees
- Altitude is typically positive (above ground) but can be negative for below-grade scenarios

### 3.2 Velocity Vector Structure

The `VelocityVector` structure represents three-dimensional motion in a North-East-Down (NED) coordinate frame:

```cpp
struct VelocityVector {
    double north = 0.0;  // Meters per second (positive = northward)
    double east = 0.0;   // Meters per second (positive = eastward)
    double down = 0.0;   // Meters per second (positive = descending)
    
    double speed() const {
        return std::sqrt(north * north + east * east + down * down);
    }
    
    double heading() const {
        double hdg = qRadiansToDegrees(std::atan2(east, north));
        if (hdg < 0) hdg += 360.0;
        return hdg;  // Degrees from true north (0-360)
    }
    
    double climbRate() const {
        return -down;  // Positive = climbing
    }
};
```

**Coordinate Convention:**
- **North-East-Down (NED):** Standard aviation/navigation frame
- Positive north = northward motion
- Positive east = eastward motion
- Positive down = descent (negative climb)

### 3.3 Bounding Box Structure

For video overlay and visual tracking:

```cpp
struct BoundingBox {
    int x = 0;           // Left edge in pixels
    int y = 0;           // Top edge in pixels
    int width = 0;       // Box width in pixels
    int height = 0;      // Box height in pixels
    QString cameraId;    // Associated camera identifier
    qint64 timestamp = 0; // Time of detection (ms since epoch)
    
    bool isValid() const { return width > 0 && height > 0; }
};
```

### 3.4 Enumeration Types

#### 3.4.1 Track Classification

```cpp
enum class TrackClassification {
    Unknown = 0,    // Insufficient data for classification
    Friendly,       // Confirmed friendly/authorized aircraft
    Hostile,        // Confirmed threat requiring engagement
    Pending,        // Awaiting classification confirmation
    Neutral         // Non-threat, non-friendly (e.g., birds)
};
```

#### 3.4.2 Track State (Lifecycle)

```cpp
enum class TrackState {
    Initiated = 0,  // Newly created, minimal data
    Active,         // Receiving regular updates
    Coasting,       // No recent updates, predicted position
    Dropped         // Track terminated
};
```

#### 3.4.3 Detection Source

```cpp
enum class DetectionSource {
    Radar = 0,      // Primary surveillance radar
    RFDetector,     // RF signal detection system
    Camera,         // Electro-optical/infrared camera
    Combined,       // Multi-sensor fusion
    Manual          // Operator-entered track
};
```

---

## 4. Track Management Subsystem

### 4.1 Overview

The `TrackManager` class is the central hub for track correlation, lifecycle management, and multi-sensor fusion. It maintains the system's ground truth for all detected targets.

### 4.2 Configuration Structure

```cpp
struct TrackManagerConfig {
    int updateRateHz = 10;               // Processing cycle rate
    double correlationDistanceM = 100.0; // Max distance for track correlation
    double correlationVelocityMps = 10.0;// Max velocity difference for correlation
    int coastingTimeoutMs = 5000;        // Time before track enters coasting
    int dropTimeoutMs = 15000;           // Time before track is dropped
    int maxCoastCount = 10;              // Maximum coast cycles before drop
    bool enableKalmanFilter = true;      // Position smoothing enabled
    int maxTracks = 200;                 // Maximum concurrent tracks
    int historyRetentionMs = 60000;      // Position history retention
};
```

### 4.3 Track Correlation Algorithm

The system uses a weighted scoring algorithm to correlate new sensor detections with existing tracks:

```cpp
double TrackManager::calculateCorrelationScore(Track* track, 
                                               const GeoPosition& pos,
                                               const VelocityVector& vel) {
    // Distance component (50% weight)
    double distance = track->distanceTo(pos);
    double distanceScore = 1.0;
    if (distance > m_config.correlationDistanceM) {
        distanceScore = 0.0;
    } else {
        distanceScore = 1.0 - (distance / m_config.correlationDistanceM);
    }
    
    // Velocity component (30% weight)
    double velocityScore = 1.0;
    VelocityVector trackVel = track->velocity();
    double velDiff = std::sqrt(
        std::pow(trackVel.north - vel.north, 2) +
        std::pow(trackVel.east - vel.east, 2) +
        std::pow(trackVel.down - vel.down, 2)
    );
    if (velDiff > m_config.correlationVelocityMps) {
        velocityScore = 0.5;  // Penalize but don't eliminate
    } else {
        velocityScore = 1.0 - (velDiff / (2 * m_config.correlationVelocityMps));
    }
    
    // Time since update component (20% weight)
    double timeScore = 1.0;
    qint64 timeSince = track->timeSinceUpdate();
    if (timeSince > m_config.coastingTimeoutMs) {
        timeScore = 0.3;
    } else {
        timeScore = 1.0 - (static_cast<double>(timeSince) / 
                          m_config.coastingTimeoutMs * 0.5);
    }
    
    // Weighted combination
    return distanceScore * 0.5 + velocityScore * 0.3 + timeScore * 0.2;
}
```

**Correlation Thresholds:**

| Parameter | Value | Description |
|-----------|-------|-------------|
| Minimum Correlation Score | 0.5 | Score below this creates new track |
| Distance Weight | 50% | Spatial proximity importance |
| Velocity Weight | 30% | Motion consistency importance |
| Time Weight | 20% | Track freshness importance |

### 4.4 Track Lifecycle State Machine

```
     ┌───────────────────────────────────────────────────────────┐
     │                                                           │
     ▼                                                           │
┌──────────┐    Update    ┌────────┐    No Update   ┌──────────┐│
│ INITIATED│───────────► │ ACTIVE │───────────────►│ COASTING ││
└──────────┘              └────────┘                └──────────┘│
     │                         │                         │      │
     │      coastingTimeoutMs  │                         │      │
     │      (5000ms)           │     dropTimeoutMs       │      │
     │                         │     (15000ms) or        │      │
     │                         │     maxCoastCount (10)  │      │
     │                         ▼                         │      │
     │                    ┌─────────┐                    │      │
     └───────────────────►│ DROPPED │◄───────────────────┘      │
                          └─────────┘                           │
                               │        Update received         │
                               └────────────────────────────────┘
```

### 4.5 Track State Update Logic

```cpp
void TrackManager::updateTrackState(Track* track) {
    qint64 timeSinceUpdate = track->timeSinceUpdate();
    
    if (track->state() == TrackState::Active) {
        if (timeSinceUpdate > m_config.coastingTimeoutMs) {
            track->setState(TrackState::Coasting);
            track->incrementCoastCount();
            emit trackStateChanged(track->trackId(), TrackState::Coasting);
        }
    } else if (track->state() == TrackState::Coasting) {
        if (timeSinceUpdate > m_config.dropTimeoutMs || 
            track->coastCount() > m_config.maxCoastCount) {
            track->setState(TrackState::Dropped);
            m_stats.totalTracksDropped++;
            emit trackStateChanged(track->trackId(), TrackState::Dropped);
            emit trackDropped(track->trackId());
        } else {
            track->incrementCoastCount();
        }
    } else if (track->state() == TrackState::Initiated) {
        if (track->timeSinceUpdate() < m_config.coastingTimeoutMs) {
            track->setState(TrackState::Active);
            emit trackStateChanged(track->trackId(), TrackState::Active);
        }
    }
}
```

### 4.6 Track ID Generation

Track identifiers follow a sequential numbering scheme with a prefix:

```cpp
QString TrackManager::generateTrackId() {
    return QString("TRK-%1").arg(m_nextTrackNumber++, 4, 10, QChar('0'));
}
// Examples: TRK-0001, TRK-0002, TRK-0003, ...
```

### 4.7 Multi-Sensor Fusion

#### 4.7.1 Radar Detection Processing

```cpp
void TrackManager::processRadarDetection(const GeoPosition& pos, 
                                         const VelocityVector& vel,
                                         double quality, 
                                         qint64 timestamp) {
    Track* correlated = findCorrelatedTrack(pos, vel, DetectionSource::Radar);
    
    if (correlated) {
        updateTrack(correlated->trackId(), pos);
        updateTrackVelocity(correlated->trackId(), vel);
        correlated->addDetectionSource(DetectionSource::Radar);
        correlated->setTrackQuality(qMax(correlated->trackQuality(), quality));
    } else {
        QString newId = createTrack(pos, DetectionSource::Radar);
        if (!newId.isEmpty()) {
            updateTrackVelocity(newId, vel);
        }
    }
}
```

#### 4.7.2 RF Detection Processing

```cpp
void TrackManager::processRFDetection(const GeoPosition& pos, 
                                      double signalStrength,
                                      qint64 timestamp) {
    VelocityVector emptyVel;
    Track* correlated = findCorrelatedTrack(pos, emptyVel, DetectionSource::RFDetector);
    
    if (correlated) {
        updateTrack(correlated->trackId(), pos);
        correlated->addDetectionSource(DetectionSource::RFDetector);
        // RF detection increases confidence it's a drone
        if (signalStrength > 0.7 && 
            correlated->classification() == TrackClassification::Pending) {
            setTrackClassification(correlated->trackId(), 
                                   TrackClassification::Hostile, 0.6);
        }
    } else {
        createTrack(pos, DetectionSource::RFDetector);
    }
}
```

**RF Classification Threshold:**
- Signal strength > 0.7: Automatically classify as Hostile with 60% confidence

### 4.8 Distance Calculation (Haversine Formula)

```cpp
double Track::distanceTo(const GeoPosition& pos) const {
    const double R = 6371000.0; // Earth radius in meters
    
    double lat1 = qDegreesToRadians(m_position.latitude);
    double lat2 = qDegreesToRadians(pos.latitude);
    double dLat = lat2 - lat1;
    double dLon = qDegreesToRadians(pos.longitude - m_position.longitude);
    
    double a = std::sin(dLat/2) * std::sin(dLat/2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon/2) * std::sin(dLon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    double horizontalDist = R * c;
    double verticalDist = pos.altitude - m_position.altitude;
    
    // 3D distance
    return std::sqrt(horizontalDist * horizontalDist + verticalDist * verticalDist);
}
```

### 4.9 Position Prediction

```cpp
GeoPosition Track::predictedPosition(qint64 deltaMs) const {
    double dt = deltaMs / 1000.0;  // Convert to seconds
    
    // Conversion factors
    const double metersPerDegreeLat = 111000.0;
    double metersPerDegreeLon = 111000.0 * 
                                std::cos(qDegreesToRadians(m_position.latitude));
    
    GeoPosition predicted = m_position;
    predicted.latitude += (m_velocity.north * dt) / metersPerDegreeLat;
    predicted.longitude += (m_velocity.east * dt) / metersPerDegreeLon;
    predicted.altitude -= m_velocity.down * dt;
    
    return predicted;
}
```

---

## 5. Threat Assessment Module

### 5.1 Overview

The `ThreatAssessor` class implements a rule-based threat evaluation engine that analyzes tracks and assigns threat levels based on configurable criteria.

### 5.2 Configuration Structure

```cpp
struct ThreatAssessorConfig {
    int assessmentIntervalMs = 500;    // Assessment cycle period
    int alertQueueMaxSize = 100;       // Maximum pending alerts
    bool autoSlewToHighestThreat = true; // Auto-camera pointing
    int highThreatThreshold = 4;       // Level triggering high-threat actions
    double headingToleranceDeg = 30.0; // Heading comparison tolerance
};
```

### 5.3 Defended Asset Definition

```cpp
struct DefendedAsset {
    QString id;                          // Unique identifier
    QString name;                        // Display name
    GeoPosition position;                // Asset location
    double criticalRadiusM = 500.0;      // Inner zone radius
    double warningRadiusM = 1500.0;      // Outer warning radius
    int priorityLevel = 1;               // 1-5, higher = more critical
};
```

### 5.4 Threat Level Calculation Algorithm

The threat level is calculated as a composite score from multiple factors:

```cpp
int ThreatAssessor::calculateThreatLevel(Track* track) {
    if (m_assets.isEmpty()) {
        return track->threatLevel();
    }
    
    int level = 1;  // Base level
    
    // Factor 1: Classification (+2 for hostile, +1 for pending)
    switch (track->classification()) {
        case TrackClassification::Hostile:
            level += 2;
            break;
        case TrackClassification::Pending:
            level += 1;
            break;
        case TrackClassification::Friendly:
            return 1;  // Friendlies always low threat
        default:
            break;
    }
    
    // Factor 2: Proximity to defended assets
    DefendedAsset* nearestAsset = nullptr;
    double distance = calculateProximityToAssets(track, &nearestAsset);
    
    if (nearestAsset) {
        if (distance < nearestAsset->criticalRadiusM) {
            level += 3;  // Critical proximity
        } else if (distance < nearestAsset->warningRadiusM) {
            level += 2;  // Warning proximity
        } else if (distance < nearestAsset->warningRadiusM * 2) {
            level += 1;  // Extended warning
        }
        
        // Factor 3: Heading toward asset (+1 if approaching)
        if (isHeadingTowardAsset(track, *nearestAsset)) {
            level += 1;
        }
    }
    
    // Factor 4: Velocity (>30 m/s adds +1)
    double speed = track->velocity().speed();
    if (speed > 30.0) {
        level += 1;
    }
    
    // Factor 5: Classification confidence
    if (track->classificationConfidence() < 0.5) {
        level = std::max(1, level - 1);  // Reduce if uncertain
    }
    
    // Clamp to valid range [1, 5]
    return std::min(5, std::max(1, level));
}
```

**Threat Level Summary:**

| Factor | Condition | Level Impact |
|--------|-----------|--------------|
| Classification: Hostile | - | +2 |
| Classification: Pending | - | +1 |
| Critical Proximity | < criticalRadiusM | +3 |
| Warning Proximity | < warningRadiusM | +2 |
| Extended Warning | < warningRadiusM × 2 | +1 |
| Heading to Asset | Within 30° tolerance | +1 |
| High Speed | > 30 m/s | +1 |
| Low Confidence | < 0.5 | -1 |

### 5.5 Threat Rule Structure

```cpp
struct ThreatRule {
    QString id;                          // Rule identifier
    QString name;                        // Display name
    QString description;                 // Rule description
    bool enabled = true;                 // Rule active status
    
    // Condition parameters (-1 means "any")
    double minProximityM = -1;           // Minimum distance
    double maxProximityM = -1;           // Maximum distance
    double minVelocityMps = -1;          // Minimum speed
    double maxVelocityMps = -1;          // Maximum speed
    double minHeadingToAssetDeg = -1;    // Heading range start
    double maxHeadingToAssetDeg = -1;    // Heading range end
    bool requiresVisualConfirmation = false;
    bool requiresRFDetection = false;
    
    // Action parameters
    int threatLevelIncrease = 0;         // Additive threat increase
    int setThreatLevel = -1;             // Absolute threat level (-1 = don't set)
    TrackClassification forceClassification = TrackClassification::Unknown;
    bool generateAlert = false;          // Create alert notification
    QString alertMessage;                // Alert text template
};
```

### 5.6 Default Threat Rules

The system initializes with five default rules:

#### Rule 1: Critical Proximity
```cpp
ThreatRule criticalProximity;
criticalProximity.id = "RULE-001";
criticalProximity.name = "Critical Proximity";
criticalProximity.description = "Track within critical radius of defended asset";
criticalProximity.maxProximityM = 500.0;
criticalProximity.setThreatLevel = 5;
criticalProximity.forceClassification = TrackClassification::Hostile;
criticalProximity.generateAlert = true;
criticalProximity.alertMessage = "CRITICAL: Track %TRACK% within critical radius!";
```

#### Rule 2: Warning Proximity
```cpp
ThreatRule warningProximity;
warningProximity.id = "RULE-002";
warningProximity.name = "Warning Proximity";
warningProximity.description = "Track within warning radius";
warningProximity.minProximityM = 500.0;
warningProximity.maxProximityM = 1500.0;
warningProximity.threatLevelIncrease = 2;
warningProximity.generateAlert = true;
warningProximity.alertMessage = "WARNING: Track %TRACK% approaching defended area";
```

#### Rule 3: High Velocity Approach
```cpp
ThreatRule highVelocity;
highVelocity.id = "RULE-003";
highVelocity.name = "High Velocity Approach";
highVelocity.description = "Fast moving track heading toward asset";
highVelocity.minVelocityMps = 20.0;
highVelocity.minHeadingToAssetDeg = 0.0;
highVelocity.maxHeadingToAssetDeg = 30.0;
highVelocity.threatLevelIncrease = 1;
```

#### Rule 4: RF Confirmed Drone
```cpp
ThreatRule rfConfirmed;
rfConfirmed.id = "RULE-004";
rfConfirmed.name = "RF Confirmed";
rfConfirmed.description = "Track confirmed by RF detection";
rfConfirmed.requiresRFDetection = true;
rfConfirmed.threatLevelIncrease = 1;
rfConfirmed.forceClassification = TrackClassification::Hostile;
```

#### Rule 5: Visual Confirmation Required
```cpp
ThreatRule visualUnconfirmed;
visualUnconfirmed.id = "RULE-005";
visualUnconfirmed.name = "Unconfirmed Visual";
visualUnconfirmed.description = "High threat without visual confirmation";
visualUnconfirmed.requiresVisualConfirmation = false;
visualUnconfirmed.minProximityM = 0;
visualUnconfirmed.maxProximityM = 2000.0;
visualUnconfirmed.generateAlert = true;
visualUnconfirmed.alertMessage = "Track %TRACK% requires visual confirmation";
```

### 5.7 Heading-to-Asset Calculation

```cpp
bool ThreatAssessor::isHeadingTowardAsset(Track* track, const DefendedAsset& asset) {
    double trackHeading = track->velocity().heading();
    double bearingToAsset = track->bearingTo(asset.position);
    
    double diff = std::abs(trackHeading - bearingToAsset);
    if (diff > 180.0) {
        diff = 360.0 - diff;
    }
    
    return diff < m_config.headingToleranceDeg;  // Default: 30°
}
```

### 5.8 Alert Generation and Deduplication

```cpp
void ThreatAssessor::generateAlert(Track* track, const ThreatRule& rule) {
    // Check for duplicate recent alerts (30-second window)
    for (const auto& existing : m_alerts) {
        if (existing.trackId == track->trackId() && 
            !existing.acknowledged &&
            existing.timestamp.secsTo(QDateTime::currentDateTimeUtc()) < 30) {
            return;  // Don't spam alerts
        }
    }
    
    ThreatAlert alert;
    alert.alertId = generateAlertId();
    alert.trackId = track->trackId();
    alert.message = rule.alertMessage;
    alert.message.replace("%TRACK%", track->trackId());
    alert.threatLevel = track->threatLevel();
    alert.timestamp = QDateTime::currentDateTimeUtc();
    
    m_alerts.append(alert);
    
    // Trim alert queue if oversized
    while (m_alerts.size() > m_config.alertQueueMaxSize) {
        m_alerts.removeFirst();
    }
    
    emit newAlert(alert);
}
```

### 5.9 Threat Metrics

```cpp
struct ThreatMetrics {
    int hostileCount = 0;          // Count of hostile tracks
    int pendingCount = 0;          // Count of pending tracks
    int highThreatCount = 0;       // Tracks at level >= highThreatThreshold
    double avgThreatLevel = 0.0;   // Average threat level
    QString highestThreatTrackId;  // ID of highest threat track
    double closestDistanceM = -1;  // Closest track to any asset
    qint64 lastAssessmentMs = 0;   // Timestamp of last assessment
};
```

---

## 6. Engagement Management Subsystem

### 6.1 Overview

The `EngagementManager` implements a state-machine-driven workflow for target engagement, enforcing authorization requirements and tracking engagement outcomes.

### 6.2 Engagement State Machine

```cpp
enum class EngagementState {
    Idle = 0,              // No active engagement
    TrackSelected,         // Target track selected
    EffectorRecommended,   // System has recommended an effector
    AwaitingAuthorization, // Waiting for operator approval
    Authorized,            // Engagement approved
    Engaging,              // Effector actively engaging
    Completed,             // Engagement finished
    Aborted,               // Engagement cancelled
    Failed                 // Engagement failed
};
```

**State Transition Diagram:**

```
┌────────────────────────────────────────────────────────────────────────┐
│                                                                        │
│  ┌──────┐  selectTrack()  ┌───────────────┐  selectEffector()         │
│  │ IDLE │────────────────►│ TRACK_SELECTED│────────────────────┐      │
│  └──────┘                 └───────────────┘                    │      │
│      ▲                                                         ▼      │
│      │                                              ┌───────────────┐ │
│      │                                              │EFFECTOR_REC'D │ │
│      │                                              └───────────────┘ │
│      │                                                         │      │
│      │                           requestAuthorization()        │      │
│      │                                                         ▼      │
│      │                                              ┌───────────────┐ │
│      │                timeout()                     │AWAITING_AUTH  │ │
│      │            ┌─────────────────────────────────┤               │ │
│      │            │                                 └───────────────┘ │
│      │            │                                         │         │
│      │            │            authorize()                  │ deny()  │
│      │            │            ┌────────────────────────────┤         │
│      │            │            ▼                            ▼         │
│      │            │   ┌────────────┐                ┌─────────────┐   │
│      │            │   │ AUTHORIZED │                │   ABORTED   │   │
│      │            │   └────────────┘                └─────────────┘   │
│      │            │            │                            ▲         │
│      │            │  execute() │                   abort()  │         │
│      │            │            ▼                            │         │
│      │            │   ┌────────────┐────────────────────────┘         │
│      │            │   │  ENGAGING  │                                  │
│      │            │   └────────────┘                                  │
│      │            │            │                                      │
│      │            │   completion│              failure                │
│      │            │            ▼                    ▼                  │
│      │            │   ┌────────────┐       ┌────────────┐             │
│      │            └──►│ COMPLETED  │       │   FAILED   │             │
│      │                └────────────┘       └────────────┘             │
│      │                      │                    │                    │
│      └──────────────────────┴────────────────────┘                    │
└────────────────────────────────────────────────────────────────────────┘
```

### 6.3 Battle Damage Assessment (BDA)

```cpp
enum class BDAResult {
    Unknown = 0,           // No assessment yet
    TargetDestroyed,       // Target confirmed destroyed
    TargetDamaged,         // Target damaged but not destroyed
    TargetMissed,          // Engagement did not affect target
    TargetEvaded,          // Target maneuvered to avoid
    AssessmentPending      // Awaiting BDA confirmation
};
```

### 6.4 Engagement Record

```cpp
struct EngagementRecord {
    QString engagementId;          // Unique engagement identifier
    QString trackId;               // Target track ID
    QString effectorId;            // Effector used
    QString effectorType;          // Effector type (RF_JAMMER, KINETIC, etc.)
    QString operatorId;            // Authorizing operator
    
    QDateTime startTime;           // Engagement initiation
    QDateTime authorizationTime;   // Authorization timestamp
    QDateTime executionTime;       // Effector activation
    QDateTime completionTime;      // Engagement end
    
    EngagementState state;         // Final state
    BDAResult bdaResult;           // Outcome assessment
    
    GeoPosition targetPosition;    // Target position at engagement
    double targetDistance = 0.0;   // Range at engagement
    int threatLevel = 0;           // Target threat level
    
    QImage videoSnapshot;          // Video capture
    QString notes;                 // Operator notes
    
    bool wasAborted = false;       // Abort flag
    QString abortReason;           // Abort cause
};
```

### 6.5 Authorization Request

```cpp
struct AuthorizationRequest {
    QString engagementId;          // Associated engagement
    QString trackId;               // Target track
    QString effectorId;            // Proposed effector
    QString effectorType;          // Effector category
    
    GeoPosition targetPosition;    // Target location
    double distance = 0.0;         // Range to target
    int threatLevel = 0;           // Current threat level
    TrackClassification classification; // Target classification
    
    QString recommendationReason;  // Why this effector
    QImage videoThumbnail;         // Visual reference
    
    QDateTime requestTime;         // Request timestamp
    int timeoutSeconds = 60;       // Authorization window
};
```

### 6.6 Effector Selection Algorithm

```cpp
double EngagementManager::calculateEffectorScore(EffectorInterface* effector, 
                                                  Track* track) {
    if (!effector || !track) return 0.0;
    
    double score = 0.0;
    
    // Check readiness
    if (!effector->isReady()) return 0.0;
    
    // Check range
    double distance = track->distanceTo(GeoPosition());  // Base position
    double maxRange = effector->maxRange();
    double minRange = effector->minRange();
    
    if (distance < minRange || distance > maxRange) {
        return 0.0;  // Out of range
    }
    
    // Range score (40% weight) - prefer middle of envelope
    double rangeCenter = (maxRange + minRange) / 2.0;
    double rangeDiff = std::abs(distance - rangeCenter);
    double rangeScore = 1.0 - (rangeDiff / (maxRange - minRange));
    score += rangeScore * 0.4;
    
    // Effectiveness score (30% weight)
    score += effector->effectiveness() * 0.3;
    
    // Availability score (20% weight)
    score += (effector->isReady() ? 0.2 : 0.0);
    
    // Effector type preference (10% weight)
    // Prefer RF jamming for unknown/pending classifications
    if (effector->effectorType() == "RF_JAMMER") {
        if (track->classification() == TrackClassification::Pending ||
            track->classification() == TrackClassification::Unknown) {
            score += 0.1;
        }
    }
    
    return score;
}
```

### 6.7 Configuration Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| authorizationTimeoutSeconds | 60 | Time window for operator to authorize |
| autoRecommendEffector | true | Automatically suggest best effector |

---

## 7. Sensor Integration Layer

### 7.1 Sensor Interface Base Class

All sensors implement the abstract `SensorInterface` class:

```cpp
class SensorInterface : public QObject {
    Q_OBJECT
    
public:
    // Identity
    virtual QString sensorType() const = 0;
    virtual DetectionSource detectionSource() const = 0;
    
    // Connection management
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // Coverage characteristics
    virtual double maxRange() const { return 5000.0; }
    virtual double fieldOfView() const { return 360.0; }
    virtual double azimuth() const { return 0.0; }
    
    // Control
    virtual void start();
    virtual void stop();
    
signals:
    void detection(const SensorDetection& detection);
    void statusChanged(SensorStatus status);
    void connectedChanged(bool connected);
    void healthUpdated(const SensorHealth& health);
    void error(const QString& message);
};
```

### 7.2 Sensor Status Enumeration

```cpp
enum class SensorStatus {
    Unknown = 0,       // Status not determined
    Initializing,      // Starting up
    Online,            // Fully operational
    Degraded,          // Partial functionality
    Offline,           // Not communicating
    Error,             // Fault condition
    Maintenance        // Scheduled downtime
};
```

### 7.3 Sensor Health Structure

```cpp
struct SensorHealth {
    SensorStatus status = SensorStatus::Unknown;
    double signalQuality = 0.0;    // 0.0 - 1.0
    int detectionCount = 0;        // Detections since start
    qint64 lastDetectionTime = 0;  // Last detection timestamp
    QString errorMessage;          // Error description
    double cpuUsage = 0.0;         // Processor utilization
    double memoryUsage = 0.0;      // Memory utilization
    int droppedPackets = 0;        // Communication losses
    int connectionRetries = 0;     // Reconnection attempts
};
```

### 7.4 Radar Sensor Implementation

#### 7.4.1 Configuration

```cpp
struct RadarConfig {
    QString host = "127.0.0.1";
    quint16 port = 5001;
    int reconnectIntervalMs = 5000;
    int timeoutMs = 3000;
    double minRangeM = 50.0;
    double maxRangeM = 5000.0;
    double minAzimuthDeg = 0.0;
    double maxAzimuthDeg = 360.0;
    double minElevationDeg = -10.0;
    double maxElevationDeg = 90.0;
    bool filterClutter = true;
    double clutterThreshold = 0.1;
};
```

#### 7.4.2 Track Report Structure

```cpp
struct RadarTrackReport {
    quint32 trackNumber;       // Radar's internal track number
    float rangeM;              // Range in meters
    float azimuthDeg;          // Azimuth angle (degrees)
    float elevationDeg;        // Elevation angle (degrees)
    float rangeRateMps;        // Range rate (radial velocity)
    float azimuthRateDps;      // Azimuth rate (degrees/sec)
    float elevationRateDps;    // Elevation rate (degrees/sec)
    float rcs;                 // Radar cross section (m²)
    quint8 quality;            // Track quality (0-255)
    quint64 timestamp;         // Detection time
    
    GeoPosition toGeoPosition(const GeoPosition& radarPos) const;
    VelocityVector toVelocityVector(const GeoPosition& radarPos) const;
};
```

#### 7.4.3 Protocol Constants

```cpp
static constexpr quint32 HEADER_MAGIC = 0x52414452;  // "RADR"
static constexpr int HEADER_SIZE = 12;  // magic(4) + type(1) + length(4) + checksum(2) + reserved(1)

enum class RadarMessageType : quint8 {
    Heartbeat = 0x01,
    TrackReport = 0x02,
    StatusReport = 0x03,
    Configuration = 0x04,
    Command = 0x05,
    Ack = 0x06
};
```

### 7.5 RF Detector Implementation

#### 7.5.1 Configuration

```cpp
struct RFDetectorConfig {
    enum class ConnectionType { UDP, Serial };
    
    ConnectionType connectionType = ConnectionType::UDP;
    
    // UDP settings
    QString udpHost = "127.0.0.1";
    quint16 udpPort = 5002;
    
    // Serial settings
    QString serialPort = "/dev/ttyUSB0";
    qint32 baudRate = 115200;
    
    // Detection parameters
    double minFrequencyMHz = 900.0;
    double maxFrequencyMHz = 6000.0;
    double signalThresholdDbm = -80.0;
    bool enableDirectionFinding = true;
    double dfAccuracyDeg = 5.0;
    
    // Antenna array
    double antennaSpacingM = 0.5;
    int antennaCount = 4;
};
```

#### 7.5.2 RF Detection Result

```cpp
struct RFDetection {
    double frequencyMHz;           // Center frequency
    double signalStrengthDbm;      // Received signal strength
    double azimuthDeg;             // Direction (if DF enabled)
    double elevationDeg;           // Elevation angle
    double bandwidth;              // Signal bandwidth
    QString protocol;              // Identified protocol
    quint64 timestamp;             // Detection time
    QByteArray signature;          // Signal fingerprint
};
```

**RF Detection Thresholds:**

| Parameter | Default | Description |
|-----------|---------|-------------|
| Signal Threshold | -80 dBm | Minimum signal for detection |
| Min Frequency | 900 MHz | Lower frequency bound |
| Max Frequency | 6000 MHz | Upper frequency bound |
| DF Accuracy | 5° | Direction finding precision |

---

## 8. Effector Control Systems

### 8.1 Effector Interface Base Class

```cpp
class EffectorInterface : public QObject {
    Q_OBJECT
    
public:
    // Identity
    virtual QString effectorType() const = 0;
    virtual QString displayName() const { return m_displayName; }
    
    // Status
    EffectorStatus status() const { return m_health.status; }
    EffectorHealth health() const { return m_health; }
    virtual bool isReady() const { return m_health.status == EffectorStatus::Ready; }
    virtual bool isEngaged() const { return m_health.status == EffectorStatus::Engaged; }
    
    // Engagement
    virtual bool engage(const GeoPosition& target) = 0;
    virtual void disengage() = 0;
    virtual bool canEngage(const GeoPosition& target) const;
    
    // Range characteristics
    virtual double minRange() const { return 0.0; }
    virtual double maxRange() const { return 1000.0; }
    virtual double effectiveness() const { return 0.8; }
    
signals:
    void statusChanged(EffectorStatus status);
    void engagementStarted(const GeoPosition& target);
    void engagementComplete(bool success);
    void fault(const QString& message);
};
```

### 8.2 Effector Status Enumeration

```cpp
enum class EffectorStatus {
    Unknown = 0,       // Status not determined
    Offline,           // Not communicating
    Initializing,      // Starting up
    Ready,             // Available for engagement
    Engaged,           // Currently engaging target
    Reloading,         // Replenishing ammunition
    Cooldown,          // Post-engagement cooldown
    Maintenance,       // Scheduled maintenance
    Fault              // Error condition
};
```

### 8.3 RF Jammer

#### 8.3.1 Configuration

```cpp
struct RFJammerConfig {
    QString host = "127.0.0.1";
    int port = 5010;
    
    // Frequency bands
    double minFrequencyMHz = 2400.0;
    double maxFrequencyMHz = 5800.0;
    QList<double> jamFrequenciesMHz = {2400.0, 2450.0, 5200.0, 5800.0};
    
    // Power settings
    double maxPowerW = 100.0;
    double defaultPowerW = 50.0;
    
    // Timing
    int engagementTimeMs = 30000;  // 30 seconds default
    int cooldownTimeMs = 5000;     // 5 seconds cooldown
    
    // Range
    double effectiveRangeM = 2000.0;
    double minimumRangeM = 50.0;
};
```

#### 8.3.2 Characteristics

| Parameter | Value | Description |
|-----------|-------|-------------|
| Effectiveness | 85% | Probability of disruption |
| Min Range | 50 m | Minimum engagement distance |
| Max Range | 2000 m | Maximum effective range |
| Default Power | 50 W | Standard output power |
| Max Power | 100 W | Maximum output power |
| Engagement Time | 30 s | Default jam duration |
| Cooldown | 5 s | Post-engagement cooldown |

### 8.4 Kinetic Interceptor

#### 8.4.1 Configuration

```cpp
struct KineticInterceptorConfig {
    QString host = "127.0.0.1";
    int port = 5011;
    
    int magazineCapacity = 10;
    double minRangeM = 100.0;
    double maxRangeM = 1500.0;
    double maxAltitudeM = 500.0;
    
    int launchTimeMs = 2000;    // Time from command to launch
    int flightTimeMs = 10000;   // Max flight time
    int reloadTimeMs = 30000;   // Single round reload time
    
    double interceptProbability = 0.85;
};
```

#### 8.4.2 Launch Phase State Machine

```cpp
enum class LaunchPhase {
    Idle,        // Ready state
    Arming,      // Pre-launch arming
    Launching,   // Round departing launcher
    InFlight,    // Interceptor flying to target
    Terminal,    // Final approach
    Complete     // Impact or miss
};
```

#### 8.4.3 Characteristics

| Parameter | Value | Description |
|-----------|-------|-------------|
| Effectiveness | 85% | Intercept probability |
| Min Range | 100 m | Minimum engagement distance |
| Max Range | 1500 m | Maximum engagement distance |
| Max Altitude | 500 m | Maximum target altitude |
| Magazine | 10 rounds | Standard load |
| Launch Time | 2 s | Command to launch delay |
| Flight Time | 10 s | Maximum time to target |
| Reload Time | 30 s | Per-round reload |

### 8.5 Directed Energy System

#### 8.5.1 Configuration

```cpp
struct DESystemConfig {
    QString host = "127.0.0.1";
    int port = 5012;
    
    double maxPowerKW = 30.0;
    double minRangeM = 100.0;
    double maxRangeM = 1000.0;
    double beamDiameterMrad = 2.0;
    
    int maxEngagementTimeMs = 10000;
    int cooldownTimeMs = 15000;
    
    double dwellTimeRequiredS = 2.0;  // Time on target for effect
};
```

#### 8.5.2 Characteristics

| Parameter | Value | Description |
|-----------|-------|-------------|
| Effectiveness | 75% | Probability of effect |
| Min Range | 100 m | Minimum engagement distance |
| Max Range | 1000 m | Maximum effective range |
| Max Power | 30 kW | Maximum output |
| Beam Diameter | 2.0 mrad | Beam divergence |
| Dwell Time | 2.0 s | Required time on target |
| Max Engagement | 10 s | Maximum engagement duration |
| Cooldown | 15 s | Post-engagement cooldown |

---

## 9. Video Management Subsystem

### 9.1 Overview

The `VideoStreamManager` handles multiple video sources with support for RTSP streams, GigE cameras, file sources, and simulated video.

### 9.2 System Constants

```cpp
class VideoStreamManager : public QObject {
public:
    static constexpr int MAX_STREAMS = 16;        // Maximum video streams
    static constexpr int MAX_DISPLAY_STREAMS = 9;  // Simultaneous display
};
```

### 9.3 Camera Definition

```cpp
struct CameraDefinition {
    QString cameraId;          // Unique camera identifier
    QString name;              // Display name
    QString streamUrl;         // Stream URL (RTSP, file, etc.)
    QString sourceType;        // "RTSP", "GigE", "FILE", "USB"
    GeoPosition position;      // Camera location
    bool hasPTZ = false;       // Pan-Tilt-Zoom capability
    QString ptzProtocol;       // "ONVIF", "Pelco-D", etc.
    QString ptzAddress;        // PTZ control address
    int ptzPort = 80;          // PTZ control port
    QVariantMap metadata;      // Additional properties
};
```

### 9.4 Video Source Status

```cpp
enum class VideoSourceStatus {
    Disconnected = 0,  // No connection
    Connecting,        // Establishing connection
    Connected,         // Connected but not streaming
    Streaming,         // Actively receiving frames
    Paused,            // Stream paused
    Error,             // Error condition
    Reconnecting       // Attempting reconnection
};
```

### 9.5 Video Source Statistics

```cpp
struct VideoSourceStats {
    qint64 framesReceived = 0;    // Total frames received
    qint64 framesDropped = 0;     // Dropped frames
    double fps = 0.0;             // Current frame rate
    double bitrate = 0.0;         // Stream bitrate
    int width = 0;                // Frame width
    int height = 0;               // Frame height
    qint64 latencyMs = 0;         // End-to-end latency
    qint64 lastFrameTime = 0;     // Last frame timestamp
};
```

### 9.6 Video Source Configuration

| Parameter | Default | Description |
|-----------|---------|-------------|
| Target FPS | 30 | Desired frame rate |
| Buffer Size | 3 frames | Frame buffer depth |
| Auto Reconnect | true | Automatic reconnection |
| Reconnect Interval | 5000 ms | Reconnection attempt delay |

### 9.7 Recording Settings (from config)

```json
{
    "video": {
        "defaultFps": 30,
        "maxStreams": 16,
        "maxDisplayStreams": 9,
        "recordingPath": "recordings/",
        "preBufferSeconds": 30,
        "postBufferSeconds": 30,
        "autoReconnect": true,
        "reconnectIntervalMs": 5000
    }
}
```

---

## 10. Network Communications

### 10.1 Message Protocol

#### 10.1.1 Message Types

```cpp
enum class MessageType : quint16 {
    Heartbeat = 0x0001,
    
    // Track messages (0x01xx)
    TrackUpdate = 0x0100,
    TrackCreate = 0x0101,
    TrackDelete = 0x0102,
    TrackClassify = 0x0103,
    
    // Sensor messages (0x02xx)
    SensorStatus = 0x0200,
    SensorDetection = 0x0201,
    SensorConfig = 0x0202,
    
    // Effector messages (0x03xx)
    EffectorStatus = 0x0300,
    EffectorCommand = 0x0301,
    EffectorResult = 0x0302,
    
    // Video messages (0x04xx)
    VideoFrame = 0x0400,
    VideoConfig = 0x0401,
    PTZCommand = 0x0402,
    
    // System messages (0x05xx)
    Alert = 0x0500,
    Config = 0x0501,
    Log = 0x0502,
    
    // Command messages (0x06xx)
    EngagementRequest = 0x0600,
    EngagementAuthorize = 0x0601,
    EngagementAbort = 0x0602,
    
    Unknown = 0xFFFF
};
```

#### 10.1.2 Message Structure

```cpp
struct Message {
    MessageType type = MessageType::Unknown;
    quint32 sequenceNumber = 0;      // Sequence for ordering/dedup
    qint64 timestamp = 0;            // Message timestamp
    QString sourceId;                 // Originating system ID
    QVariantMap payload;             // Message data
};
```

#### 10.1.3 Protocol Header

```cpp
static constexpr quint32 MAGIC = 0x43554153;  // "CUAS"
static constexpr int HEADER_SIZE = 20;
// Header format:
// - Magic: 4 bytes (0x43554153)
// - Type: 2 bytes
// - Sequence: 4 bytes
// - Timestamp: 8 bytes
// - Length: 4 bytes (followed by -2 for checksum)
```

### 10.2 Connection Configuration

```cpp
struct ConnectionConfig {
    QString connectionId;              // Unique connection ID
    QString name;                      // Display name
    QString host;                      // Remote host
    int port;                          // Remote port
    bool useTcp = true;                // TCP (true) or UDP (false)
    bool autoReconnect = true;         // Auto-reconnect enabled
    int reconnectIntervalMs = 5000;    // Reconnect delay
    int timeoutMs = 3000;              // Connection timeout
    QString username;                  // Authentication user
    QString password;                  // Authentication password
};
```

### 10.3 Bandwidth Monitoring

```cpp
struct BandwidthStats {
    qint64 bytesSent = 0;          // Total bytes transmitted
    qint64 bytesReceived = 0;      // Total bytes received
    double sendRateBps = 0.0;      // Send rate (bytes/sec)
    double receiveRateBps = 0.0;   // Receive rate (bytes/sec)
};
```

---

## 11. Simulation Framework

### 11.1 Track Simulator

#### 11.1.1 Manual Target Parameters

```cpp
struct ManualTargetParams {
    double rangeM = 2000.0;         // Distance from base (meters)
    double bearingDeg = 0.0;        // Bearing from base (0=North)
    double altitudeM = 150.0;       // Altitude AGL (meters)
    double speedMps = 15.0;         // Speed (m/s)
    double headingDeg = 180.0;      // Target heading (0=North)
    double climbRateMps = 0.0;      // Vertical speed (+ = climb)
    TrackClassification classification = TrackClassification::Pending;
};
```

#### 11.1.2 Simulated Target Structure

```cpp
struct SimulatedTarget {
    QString id;                          // Target identifier
    GeoPosition position;                // Current position
    VelocityVector velocity;             // Current velocity
    TrackClassification classification;  // Target class
    bool active = true;                  // Active flag
};
```

#### 11.1.3 Configuration

| Parameter | Default | Description |
|-----------|---------|-------------|
| Update Rate | 10 Hz | Position update frequency |
| Auto Spawn | true | Automatic target generation |
| Spawn Interval | 5000 ms | Time between spawns |
| Max Targets | 10 | Maximum concurrent targets |

#### 11.1.4 Target Generation Algorithm

```cpp
void TrackSimulator::spawnTarget() {
    // Spawn at edge of detection range
    double range = 2000.0 + gen->generateDouble() * 1000.0;  // 2000-3000m
    double bearing = gen->generateDouble() * 360.0;
    
    // Altitude: 50-300m AGL
    target.position.altitude = m_basePosition.altitude + 
                               50.0 + gen->generateDouble() * 250.0;
    
    // Speed: 8-20 m/s toward base
    double speed = 8.0 + gen->generateDouble() * 12.0;
    double velBearing = bearing + 180.0 + (gen->generateDouble() * 40.0 - 20.0);
    
    // Classification: 70% hostile, 30% pending
    target.classification = gen->bounded(100) < 70 ? 
                           TrackClassification::Hostile : 
                           TrackClassification::Pending;
}
```

### 11.2 Sensor Simulator

#### 11.2.1 Simulation Parameters

```cpp
// Simulation characteristics
void setRealisticMode(bool enable);           // Enable realistic behavior
void setClutterLevel(double level);           // 0-1 clutter intensity
void setNoiseLevel(double level);             // 0-1 noise level
void setDetectionProbability(double prob);    // 0-1 detection probability

// Defaults
bool m_realisticMode = true;
double m_clutterLevel = 0.1;
double m_noiseLevel = 0.05;
double m_detectionProbability = 0.9;
```

#### 11.2.2 Radar Simulation State

```cpp
struct RadarSimState {
    bool active = false;
    int detectedTargets = 0;
    int clutterReturns = 0;
    double signalQuality = 1.0;
    QList<SimulatedRadarTarget> currentTargets;
    qint64 lastScanTime = 0;
    double scanAngle = 0.0;  // For rotating radar
};
```

#### 11.2.3 RF Detector Simulation State

```cpp
struct RFDetectorSimState {
    bool active = false;
    int detectedEmissions = 0;
    double noiseFloor = -90.0;  // dBm
    QList<SimulatedRFEmission> currentEmissions;
    QHash<QString, QString> identifiedProtocols;
};
```

#### 11.2.4 Camera Simulation State

```cpp
struct CameraSimState {
    bool active = false;
    int detectedObjects = 0;
    double pan = 0.0;
    double tilt = 0.0;
    double zoom = 1.0;
    QList<SimulatedVisualTarget> visibleTargets;
    QString trackingTargetId;
    bool autoTrackEnabled = false;
};
```

---

## 12. User Interface Design

### 12.1 Main Window Layout

```
┌─────────────────────────────────────────────────────────────────────────┐
│ Menu Bar: File | Edit | View | Simulation | Sensors | Effectors | Help  │
├─────────────────────────────────────────────────────────────────────────┤
│ Main Toolbar: [Start] [Stop] [Pause] [Reset] | [Config] [Save] | [Info] │
│ PPI Toolbar: [Map/PPI] [Mode] [Range] [Sweep]                           │
├───────────────────────────────────────────────────────┬─────────────────┤
│                                                       │  Track List     │
│                                                       │  ┌───────────┐  │
│                                                       │  │ TRK-0001  │  │
│             Main Display Area                         │  │ TRK-0002  │  │
│        (Map Widget / PPI Display)                     │  │ TRK-0003  │  │
│                                                       │  └───────────┘  │
│                                                       ├─────────────────┤
│                                                       │  Track Detail   │
│                                                       │  ┌───────────┐  │
│                                                       │  │ Position  │  │
├───────────────────────────────────────────────────────┤  │ Velocity  │  │
│  Primary Video    │  Video Grid (3x3)                 │  │ Threat    │  │
│  ┌─────────────┐  │  ┌───┐ ┌───┐ ┌───┐               │  └───────────┘  │
│  │             │  │  │   │ │   │ │   │               ├─────────────────┤
│  │             │  │  ├───┤ ├───┤ ├───┤               │  Sensor Status  │
│  │             │  │  │   │ │   │ │   │               │  ┌───────────┐  │
│  └─────────────┘  │  └───┘ └───┘ └───┘               │  │ RADAR: OK │  │
│                                                       │  │ RF: OK    │  │
├───────────────────────────────────────────────────────┤  └───────────┘  │
│  Alert Queue                                          ├─────────────────┤
│  ┌─────────────────────────────────────────────────┐  │ Effector Panel  │
│  │ [!] CRITICAL: Track TRK-0001 in critical zone   │  │ ┌───────────┐  │
│  │ [!] WARNING: Track TRK-0002 approaching         │  │ │ Jammer    │  │
│  └─────────────────────────────────────────────────┘  │ │ Kinetic   │  │
├───────────────────────────────────────────────────────┤  └───────────┘  │
│ Status: Tracks: 3 | Threats: 2 | Sim: Running | 14:32:45 UTC           │
└─────────────────────────────────────────────────────────────────────────┘
```

### 12.2 Dock Widget Configuration

| Dock Widget | Position | Content |
|-------------|----------|---------|
| Track List | Right | List of active tracks |
| Track Detail | Right | Selected track details |
| Sensor Status | Right | Sensor health indicators |
| Camera Status | Right | Camera stream status |
| Effector Control | Right | Effector status/control |
| Alert Queue | Bottom | Threat alerts |
| Primary Video | Bottom-Left | Main video feed |
| Video Grid | Bottom-Center | Multi-camera view |

### 12.3 Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| F11 | Toggle full-screen video |
| Ctrl+O | Load configuration |
| Ctrl+S | Save configuration |
| Ctrl+Q | Exit application |

### 12.4 Map Widget Functions

```cpp
class MapWidget : public QWidget {
public:
    void setCenter(const GeoPosition& pos);
    void setZoom(double zoom);
    void selectTrack(const QString& trackId);
    
public slots:
    void addTrack(const QString& trackId);
    void updateTrack(const QString& trackId);
    void removeTrack(const QString& trackId);
    void clearTracks();
    
signals:
    void trackSelected(const QString& trackId);
    void mapClicked(const GeoPosition& pos);
    void zoomChanged(double zoom);
    
private:
    QPointF geoToScreen(const GeoPosition& pos) const;
    GeoPosition screenToGeo(const QPointF& screen) const;
    void drawGrid(QPainter& painter);
    void drawTracks(QPainter& painter);
    void drawDefendedArea(QPainter& painter);
    QColor colorForClassification(TrackClassification cls) const;
};
```

### 12.5 Track Color Coding

| Classification | Color |
|----------------|-------|
| Unknown | Gray |
| Friendly | Green |
| Hostile | Red |
| Pending | Yellow |
| Neutral | Blue |

---

## 13. Configuration Management

### 13.1 Configuration File Structure

The system uses JSON configuration files stored in the `config/` directory:

#### 13.1.1 system.json

```json
{
    "system": {
        "updateRateHz": 10,
        "logLevel": "INFO",
        "logPath": "logs/",
        "dataPath": "data/"
    },
    "trackManager": {
        "correlationDistanceM": 100.0,
        "correlationVelocityMps": 10.0,
        "coastingTimeoutMs": 5000,
        "dropTimeoutMs": 15000,
        "maxCoastCount": 10,
        "enableKalmanFilter": true,
        "maxTracks": 200
    },
    "threatAssessor": {
        "assessmentIntervalMs": 500,
        "alertQueueMaxSize": 100,
        "autoSlewToHighestThreat": true,
        "highThreatThreshold": 4,
        "headingToleranceDeg": 30.0
    },
    "video": {
        "defaultFps": 30,
        "maxStreams": 16,
        "maxDisplayStreams": 9,
        "recordingPath": "recordings/",
        "preBufferSeconds": 30,
        "postBufferSeconds": 30,
        "autoReconnect": true,
        "reconnectIntervalMs": 5000
    },
    "map": {
        "defaultCenterLat": 34.0522,
        "defaultCenterLon": -118.2437,
        "defaultZoom": 15,
        "updateRateHz": 10
    },
    "engagement": {
        "authorizationTimeoutSeconds": 60,
        "autoRecommendEffector": true
    }
}
```

#### 13.1.2 sensors.json

```json
{
    "sensors": [
        {
            "sensorId": "RADAR-001",
            "name": "Primary Surveillance Radar",
            "type": "RADAR",
            "host": "192.168.1.50",
            "port": 5001,
            "position": {
                "latitude": 34.0522,
                "longitude": -118.2437,
                "altitude": 130.0
            },
            "config": {
                "minRangeM": 50.0,
                "maxRangeM": 5000.0,
                "updateRateHz": 10,
                "filterClutter": true,
                "clutterThreshold": 0.1
            }
        }
    ]
}
```

#### 13.1.3 effectors.json

```json
{
    "effectors": [
        {
            "effectorId": "JAMMER-001",
            "name": "RF Jammer Alpha",
            "type": "RF_JAMMER",
            "host": "192.168.1.60",
            "port": 5010,
            "position": {
                "latitude": 34.0522,
                "longitude": -118.2437,
                "altitude": 110.0
            },
            "config": {
                "minFrequencyMHz": 2400.0,
                "maxFrequencyMHz": 5800.0,
                "maxPowerW": 100.0,
                "effectiveRangeM": 2000.0,
                "engagementTimeMs": 30000,
                "cooldownTimeMs": 5000
            }
        }
    ]
}
```

#### 13.1.4 cameras.json

```json
{
    "cameras": [
        {
            "cameraId": "CAM-001",
            "name": "North Tower PTZ",
            "streamUrl": "rtsp://192.168.1.101/stream1",
            "sourceType": "RTSP",
            "position": {
                "latitude": 34.0532,
                "longitude": -118.2437,
                "altitude": 120.0
            },
            "hasPTZ": true,
            "ptzProtocol": "ONVIF",
            "ptzAddress": "192.168.1.101",
            "ptzPort": 80
        }
    ]
}
```

#### 13.1.5 defended_assets.json

```json
{
    "defendedAssets": [
        {
            "id": "BASE-01",
            "name": "Main Installation HQ",
            "position": {
                "latitude": 34.0522,
                "longitude": -118.2437,
                "altitude": 100.0
            },
            "criticalRadiusM": 500.0,
            "warningRadiusM": 1500.0,
            "priorityLevel": 5
        }
    ],
    "noFireZones": [
        {
            "id": "NFZ-01",
            "name": "Residential Area",
            "polygon": [
                {"latitude": 34.0540, "longitude": -118.2500},
                {"latitude": 34.0540, "longitude": -118.2460},
                {"latitude": 34.0560, "longitude": -118.2460},
                {"latitude": 34.0560, "longitude": -118.2500}
            ]
        }
    ]
}
```

### 13.2 ConfigManager Class

```cpp
class ConfigManager : public QObject {
    Q_OBJECT
    
public:
    static ConfigManager& instance();  // Singleton access
    
    bool loadConfig(const QString& path);
    bool saveConfig(const QString& path);
    bool loadDefaults();
    
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setValue(const QString& key, const QVariant& value);
    
    QJsonObject section(const QString& name) const;
    void setSection(const QString& name, const QJsonObject& section);
    
signals:
    void configChanged(const QString& key);
    void configLoaded();
};
```

---

## 14. Filter and Algorithm Implementations

### 14.1 2D Kalman Filter

The Kalman filter provides position smoothing and velocity estimation:

```cpp
class KalmanFilter2D {
public:
    void initialize(double x, double y);
    void update(double measX, double measY);
    void predict(double dt = 0.1);
    
    double stateX() const { return m_x; }
    double stateY() const { return m_y; }
    double velocityX() const { return m_vx; }
    double velocityY() const { return m_vy; }
    
    void setProcessNoise(double q) { m_processNoise = q; }
    void setMeasurementNoise(double r) { m_measurementNoise = r; }
    
private:
    // State: [x, y, vx, vy]
    double m_x, m_y;           // Position
    double m_vx, m_vy;         // Velocity
    
    // Covariance (simplified)
    double m_px, m_py;         // Position variance
    double m_pvx, m_pvy;       // Velocity variance
    
    double m_processNoise = 0.1;       // Q
    double m_measurementNoise = 1.0;   // R
};
```

#### 14.1.1 Prediction Step

```cpp
void KalmanFilter2D::predict(double dt) {
    if (!m_initialized) return;
    
    // Predict state
    m_x += m_vx * dt;
    m_y += m_vy * dt;
    
    // Predict covariance
    m_px += m_pvx * dt * dt + m_processNoise;
    m_py += m_pvy * dt * dt + m_processNoise;
    m_pvx += m_processNoise;
    m_pvy += m_processNoise;
}
```

#### 14.1.2 Update Step

```cpp
void KalmanFilter2D::update(double measX, double measY) {
    if (!m_initialized) {
        initialize(measX, measY);
        return;
    }
    
    predict();  // Predict first
    
    // Kalman gain
    double kx = m_px / (m_px + m_measurementNoise);
    double ky = m_py / (m_py + m_measurementNoise);
    
    // Innovation
    double innovationX = measX - m_x;
    double innovationY = measY - m_y;
    
    // Velocity gain
    double dt = 0.1;
    double kvx = 0.5;
    
    // State update
    m_x += kx * innovationX;
    m_y += ky * innovationY;
    m_vx += kvx * innovationX / dt;
    m_vy += kvx * innovationY / dt;
    
    // Covariance update
    m_px *= (1.0 - kx);
    m_py *= (1.0 - ky);
}
```

### 14.2 Alpha-Beta Filter

A simpler alternative for single-variable smoothing:

```cpp
class AlphaBetaFilter {
public:
    AlphaBetaFilter(double alpha = 0.85, double beta = 0.005);
    
    void initialize(double value);
    void update(double measurement, double dt = 0.1);
    
    double value() const { return m_value; }
    double velocity() const { return m_velocity; }
    
private:
    double m_value = 0.0;
    double m_velocity = 0.0;
    double m_alpha;   // Position smoothing factor
    double m_beta;    // Velocity smoothing factor
};
```

#### 14.2.1 Update Algorithm

```cpp
void AlphaBetaFilter::update(double measurement, double dt) {
    if (!m_initialized) {
        initialize(measurement);
        return;
    }
    
    // Predict
    double predicted = m_value + m_velocity * dt;
    
    // Residual
    double residual = measurement - predicted;
    
    // Update
    m_value = predicted + m_alpha * residual;
    m_velocity = m_velocity + (m_beta / dt) * residual;
}
```

**Default Parameters:**
- Alpha: 0.85 (position smoothing)
- Beta: 0.005 (velocity smoothing)

### 14.3 Coordinate Conversion Utilities

```cpp
class CoordinateUtils {
public:
    // Distance using Haversine formula
    static double haversineDistance(double lat1, double lon1, 
                                    double lat2, double lon2);
    
    // Bearing calculation
    static double bearing(double lat1, double lon1, 
                         double lat2, double lon2);
    
    // Position from bearing and distance
    static GeoPosition positionFromBearingDistance(
        const GeoPosition& origin,
        double bearingDeg, 
        double distanceM);
    
    // Local coordinate conversions
    static QPointF geoToLocal(const GeoPosition& pos, 
                              const GeoPosition& origin);
    static GeoPosition localToGeo(const QPointF& local, 
                                  const GeoPosition& origin);
    
    // Screen coordinate conversions
    static QPointF geoToScreen(const GeoPosition& pos, 
                               const GeoPosition& center,
                               double zoom, 
                               const QSize& screenSize);
    static GeoPosition screenToGeo(const QPointF& screen, 
                                   const GeoPosition& center,
                                   double zoom, 
                                   const QSize& screenSize);
    
    // Format conversions
    static QString toMGRS(const GeoPosition& pos, int precision = 5);
    static GeoPosition fromMGRS(const QString& mgrs);
    static QString toDMS(double degrees, bool isLatitude);
    static double fromDMS(const QString& dms);
    
    // Constants
    static constexpr double EARTH_RADIUS_M = 6371000.0;
    static constexpr double DEG_TO_M_LAT = 111000.0;
    
    static double degToMeterLon(double latitude);
};
```

---

## 15. System Thresholds and Constants

### 15.1 Complete Threshold Reference

| Category | Parameter | Value | Unit | Description |
|----------|-----------|-------|------|-------------|
| **Track Management** | | | | |
| | Update Rate | 10 | Hz | Track processing cycle rate |
| | Correlation Distance | 100.0 | m | Max distance for track correlation |
| | Correlation Velocity | 10.0 | m/s | Max velocity diff for correlation |
| | Coasting Timeout | 5000 | ms | Time before track enters coasting |
| | Drop Timeout | 15000 | ms | Time before track is dropped |
| | Max Coast Count | 10 | cycles | Max coast cycles before drop |
| | Max Tracks | 200 | count | Maximum concurrent tracks |
| | History Retention | 60000 | ms | Position history retention |
| | Min Correlation Score | 0.5 | 0-1 | Minimum score for correlation |
| **Threat Assessment** | | | | |
| | Assessment Interval | 500 | ms | Threat evaluation cycle |
| | Alert Queue Max | 100 | count | Maximum pending alerts |
| | High Threat Threshold | 4 | 1-5 | Level triggering high-threat |
| | Heading Tolerance | 30.0 | deg | Approach heading tolerance |
| | Alert Dedup Window | 30 | sec | Duplicate alert prevention |
| | Critical Radius (default) | 500.0 | m | Inner defense zone |
| | Warning Radius (default) | 1500.0 | m | Outer warning zone |
| | High Speed Threshold | 30.0 | m/s | Speed for +1 threat |
| | Low Confidence Threshold | 0.5 | 0-1 | Classification confidence |
| **Engagement** | | | | |
| | Authorization Timeout | 60 | sec | Time for operator approval |
| | Completion Check Interval | 100 | ms | Engagement status polling |
| **Radar Sensor** | | | | |
| | Min Range | 50.0 | m | Minimum detection range |
| | Max Range | 5000.0 | m | Maximum detection range |
| | Reconnect Interval | 5000 | ms | Connection retry delay |
| | Timeout | 3000 | ms | Connection timeout |
| | Clutter Threshold | 0.1 | 0-1 | Clutter filtering level |
| **RF Detector** | | | | |
| | Min Frequency | 900.0 | MHz | Lower frequency bound |
| | Max Frequency | 6000.0 | MHz | Upper frequency bound |
| | Signal Threshold | -80.0 | dBm | Minimum detection level |
| | DF Accuracy | 5.0 | deg | Direction finding precision |
| | Classification Threshold | 0.7 | 0-1 | Auto-hostile classification |
| **RF Jammer** | | | | |
| | Effectiveness | 0.85 | 0-1 | Success probability |
| | Min Range | 50.0 | m | Minimum engagement range |
| | Max Range | 2000.0 | m | Maximum effective range |
| | Default Power | 50.0 | W | Standard output power |
| | Max Power | 100.0 | W | Maximum output power |
| | Engagement Time | 30000 | ms | Default jam duration |
| | Cooldown Time | 5000 | ms | Post-engagement cooldown |
| **Kinetic Interceptor** | | | | |
| | Effectiveness | 0.85 | 0-1 | Intercept probability |
| | Min Range | 100.0 | m | Minimum engagement range |
| | Max Range | 1500.0 | m | Maximum engagement range |
| | Max Altitude | 500.0 | m | Maximum target altitude |
| | Magazine Capacity | 10 | rounds | Standard load |
| | Launch Time | 2000 | ms | Command to launch delay |
| | Flight Time | 10000 | ms | Maximum time to target |
| | Reload Time | 30000 | ms | Per-round reload |
| **Directed Energy** | | | | |
| | Effectiveness | 0.75 | 0-1 | Success probability |
| | Min Range | 100.0 | m | Minimum engagement range |
| | Max Range | 1000.0 | m | Maximum effective range |
| | Max Power | 30.0 | kW | Maximum output |
| | Beam Diameter | 2.0 | mrad | Beam divergence |
| | Dwell Time Required | 2.0 | sec | Time on target for effect |
| | Max Engagement Time | 10000 | ms | Maximum engagement |
| | Cooldown Time | 15000 | ms | Post-engagement cooldown |
| **Video System** | | | | |
| | Max Streams | 16 | count | Maximum video streams |
| | Max Display Streams | 9 | count | Simultaneous display |
| | Default FPS | 30 | fps | Target frame rate |
| | Buffer Size | 3 | frames | Frame buffer depth |
| | Pre-Buffer | 30 | sec | Pre-incident recording |
| | Post-Buffer | 30 | sec | Post-incident recording |
| | Reconnect Interval | 5000 | ms | Stream reconnection delay |
| | Latency Target | <300 | ms | End-to-end video latency |
| **Simulation** | | | | |
| | Clutter Level | 0.1 | 0-1 | Simulated clutter |
| | Noise Level | 0.05 | 0-1 | Simulated noise |
| | Detection Probability | 0.9 | 0-1 | Simulated detection rate |
| | Target Speed Range | 8-20 | m/s | Simulated drone speed |
| | Spawn Interval | 5000 | ms | Target generation rate |
| | Max Targets | 10 | count | Maximum simulated targets |
| | Noise Floor (RF) | -90.0 | dBm | Simulated RF noise |
| **Map Display** | | | | |
| | Default Zoom | 15 | level | Initial zoom level |
| | Update Rate | 10 | Hz | Display refresh rate |
| **Kalman Filter** | | | | |
| | Process Noise (Q) | 0.1 | - | State transition noise |
| | Measurement Noise (R) | 1.0 | - | Observation noise |
| | Velocity Gain | 0.5 | - | Velocity estimation gain |
| **Alpha-Beta Filter** | | | | |
| | Alpha | 0.85 | 0-1 | Position smoothing |
| | Beta | 0.005 | 0-1 | Velocity smoothing |

### 15.2 Geographic Constants

| Constant | Value | Description |
|----------|-------|-------------|
| Earth Radius | 6,371,000 m | Mean Earth radius |
| Degrees to Meters (Lat) | 111,000 m/° | Latitude conversion |
| Degrees to Meters (Lon) | 111,000 × cos(lat) m/° | Longitude conversion |

---

## 16. Performance Specifications

### 16.1 System Requirements

| Requirement | Specification |
|-------------|---------------|
| Update Rate | 10+ Hz for tracks and map |
| Track Capacity | 100+ simultaneous tracks |
| Video Streams | Up to 16 (9 displayed) |
| Video Latency | < 300 ms end-to-end |
| Response Time | < 100 ms for UI interactions |

### 16.2 Resource Budgets

| Component | Memory | CPU |
|-----------|--------|-----|
| TrackManager | ~10 KB/track | ~5% per 100 tracks |
| VideoStreamManager | ~50 MB/stream | ~10% per HD stream |
| MapWidget | ~20 MB | ~5% at 10 Hz |
| Network | 1 KB/message typical | ~2% |

### 16.3 Timing Requirements

| Operation | Target | Maximum |
|-----------|--------|---------|
| Track correlation | 1 ms | 5 ms |
| Threat assessment | 5 ms | 20 ms |
| Video frame decode | 10 ms | 33 ms |
| Map redraw | 10 ms | 50 ms |
| Effector command | 10 ms | 100 ms |

---

## 17. Appendices

### 17.1 Signal/Slot Connections Summary

#### TrackManager Signals
- `trackCreated(QString trackId)`
- `trackUpdated(QString trackId)`
- `trackClassificationChanged(QString trackId, TrackClassification cls)`
- `trackThreatLevelChanged(QString trackId, int level)`
- `trackStateChanged(QString trackId, TrackState state)`
- `trackDropped(QString trackId)`
- `trackCountChanged(int count)`
- `highThreatDetected(QString trackId, int level)`

#### ThreatAssessor Signals
- `threatLevelChanged(QString trackId, int oldLevel, int newLevel)`
- `newAlert(ThreatAlert alert)`
- `alertAcknowledged(QString alertId)`
- `highThreatDetected(QString trackId)`
- `metricsUpdated(ThreatMetrics metrics)`
- `slewCameraRequest(QString cameraId, GeoPosition pos)`

#### EngagementManager Signals
- `stateChanged(EngagementState state)`
- `engagementChanged(QString engagementId)`
- `trackSelected(QString trackId)`
- `effectorSelected(QString effectorId)`
- `effectorRecommended(QString effectorId, QString reason)`
- `authorizationRequested(AuthorizationRequest request)`
- `authorizationGranted(QString operatorId)`
- `authorizationDenied(QString reason)`
- `engagementStarted(QString engagementId)`
- `engagementCompleted(QString engagementId, BDAResult result)`
- `engagementAborted(QString engagementId, QString reason)`

### 17.2 Qt Module Dependencies

```cmake
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Sql
    Multimedia
    MultimediaWidgets
    OpenGLWidgets
    Concurrent
    StateMachine
)
```

### 17.3 Build Configuration (CMakeLists.txt Summary)

```cmake
cmake_minimum_required(VERSION 3.16)
project(CounterUAS_C2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
```

### 17.4 Error Codes and Messages

| Code | Category | Message Template |
|------|----------|-----------------|
| E001 | Track | Maximum track limit reached |
| E002 | Track | Track not found: %1 |
| E003 | Sensor | Sensor connection failed: %1 |
| E004 | Effector | Effector not ready: %1 |
| E005 | Effector | Target out of range |
| E006 | Video | Stream connection failed: %1 |
| E007 | Config | Configuration load failed: %1 |
| E008 | Engagement | Authorization timeout |
| E009 | Network | Connection lost: %1 |

### 17.5 Log Level Definitions

| Level | Priority | Usage |
|-------|----------|-------|
| DEBUG | 0 | Development/troubleshooting |
| INFO | 1 | Normal operations |
| WARNING | 2 | Recoverable issues |
| ERROR | 3 | Failures requiring attention |
| CRITICAL | 4 | System-threatening conditions |

---

## Document Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-01-18 | System | Initial comprehensive documentation |

---

**END OF DOCUMENT**
