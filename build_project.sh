#!/bin/bash

# first give permission to terminal to run this script:
# chmod +x build_project.sh

# Navigate to the build directory, create it if it doesn't exist
mkdir -p build
cd build

# Run CMake to configure the project
echo "Configuring project using CMake..."
cmake ..

# Build the project
echo "Building the project..."
make

echo "Install library.."
make install

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed. Please check for errors."
fi
