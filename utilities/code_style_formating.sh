#!/bin/bash

# Check if the project root directory is provided as an argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <project_directory>"
    exit 1
fi

project_directory="$1"

# Find all .hpp and .cpp files in the project directory and its subdirectories
files=$(find "$project_directory" -name "*.hpp" -o -name "*.cpp")

# Run clang-format-14 on each file
for file in $files; do
    clang-format -i -style=file "$file"
    echo "Formatted: $file"
done
