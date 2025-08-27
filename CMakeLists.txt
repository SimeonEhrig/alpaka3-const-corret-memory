#
# Copyright 2025 Simeon Ehrig
# SPDX-License-Identifier: ISC
#

cmake_minimum_required(VERSION 3.19)

project(alpaka3_const_corret_memory LANGUAGES CXX)

include(cmake/CPM.cmake)
CPMAddPackage("gh:catchorg/Catch2@3.10.0")
find_package(Catch2 3 REQUIRED)
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)


include(cmake/AddTest.cmake)

add_library(headers INTERFACE)
target_include_directories(headers INTERFACE include)


add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
    main.cpp
)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE headers)

include(CTest)
enable_testing()
add_test_target("" tests)