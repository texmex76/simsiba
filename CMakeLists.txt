cmake_minimum_required(VERSION 3.0)
project(simsiba)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# For programming
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the parse library
add_library(simsiba STATIC simsiba.c)

# Specify where the library and its headers are
target_include_directories(simsiba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add the test executable
add_executable(unit_test test/unit_test.c)
target_link_libraries(unit_test PRIVATE simsiba)

add_executable(fuzz test/fuzz.c)
target_link_libraries(fuzz PRIVATE simsiba)

# Enable testing with CTest
enable_testing()

# Add a test
add_test(NAME UnitTest COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/test/unit_test.sh)
add_test(NAME Fuzzing COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/test/fuzz.sh)


if (DEBUG)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O0 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif()
