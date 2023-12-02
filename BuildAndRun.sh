#!/bin/bash 

source_dir="$PWD"
build_dir="$source_dir/build"

# Check if the build directory exists
if [ -d "$build_dir" ]; then
    echo "Build directory already exists. Deleting..."
    rm -r "$build_dir"
fi

mkdir -p "$build_dir"
cmake -S "$source_dir" -B "$build_dir"
cd "$build_dir"
make && ./SchaakPlezier
cd "$source_dir"
