cmake_minimum_required(VERSION 3.16)

# Early compiler check before project() to provide better error messages
if(NOT DEFINED CMAKE_CXX_COMPILER)
    # Try to find a C++ compiler
    if(WIN32)
        find_program(CXX_COMPILER_PATH
            NAMES cl.exe g++.exe clang++.exe
            PATHS
                "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/*/bin/Hostx64/x64"
                "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/*/bin/Hostx64/x64"
                "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/Enterprise/VC/Tools/MSVC/*/bin/Hostx64/x64"
                "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/*/VC/Tools/MSVC/*/bin/Hostx64/x64"
        )
    else()
        find_program(CXX_COMPILER_PATH NAMES g++ clang++ c++)
    endif()
endif()

project(CounterUAS_C2 VERSION 1.0.0 LANGUAGES CXX)

# Verify compiler was found and provide helpful error message
if(NOT CMAKE_CXX_COMPILER)
    if(WIN32)
        message(FATAL_ERROR
            "No C++ compiler found!\n"
            "Please install one of the following:\n"
            "  1. Visual Studio 2022 with 'Desktop development with C++' workload:\n"
            "     - Run Visual Studio Installer\n"
            "     - Select 'Modify' on your Visual Studio installation\n"
            "     - Check 'Desktop development with C++' workload\n"
            "     - Click 'Modify' to install\n"
            "  2. Or install Build Tools for Visual Studio 2022:\n"
            "     https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022\n"
            "  3. Or use MinGW-w64/MSYS2:\n"
            "     https://www.msys2.org/\n"
            "\n"
            "After installation, restart your terminal/IDE and try again."
        )
    else()
        message(FATAL_ERROR
            "No C++ compiler found!\n"
            "Please install a C++ compiler:\n"
            "  Ubuntu/Debian: sudo apt install build-essential\n"
            "  Fedora/RHEL:   sudo dnf install gcc-c++\n"
            "  macOS:         xcode-select --install\n"
        )
    endif()
endif()

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages - try Qt6 first, then Qt5
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Network
    Sql
    Multimedia
    MultimediaWidgets
    OpenGLWidgets
    Concurrent
    SerialPort
    QUIET
)

if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Using Qt6")
    # StateMachine is a separate module in Qt6
    find_package(Qt6 COMPONENTS StateMachine QUIET)
    if(Qt6StateMachine_FOUND)
        message(STATUS "Qt6 StateMachine found")
    else()
        message(STATUS "Qt6 StateMachine not found - some features may be limited")
    endif()
else()
    find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        Sql
        Multimedia
        MultimediaWidgets
        OpenGL
        Concurrent
        SerialPort
    )
    set(QT_VERSION_MAJOR 5)
    message(STATUS "Using Qt5")
endif()

# Optional: Qt Location for map display
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 COMPONENTS Positioning Location QUIET)
else()
    find_package(Qt5 COMPONENTS Positioning Location QUIET)
endif()

# Source files
set(CORE_SOURCES
    src/core/Track.cpp
    src/core/TrackManager.cpp
    src/core/ThreatAssessor.cpp
    src/core/EngagementManager.cpp
)

set(SENSOR_SOURCES
    src/sensors/SensorInterface.cpp
    src/sensors/RadarSensor.cpp
    src/sensors/RFDetector.cpp
    src/sensors/CameraSystem.cpp
)

set(VIDEO_SOURCES
    src/video/VideoStreamManager.cpp
    src/video/VideoSource.cpp
    src/video/RTSPVideoSource.cpp
    src/video/GigEVideoSource.cpp
    src/video/FileVideoSource.cpp
    src/video/SimulationVideoSource.cpp
    src/video/VideoDecoder.cpp
    src/video/VideoOverlayRenderer.cpp
    src/video/VideoRecorder.cpp
    src/video/PTZController.cpp
    src/video/CameraSlewController.cpp
)

set(EFFECTOR_SOURCES
    src/effectors/EffectorInterface.cpp
    src/effectors/RFJammer.cpp
    src/effectors/KineticInterceptor.cpp
    src/effectors/DirectedEnergySystem.cpp
)

