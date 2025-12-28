#!/bin/bash

# Counter-UAS C2 Build Script

set -e

BUILD_TYPE="${1:-Release}"
BUILD_DIR="build"

echo "========================================"
echo "Counter-UAS C2 System Build"
echo "Build Type: $BUILD_TYPE"
echo "========================================"

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo "Configuring with CMake..."
# Use g++ explicitly if available (avoids clang/libstdc++ issues on some systems)
if command -v g++ &> /dev/null; then
    cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_CXX_COMPILER=/usr/bin/g++
else
    cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# Build
echo "Building..."
cmake --build . --parallel $(nproc)

# Run tests if available
if [ -f "test_track_manager" ]; then
    echo "Running tests..."
    ctest --output-on-failure
fi

echo "========================================"
echo "Build complete!"
echo "Executable: $BUILD_DIR/CounterUAS_C2"
echo "========================================"
