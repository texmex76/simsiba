#!/bin/bash

# Run valgrind and capture its output
output=$(valgrind ./unit_test 2>&1)

# Search for the no-leak message in the output
if echo "$output" | grep -q "All heap blocks were freed -- no leaks are possible"; then
    # No leaks were found
    exit 0
else
    # Leaks were found or another error occurred
    exit 1
fi