set(NETWORK_SOURCES
    src/network/NetworkManager.cpp
    src/network/MessageProtocol.cpp
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/MapWidget.cpp
    src/ui/VideoDisplayWidget.cpp
    src/ui/VideoGridWidget.cpp
    src/ui/VideoControlBar.cpp
    src/ui/PTZControlWidget.cpp
    src/ui/TrackListWidget.cpp
    src/ui/TrackDetailPanel.cpp
    src/ui/SensorStatusPanel.cpp
    src/ui/CameraStatusPanel.cpp
    src/ui/EffectorControlPanel.cpp
    src/ui/AlertQueue.cpp
    src/ui/EngagementDialog.cpp
)

set(CONFIG_SOURCES
    src/config/ConfigManager.cpp
    src/config/CameraConfig.cpp
    src/config/DatabaseManager.cpp
)

set(UTILS_SOURCES
    src/utils/CoordinateUtils.cpp
    src/utils/TimeUtils.cpp
    src/utils/FrameBuffer.cpp
    src/utils/Logger.cpp
    src/utils/KalmanFilter.cpp
)

set(SIMULATOR_SOURCES
    src/simulators/SensorSimulator.cpp
    src/simulators/TrackSimulator.cpp
    src/simulators/VideoSimulator.cpp
)

# Header files
set(CORE_HEADERS
    src/core/Track.h
    src/core/TrackManager.h
    src/core/ThreatAssessor.h
    src/core/EngagementManager.h
)

set(SENSOR_HEADERS
    src/sensors/SensorInterface.h
    src/sensors/RadarSensor.h
    src/sensors/RFDetector.h
    src/sensors/CameraSystem.h
)

set(VIDEO_HEADERS
    src/video/VideoStreamManager.h
    src/video/VideoSource.h
    src/video/RTSPVideoSource.h
    src/video/GigEVideoSource.h
    src/video/FileVideoSource.h
    src/video/SimulationVideoSource.h
    src/video/VideoDecoder.h
    src/video/VideoOverlayRenderer.h
    src/video/VideoRecorder.h
    src/video/PTZController.h
    src/video/CameraSlewController.h
)

set(EFFECTOR_HEADERS
    src/effectors/EffectorInterface.h
    src/effectors/RFJammer.h
    src/effectors/KineticInterceptor.h
    src/effectors/DirectedEnergySystem.h
)

set(NETWORK_HEADERS
    src/network/NetworkManager.h
    src/network/MessageProtocol.h
)

set(UI_HEADERS
    src/ui/MainWindow.h
    src/ui/MapWidget.h
    src/ui/VideoDisplayWidget.h
    src/ui/VideoGridWidget.h
    src/ui/VideoControlBar.h
    src/ui/PTZControlWidget.h
    src/ui/TrackListWidget.h
    src/ui/TrackDetailPanel.h
    src/ui/SensorStatusPanel.h
    src/ui/CameraStatusPanel.h
    src/ui/EffectorControlPanel.h
    src/ui/AlertQueue.h
    src/ui/EngagementDialog.h
)

set(CONFIG_HEADERS
    src/config/ConfigManager.h
    src/config/CameraConfig.h
    src/config/DatabaseManager.h
)

set(UTILS_HEADERS
    src/utils/CoordinateUtils.h
    src/utils/TimeUtils.h
    src/utils/FrameBuffer.h
    src/utils/Logger.h
    src/utils/KalmanFilter.h
)

set(SIMULATOR_HEADERS
    src/simulators/SensorSimulator.h
    src/simulators/TrackSimulator.h
    src/simulators/VideoSimulator.h
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${CORE_SOURCES}
    ${SENSOR_SOURCES}
    ${VIDEO_SOURCES}
    ${EFFECTOR_SOURCES}
    ${NETWORK_SOURCES}
    ${UI_SOURCES}
    ${CONFIG_SOURCES}
    ${UTILS_SOURCES}
    ${SIMULATOR_SOURCES}
    ${CORE_HEADERS}
    ${SENSOR_HEADERS}
    ${VIDEO_HEADERS}
    ${EFFECTOR_HEADERS}
    ${NETWORK_HEADERS}
    ${UI_HEADERS}
    ${CONFIG_HEADERS}
    ${UTILS_HEADERS}
    ${SIMULATOR_HEADERS}
    ${RESOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Link Qt libraries
if(QT_VERSION_MAJOR EQUAL 6)
    set(QT6_LINK_LIBS
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::OpenGLWidgets
        Qt6::Concurrent
        Qt6::SerialPort
    )
    
    # Add StateMachine if available
    if(Qt6StateMachine_FOUND)
        list(APPEND QT6_LINK_LIBS Qt6::StateMachine)
    endif()
    
    target_link_libraries(${PROJECT_NAME} PRIVATE ${QT6_LINK_LIBS})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Sql
        Qt5::Multimedia
        Qt5::MultimediaWidgets
        Qt5::OpenGL
        Qt5::Concurrent
        Qt5::SerialPort
    )
endif()

# Optional Qt Location linking
if(QT_VERSION_MAJOR EQUAL 6 AND Qt6Location_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Positioning
        Qt6::Location
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_QT_LOCATION)
elseif(QT_VERSION_MAJOR EQUAL 5 AND Qt5Location_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Positioning
        Qt5::Location
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_QT_LOCATION)
endif()

# Platform-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Copy config files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/config/ DESTINATION ${CMAKE_BINARY_DIR}/config)

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY config/ DESTINATION config)
install(DIRECTORY resources/ DESTINATION resources)

