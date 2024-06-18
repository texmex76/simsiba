cmake_minimum_required(VERSION 3.10)
project(simsiba)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(simsiba STATIC simsiba.c)

target_include_directories(simsiba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTING)
    add_executable(unit_test test/unit_test.c)
    target_link_libraries(unit_test PRIVATE simsiba)

    add_executable(fuzz test/fuzz.c)
    target_link_libraries(fuzz PRIVATE simsiba)

    # Enable testing with CTest
    enable_testing()

    add_test(NAME UnitTest COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/test/unit_test.sh)
    add_test(NAME Fuzzing COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/test/fuzz.sh)
    add_test(NAME MemoryLeaks COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/test/memory_leaks.sh)
endif()

if (DEBUG)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O0 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif()
