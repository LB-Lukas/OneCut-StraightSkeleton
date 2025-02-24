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
    message(STATUS "This project requires the CGAL library, and will not be compiled.")
    return()
endif()
include_directories(${CGAL_INCLUDE_DIRS})

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

# Cairo
find_package(PkgConfig REQUIRED) # Use pkg-config to locate Cairo
pkg_check_modules(CAIRO REQUIRED cairo)

# Include Cairo directories
include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})
add_definitions(${CAIRO_CFLAGS_OTHER})

#----------------------------------------------#
#---------------Verwende Pybind11--------------#
#----------------------------------------------#
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)
find_package(CGAL QUIET COMPONENTS Core)


#----------------------------------------------#
#---------------Erzeuge Projekt----------------#
#----------------------------------------------#

# Selbsgeschriebene Includes
include_directories(include)
# Externe Includes
include_directories(external)

# Quelle für das Projekt
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(common OBJECT ${SRC_FILES})
target_link_libraries(common PRIVATE ${CGAL_LIBRARIES})

# Build PerpendicularDemo 
add_executable(PerpendicularDemo examples/PerpendicularDemo.cpp $<TARGET_OBJECTS:common>)
target_link_libraries(PerpendicularDemo PRIVATE ${CGAL_LIBRARIES})

# Build ClosestPairDemo
add_executable(ClosestPairDemo examples/ClosestPairDemo.cpp $<TARGET_OBJECTS:common>)
target_link_libraries(ClosestPairDemo PRIVATE ${CGAL_LIBRARIES})

#----------------------------------------------#
#---------------Pybind11-Modul-----------------#
#----------------------------------------------#

pybind11_add_module(geometry
    bindings/StraightSkeletonPyBind.cpp
    ${SRC_FILES}
)

# specify name of generated .so file
set_target_properties(geometry PROPERTIES OUTPUT_NAME geometry)

target_link_libraries(geometry PRIVATE ${CGAL_LIBRARIES} pybind11::module)

#----------------------------------------------#
#---------------Tests--------------------------#
#----------------------------------------------#
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    message(STATUS "Building tests")
    enable_testing()

    # Test: SkeletonEventTriangleTest
    add_executable(triangle_test tests/SkeletonEventTriangleTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(triangle_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(triangle_test)

    # Test: SkeletonEventRayTest
    add_executable(ray_test tests/SkeletonEventRayTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(ray_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(ray_test)

    # Test: min_heap_triangle_test
    add_executable(min_heap_triangle_ray_test tests/MinHeapTriangleRayTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(min_heap_triangle_ray_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(min_heap_triangle_ray_test)

    # Test: NaiveClosestPairTest
    add_executable(naive_closest_pair_test tests/NaiveClosestPairTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(naive_closest_pair_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(naive_closest_pair_test)

    # Test: StraightSkeletonTest
    add_executable(straight_skeleton_test tests/StraightSkeletonTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(straight_skeleton_test PRIVATE ${CGAL_LIBRARIES} ${CAIRO_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(straight_skeleton_test)

    # Test: PerpendicularTest
    add_executable(perpendicular_test tests/PerpendicularTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(perpendicular_test PRIVATE ${CGAL_LIBRARIES} ${CAIRO_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(perpendicular_test)

    # Test: BasicStraightSkeletonTest
    add_executable(basic_ss_p_test tests/BasicSkeletonPerpendicularTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(basic_ss_p_test PRIVATE ${CGAL_LIBRARIES} ${CAIRO_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(basic_ss_p_test)
else()
    message(STATUS "Skipping tests")
endif()

#----------------------------------------------#
#----------Project-Installation----------------#
#----------------------------------------------#

install(TARGETS geometry
    COMPONENT python
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/python/gui"
)