# Unit tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    if(QT_VERSION_MAJOR EQUAL 6)
        find_package(Qt6 COMPONENTS Test REQUIRED)
    else()
        find_package(Qt5 COMPONENTS Test REQUIRED)
    endif()
    
    # Helper function to apply platform-specific compile options to test targets
    function(apply_test_compile_options target_name)
        if(MSVC)
            target_compile_options(${target_name} PRIVATE /W3)
        else()
            target_compile_options(${target_name} PRIVATE -Wall -Wextra)
        endif()
    endfunction()
    
    add_executable(test_track_manager
        tests/test_track_manager.cpp
        ${CORE_SOURCES}
        ${UTILS_SOURCES}
        ${EFFECTOR_SOURCES}
    )
    target_include_directories(test_track_manager PRIVATE ${CMAKE_SOURCE_DIR}/src)
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(test_track_manager PRIVATE Qt6::Core Qt6::Gui Qt6::StateMachine Qt6::Network Qt6::SerialPort Qt6::Test)
    else()
        target_link_libraries(test_track_manager PRIVATE Qt5::Core Qt5::Gui Qt5::Network Qt5::SerialPort Qt5::Test)
    endif()
    apply_test_compile_options(test_track_manager)
    add_test(NAME TrackManagerTest COMMAND test_track_manager)
    
    add_executable(test_threat_assessor
        tests/test_threat_assessor.cpp
        ${CORE_SOURCES}
        ${UTILS_SOURCES}
        ${EFFECTOR_SOURCES}
    )
    target_include_directories(test_threat_assessor PRIVATE ${CMAKE_SOURCE_DIR}/src)
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(test_threat_assessor PRIVATE Qt6::Core Qt6::Gui Qt6::StateMachine Qt6::Network Qt6::SerialPort Qt6::Test)
    else()
        target_link_libraries(test_threat_assessor PRIVATE Qt5::Core Qt5::Gui Qt5::Network Qt5::SerialPort Qt5::Test)
    endif()
    apply_test_compile_options(test_threat_assessor)
    add_test(NAME ThreatAssessorTest COMMAND test_threat_assessor)
    
    add_executable(test_video_pipeline
        tests/test_video_pipeline.cpp
        ${VIDEO_SOURCES}
        ${UTILS_SOURCES}
        ${CORE_SOURCES}
        ${EFFECTOR_SOURCES}
    )
    target_include_directories(test_video_pipeline PRIVATE ${CMAKE_SOURCE_DIR}/src)
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(test_video_pipeline PRIVATE Qt6::Core Qt6::Gui Qt6::Multimedia Qt6::Network Qt6::SerialPort Qt6::StateMachine Qt6::Test)
    else()
        target_link_libraries(test_video_pipeline PRIVATE Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::Network Qt5::SerialPort Qt5::Test)
    endif()
    apply_test_compile_options(test_video_pipeline)
    add_test(NAME VideoPipelineTest COMMAND test_video_pipeline)
endif()

# Debug/Release configurations - platform-specific compiler flags
if(MSVC)
    # Microsoft Visual C++ compiler flags
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2>
        /W3
    )
else()
    # GCC/Clang compiler flags
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3>
        -Wall
        -Wextra
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    message(STATUS "Qt Version: ${Qt6_VERSION}")
    message(STATUS "Qt Location Available: ${Qt6Location_FOUND}")
else()
    message(STATUS "Qt Version: ${Qt5_VERSION}")
    message(STATUS "Qt Location Available: ${Qt5Location_FOUND}")
endif()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
