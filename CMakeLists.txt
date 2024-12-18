#----------------------------------------------#
#-----Konfiguration des Buildsystems CMake-----#
#----------------------------------------------#

cmake_minimum_required(VERSION 3.29)
project(OneStraightCut)

#----------------------------------------------#
#-------------------Optionen-------------------#
#----------------------------------------------#

# Setze verwendeten C++-Standard auf C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

# Füge selbsgeschriebene Includes hinzu
include_directories(include)
# Füge externe Includes hinzu
include_directories(external)

#----------------------------------------------#
#----------Download und Nutzung von GoogleTest--#
#----------------------------------------------#

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

# Quelle für das Projekt
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Pybind11 Modul erzeugen
pybind11_add_module(geometry
    bindings/straightSkeletonPyBind.cpp
    ${SRC_FILES}
)

# Verlinke das Pybind11 Modul
target_link_libraries(geometry PRIVATE ${CGAL_LIBRARIES} pybind11::module)

#----------------------------------------------#
#-------------Erzeuge Testprogramme------------#
#----------------------------------------------#

# Testprogramm für simple_test
add_executable(simple_test tests/simple_test.cpp)
target_link_libraries(simple_test GTest::gtest_main)
gtest_discover_tests(simple_test)

# TODO: Test kompiliert nicht
# Testprogramm für folding_test 
# add_executable(folding_test tests/folding_test.cpp)
# target_link_libraries(folding_test GTest::gtest_main)
# gtest_discover_tests(folding_test)

#----------------------------------------------#
#----------Installation des Projekts-----------#
#----------------------------------------------#
install(TARGETS geometry
COMPONENT python
LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/extra/gui")
