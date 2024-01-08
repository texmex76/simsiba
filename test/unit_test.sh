#!/bin/bash

# Define path to the test executable
TEST_EXECUTABLE=./unit_test

# Initialize pass and fail counts
pass_count=0
fail_count=0

# Function to perform test and update pass_count and fail_count
perform_test() {
    output=$1
    expected=$2

    if [ "$output" == "$expected" ]; then
        ((pass_count++))
    else
        ((fail_count++))
        echo "Test $((pass_count + fail_count)) Failed: Expected '$expected', got '$output'"
    fi
}

# Test case 1: No arguments
perform_test "$($TEST_EXECUTABLE)" "help_flag=0;model_path=NULL;some_int=0;some_uint32=0"

# Test case 2: Model path set
perform_test "$($TEST_EXECUTABLE -m model.txt)" "help_flag=0;model_path=model.txt;some_int=0;some_uint32=0"

# Test case 3: Set integer value
perform_test "$($TEST_EXECUTABLE --int 42)" "help_flag=0;model_path=NULL;some_int=42;some_uint32=0"

# Test case 4: Set uint32 value
perform_test "$($TEST_EXECUTABLE --uint32 12345)" "help_flag=0;model_path=NULL;some_int=0;some_uint32=12345"

# Test case 5: Set all values
perform_test "$($TEST_EXECUTABLE -m model.txt --int 42 --uint32 12345)" "help_flag=0;model_path=model.txt;some_int=42;some_uint32=12345"

# Test case 6: Negative integer value
perform_test "$($TEST_EXECUTABLE --int -42)" "help_flag=0;model_path=NULL;some_int=-42;some_uint32=0"

# Test case 7: Extremely large uint32 value
perform_test "$($TEST_EXECUTABLE --uint32 4294967295)" "help_flag=0;model_path=NULL;some_int=0;some_uint32=4294967295"

# Test case 8: Invalid integer (non-numeric)
perform_test "$($TEST_EXECUTABLE --int abc 2>&1)" "Option '--int' requires an integer argument."

# Test case 9: Invalid uint32 (non-numeric)
perform_test "$($TEST_EXECUTABLE --uint32 abc 2>&1)" "Option '--uint32' requires an unsigned long argument."

# Test case 10: Help flag
perform_test "$($TEST_EXECUTABLE --help)" "Usage: [options]
  --help: Display this help message and exit.
  -m: Set the model path.
  --int: Set an integer value.
  --uint32: Set a uint32 value."

# Test case 11: Unknown option
perform_test "$($TEST_EXECUTABLE --unknown 2>&1)" "Unknown option '--unknown'. Use '--help' for usage."

# Test case 12: Integer overflow
perform_test "$($TEST_EXECUTABLE --int 2147483648 2>&1)" "Option '--int' requires an integer argument."

# Test case 13: uint32 overflow
perform_test "$($TEST_EXECUTABLE --uint32 4294967296 2>&1)" "Option '--uint32' requires an unsigned long argument."

# Test case 14: Empty string for model path
perform_test "$($TEST_EXECUTABLE -m "")" "help_flag=0;model_path=;some_int=0;some_uint32=0"

# Test case 15: Special characters in model path
perform_test "$($TEST_EXECUTABLE -m "@!#")" "help_flag=0;model_path=@!#;some_int=0;some_uint32=0"

# Test case 16: Mixed valid and invalid options
perform_test "$($TEST_EXECUTABLE -m model.txt --unknown 2>&1)" "Unknown option '--unknown'. Use '--help' for usage."

# Test case 17: Multiple flags
perform_test "$($TEST_EXECUTABLE -m model.txt --int 42 --int 100)" "help_flag=0;model_path=model.txt;some_int=100;some_uint32=0"

# Test case 18: Missing argument for string
perform_test "$($TEST_EXECUTABLE -m 2>&1)" "Option '-m' requires a string argument."

# Test case 19: Missing argument for int
perform_test "$($TEST_EXECUTABLE --int 2>&1)" "Option '--int' requires an integer argument."

# Test case 20: Missing argument for uint32
perform_test "$($TEST_EXECUTABLE --uint32 2>&1)" "Option '--uint32' requires an unsigned long argument."

# Final check and output
if [ $fail_count -gt 0 ]; then
    echo "$pass_count out of $((pass_count + fail_count)) tests passed."
    exit 1
else
    echo "$pass_count out of $((pass_count + fail_count)) tests passed."
    exit 0
fi
