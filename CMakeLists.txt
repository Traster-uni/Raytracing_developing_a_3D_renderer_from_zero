cmake_minimum_required(VERSION 3.22)

project(r3dfrom0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/debug)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(ext)
add_subdirectory(libs)
add_subdirectory(src)