#----------------------------------------------#
#------------- CMake Configuration ------------#
#----------------------------------------------#

cmake_minimum_required(VERSION 3.29)
project(OneStraightCut)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Build)

# Disable CGAL build warnings about the CMake build type
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)
cmake_policy(SET CMP0167 NEW)

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
#----------------- Dependencies ---------------#
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


#----------------------------------------------#
#----------------- Use Pybind11 ---------------#
#----------------------------------------------#
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)
find_package(CGAL QUIET COMPONENTS Core)


#----------------------------------------------#
#----------------Create Project----------------#
#----------------------------------------------#

# own Includes
include_directories(include)
# external Includes
include_directories(external)

# sources for the project
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(common OBJECT ${SRC_FILES})
target_link_libraries(common PRIVATE ${CGAL_LIBRARIES})

# Build PerpendicularDemo 
add_executable(PerpendicularDemo examples/PerpendicularDemo.cpp $<TARGET_OBJECTS:common>)
target_link_libraries(PerpendicularDemo PRIVATE ${CGAL_LIBRARIES})

# Build SkeletonBuilderDemo 
add_executable(SkeletonBuilderDemo examples/SkeletonBuilderDemo.cpp $<TARGET_OBJECTS:common>)
target_link_libraries(SkeletonBuilderDemo PRIVATE ${CGAL_LIBRARIES})

#----------------------------------------------#
#---------------Pybind11-Module----------------#
#----------------------------------------------#

pybind11_add_module(one_cut
    bindings/StraightSkeletonPyBind.cpp
    ${SRC_FILES}
)

# specify name of generated .so file
set_target_properties(one_cut PROPERTIES OUTPUT_NAME one_cut)

target_link_libraries(one_cut PRIVATE ${CGAL_LIBRARIES} pybind11::module)

#----------------------------------------------#
#---------------Tests--------------------------#
#----------------------------------------------#
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    message(STATUS "Building tests")
    enable_testing()

    # Test: GeometryUtilTest
    add_executable(geometry_util_test tests/GeometryUtilTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(geometry_util_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(geometry_util_test)

    # Test: SkeletonBuilderTest
    add_executable(skeleton_builder_test tests/SkeletonBuilderTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(skeleton_builder_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(skeleton_builder_test)

    # Test: PerpendicularFinderTest
    add_executable(perpendicular_finder_test tests/PerpendicularFinderTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(perpendicular_finder_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(perpendicular_finder_test)

    # Test: IntersectionUtilTest
    add_executable(intersection_util_test tests/IntersectionUtilTest.cpp $<TARGET_OBJECTS:common>)
    target_link_libraries(intersection_util_test PRIVATE ${CGAL_LIBRARIES} GTest::gtest_main)
    gtest_discover_tests(intersection_util_test)

else()
    message(STATUS "Skipping tests")
endif()

#----------------------------------------------#
#----------Project-Installation----------------#
#----------------------------------------------#

install(TARGETS one_cut
    COMPONENT python
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/python/gui"
)
