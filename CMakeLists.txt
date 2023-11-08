cmake_minimum_required(VERSION 3.0)
project(parse)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the parse library
add_library(parse STATIC parse.c)

# Specify where the library and its headers are
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

