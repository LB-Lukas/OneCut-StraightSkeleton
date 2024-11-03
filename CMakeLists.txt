cmake_minimum_required(VERSION 3.29)
project(OneStraightCut)

set(CMAKE_CXX_STANDARD 26)

add_executable(OneStraightCut main.cpp
        src/doublyConnectedEdgeList.cpp
        include/StraightSkeleton/doublyConnectedEdgeList.h)
