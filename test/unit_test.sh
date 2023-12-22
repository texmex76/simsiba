#!/bin/bash

# Define path to the test executable
TEST_EXECUTABLE=./unit_test

# Test case 1: No arguments
output=$($TEST_EXECUTABLE)
expected="help_flag=0;model_path=NULL;some_int=0;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 1 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 2: Model path set
output=$($TEST_EXECUTABLE -m model.txt)
expected="help_flag=0;model_path=model.txt;some_int=0;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 2 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 3: Set integer value
output=$($TEST_EXECUTABLE --int 42)
expected="help_flag=0;model_path=NULL;some_int=42;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 3 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 4: Set uint32 value
output=$($TEST_EXECUTABLE --uint32 12345)
expected="help_flag=0;model_path=NULL;some_int=0;some_uint32=12345"
if [ "$output" != "$expected" ]; then
    echo "Test 4 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 5: Set all values
output=$($TEST_EXECUTABLE -m model.txt --int 42 --uint32 12345)
expected="help_flag=0;model_path=model.txt;some_int=42;some_uint32=12345"
if [ "$output" != "$expected" ]; then
    echo "Test 5 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 6: Negative integer value
output=$($TEST_EXECUTABLE --int -42)
expected="help_flag=0;model_path=NULL;some_int=-42;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 6 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 7: Extremely large uint32 value
output=$($TEST_EXECUTABLE --uint32 4294967295)
expected="help_flag=0;model_path=NULL;some_int=0;some_uint32=4294967295"
if [ "$output" != "$expected" ]; then
    echo "Test 7 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 8: Invalid integer (non-numeric)
output=$($TEST_EXECUTABLE --int abc 2>&1)
expected="Option '--int' requires an integer argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 8 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 9: Invalid uint32 (non-numeric)
output=$($TEST_EXECUTABLE --uint32 abc 2>&1)
expected="Option '--uint32' requires an unsigned long argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 9 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 10: Help flag
output=$($TEST_EXECUTABLE --help)
expected="Usage: [options]"
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 10 Failed: Expected usage message containing '$expected'"
    exit 1
fi

# Test case 11: Unknown option
output=$($TEST_EXECUTABLE --unknown 2>&1)
expected="Unknown option '--unknown'. Use '--help' for usage."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 11 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 12: Integer overflow
output=$($TEST_EXECUTABLE --int 2147483648 2>&1)
expected="Option '--int' requires an integer argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 12 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 13: uint32 overflow
output=$($TEST_EXECUTABLE --uint32 4294967296 2>&1)
expected="Option '--uint32' requires an unsigned long argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 13 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 14: Empty string for model path
output=$($TEST_EXECUTABLE -m "")
expected="help_flag=0;model_path=;some_int=0;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 14 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 15: Special characters in model path
output=$($TEST_EXECUTABLE -m "@!#")
expected="help_flag=0;model_path=@!#;some_int=0;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 15 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 16: Mixed valid and invalid options
output=$($TEST_EXECUTABLE -m model.txt --unknown 2>&1)
expected="Unknown option '--unknown'. Use '--help' for usage."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 16 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 17: Multiple flags
output=$($TEST_EXECUTABLE -m model.txt --int 42 --int 100)
expected="help_flag=0;model_path=model.txt;some_int=100;some_uint32=0"
if [ "$output" != "$expected" ]; then
    echo "Test 17 Failed: Expected '$expected', got '$output'"
    exit 1
fi

# Test case 18: Missing argument for string
output=$($TEST_EXECUTABLE -m 2>&1)
expected="Option '-m' requires a string argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 18 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 19: Missing argument for int
output=$($TEST_EXECUTABLE --int 2>&1)
expected="Option '--int' requires an integer argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 19 Failed: Expected error message containing '$expected'"
    exit 1
fi

# Test case 20: Missing argument for uint32
output=$($TEST_EXECUTABLE --uint32 2>&1)
expected="Option '--uint32' requires an unsigned long argument."
if [[ "$output" != *"$expected"* ]]; then
    echo "Test 20 Failed: Expected error message containing '$expected'"
    exit 1
fi

echo "All tests passed."
