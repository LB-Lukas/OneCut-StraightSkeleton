cmake_minimum_required(VERSION 3.29)
project(OneStraightCut)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 26)

# Find CGAL package
find_package(CGAL REQUIRED)

# Include directories (CGAL headers)
include_directories(${CGAL_INCLUDE_DIRS})

# Add executable and source files
add_executable(OneStraightCut main.cpp
        src/doublyConnectedEdgeList.cpp
        include/StraightSkeleton/doublyConnectedEdgeList.h
        include/StraightSkeleton/straightSkeleton.h
        src/straightSkeleton.cpp
)

# Link CGAL and its dependencies
target_link_libraries(OneStraightCut ${CGAL_LIBRARIES})
