#----------------------------------------------#
#-----Konfiguration des Buildsystems CMake-----#
#----------------------------------------------#

cmake_minimum_required(VERSION 3.29)
project(OneStraightCut)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Disable CGAL build warnings about the CMake build type
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

#----------------------------------------------#
#----- Platform-Specific Configuration --------#
#----------------------------------------------#

# Platform-specific paths for GMP and MPFR
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
    message(STATUS "Configuring for macOS")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows") # Windows
    message(STATUS "Configuring for Windows")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux") # Linux
    message(STATUS "Configuring for Linux")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

#----------------------------------------------#
#----------------- Abhängigkeiten -------------#
#----------------------------------------------#

# CGAL
find_package(CGAL QUIET COMPONENTS Core)
if(NOT CGAL_FOUND)
    message(STATUS "This project requires the CGAL library and will not be compiled.")
    return()
endif()

# Pybind11
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

# GoogleTest
include(FetchContent)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "Enable installation of googletest." OFF)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

#----------------------------------------------#
#---------------Verwende Pybind11--------------#
#----------------------------------------------#
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)
find_package(CGAL QUIET COMPONENTS Core)
if(NOT CGAL_FOUND)
    message(STATUS "This project requires the CGAL library, and will not be compiled.")
    return()
endif()
include_directories(${CGAL_INCLUDE_DIRS})

#----------------------------------------------#
#---------------Erzeuge Projekt----------------#
#----------------------------------------------#

# Selbsgeschriebene Includes
include_directories(include)
# Externe Includes
include_directories(external)

# Quelle für das Projekt
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

#----------------------------------------------#
#---------------Pybind11-Modul-----------------#
#----------------------------------------------#

pybind11_add_module(geometry
    bindings/straightSkeletonPyBind.cpp
    ${SRC_FILES}
)

# specify name of generated .so file
set_target_properties(geometry PROPERTIES OUTPUT_NAME geometry)

target_link_libraries(geometry PRIVATE ${CGAL_LIBRARIES} pybind11::module)

#----------------------------------------------#
#---------------Tests--------------------------#
#----------------------------------------------#

enable_testing()

# Test: folding_test
add_executable(folding_test tests/folding_test.cpp ${SRC_FILES}
        src/polygon.cpp)
target_link_libraries(folding_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
gtest_discover_tests(folding_test)

# Test: SkeletonEventTriangleTest
add_executable(triangle_test tests/SkeletonEventTriangleTest.cpp ${SRC_FILES})
target_link_libraries(triangle_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
gtest_discover_tests(triangle_test)

# Test: SkeletonEventRayTest
add_executable(ray_test tests/SkeletonEventRayTest.cpp ${SRC_FILES})
target_link_libraries(ray_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
gtest_discover_tests(ray_test)

# Test: min_heap_triangle_test
add_executable(min_heap_triangle_ray_test tests/MinHeapTriangleRayTest.cpp ${SRC_FILES})
target_link_libraries(min_heap_triangle_ray_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
gtest_discover_tests(min_heap_triangle_ray_test)

# Test: NaiveClosestPairTest
add_executable(naive_closest_pair_test tests/NaiveClosestPairTest.cpp ${SRC_FILES})
target_link_libraries(naive_closest_pair_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
gtest_discover_tests(naive_closest_pair_test)

#----------------------------------------------#
#----------Project-Installation----------------#
#----------------------------------------------#

install(TARGETS geometry
    COMPONENT python
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/python/gui"
)
