cmake_minimum_required(VERSION 3.29)
project(OneStraightCut)

# Set the C++ standard (adjust as needed based on compiler support)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CGAL REQUIRED)
find_package(pybind11 REQUIRED)

# Include CGAL headers
include_directories(${CGAL_INCLUDE_DIRS})

# Add the Python module (shared library) target
add_library(geometry MODULE
        bindings/straightSkeletonPyBind.cpp
        src/straightSkeleton.cpp
        src/doublyConnectedEdgeList.cpp
        src/Folding.cpp
        include/StraightSkeleton/doublyConnectedEdgeList.h
        include/StraightSkeleton/straightSkeleton.h
        include/StraightSkeleton/Folding.h
)

# Link required libraries (CGAL and pybind11)
target_link_libraries(geometry
        ${CGAL_LIBRARIES}
        pybind11::module
)

# Specify the output directory for the shared library
set_target_properties(geometry PROPERTIES
        PREFIX ""
        SUFFIX ".so"  # For Linux or macOS, use ".pyd" for Windows
)

