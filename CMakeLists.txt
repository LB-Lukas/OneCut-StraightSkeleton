cmake_minimum_required(VERSION 3.29)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the build type" FORCE)
endif()


project(OneStraightCut)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CGAL REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(${CGAL_INCLUDE_DIRS})

add_library(geometry MODULE
        bindings/straightSkeletonPyBind.cpp
        src/straightSkeleton.cpp
        src/doublyConnectedEdgeList.cpp
        src/folding.cpp
        include/StraightSkeleton/doublyConnectedEdgeList.h
        include/StraightSkeleton/straightSkeleton.h
        include/StraightSkeleton/folding.h
        include/StraightSkeleton/MinHeapTriangleRay.h
        src/MinHeapTriangleRay.cpp
)

target_link_libraries(geometry
        ${CGAL_LIBRARIES}
        pybind11::module
)

#set(CMAKE_INSTALL_RPATH "$ORIGIN")

set_target_properties(geometry PROPERTIES
        PREFIX ""
        SUFFIX ".so"
)

install(TARGETS geometry
        COMPONENT python
        DESTINATION "${PROJECT_SOURCE_DIR}/extra/gui")
