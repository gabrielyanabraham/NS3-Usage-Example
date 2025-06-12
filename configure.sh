#!/bin/bash

# configure.sh
# Script to configure and build the NS3 submodule for the UDP Raw Socket Simulation project.

set -e

NS3_DIR="third_party/ns3"
NS3_BUILD_DIR="$NS3_DIR/build"

echo "Configuring NS3 project..."

if [ -f ".gitmodules" ] && grep -q "path = third_party/ns3" .gitmodules; then
    echo "NS3 submodule found in .gitmodules. Updating..."
    git submodule update --init --recursive || {
        echo "Error: Failed to update NS3 submodule."
        exit 1
    }
elif [ -d "$NS3_DIR" ]; then
    echo "Warning: $NS3_DIR exists but is not a Git submodule."
    echo "Please ensure it is a valid NS3 repository or remove it and add the submodule with:"
    echo "  git submodule add https://gitlab.com/nsnam/ns-3-dev.git $NS3_DIR"
    echo "  git submodule update --init --recursive"
    exit 1
else
    echo "Error: NS3 submodule is not initialized, and $NS3_DIR does not exist."
    echo "Please add the NS3 submodule with:"
    echo "  git submodule add https://gitlab.com/nsnam/ns-3-dev.git $NS3_DIR"
    echo "  git submodule update --init --recursive"
    exit 1
fi

if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: CMake is not installed. Please install CMake (version 3.10 or higher)."
    exit 1
fi

echo "Running NS3 CMake configuration..."
mkdir -p "$NS3_BUILD_DIR"
cd "$NS3_BUILD_DIR"
cmake .. || {
    echo "Error: NS3 CMake configuration failed."
    exit 1
}
echo "Building NS3..."
cmake --build . || {
    echo "Error: NS3 build failed."
    exit 1
}
cd ../..

echo "NS3 configuration and build completed successfully."
echo "You can now build the project using:"
echo "  mkdir build"
echo "  cd build"
echo "  cmake .."
echo "  make"
echo "Or:"
echo "  cmake -B build -S ."
echo "  cmake --build build"
