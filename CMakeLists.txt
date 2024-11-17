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

# Add executable and specify source files
add_executable(OneStraightCut
        main.cpp
        src/doublyConnectedEdgeList.cpp
        include/StraightSkeleton/doublyConnectedEdgeList.h
        include/StraightSkeleton/straightSkeleton.h
        src/straightSkeleton.cpp
        include/StraightSkeleton/Folding.h
        src/Folding.cpp
        bindings/straightSkeletonPyBind.cpp
)

# Link CGAL and pybind11 to the executable
target_link_libraries(OneStraightCut
        ${CGAL_LIBRARIES}
        pybind11::module
)
