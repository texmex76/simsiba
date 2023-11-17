cmake_minimum_required(VERSION 3.0)
project(simsiba)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the parse library
add_library(simsiba STATIC simsiba.c)

# Specify where the library and its headers are
target_include_directories(simsiba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (DEBUG)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O0 -g -ggdb3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -ggdb3")
endif()
