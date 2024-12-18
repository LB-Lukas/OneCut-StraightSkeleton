#----------------------------------------------#
#----- Configuration of the CMake Build System #
#----------------------------------------------#

cmake_minimum_required(VERSION 3.15)
project(OneStraightCut)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
#------------ External Libraries --------------#
#----------------------------------------------#

# CGAL library setup
find_package(CGAL QUIET COMPONENTS Core)
if(NOT CGAL_FOUND)
    message(FATAL_ERROR "This project requires the CGAL library. Please install it before proceeding.")
endif()
include_directories(${CGAL_INCLUDE_DIRS})

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Relevant only for Windows
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

#----------------------------------------------#
#--------------- Pybind11 Setup ---------------#
#----------------------------------------------#

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

#----------------------------------------------#
#-------------- Build Targets -----------------#
#----------------------------------------------#

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Pybind11 module
pybind11_add_module(geometry bindings/straightSkeletonPyBind.cpp ${SRC_FILES})
target_link_libraries(geometry PRIVATE ${CGAL_LIBRARIES} pybind11::module ${GMP_LIBRARIES} ${MPFR_LIBRARIES})
target_include_directories(geometry PRIVATE ${CGAL_INCLUDE_DIRS})

# Test executables
add_executable(simple_test tests/simple_test.cpp)
target_link_libraries(simple_test PRIVATE GTest::gtest_main)
gtest_discover_tests(simple_test)

add_executable(folding_test tests/folding_test.cpp ${SRC_FILES})
target_link_libraries(folding_test PRIVATE GTest::gtest_main ${CGAL_LIBRARIES} ${GMP_LIBRARIES} ${MPFR_LIBRARIES})
target_include_directories(folding_test PRIVATE ${CGAL_INCLUDE_DIRS})

#----------------------------------------------#
#------------ Installation Paths --------------#
#----------------------------------------------#

install(TARGETS geometry
        COMPONENT python
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
)
