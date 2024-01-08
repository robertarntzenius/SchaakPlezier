#!/bin/bash --
# You can now call this script without or with arguments 
# ./BuildAndRun.sh BUILD_TYPE
# to compile in Release (default) or Debug

build_type=${1:-Release}

if [ "$build_type" != "Release" ] && [ "$build_type" != "Debug" ]; then
    echo "Invalid build type, use Debug or Release. (Build type: $build_type)"
    exit 1
fi

source_dir="$PWD"
build_dir="$source_dir/build/$build_type"

if [ -d "$build_dir" ]; then
    echo "Build directory already exists. Deleting..."
    rm -r "$build_dir"
fi

mkdir -p "$build_dir"
cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="$build_type"
cd "$build_dir" || exit
make && ./SchaakPlezier
if [ "$build_type" == "Debug" ]; then
    ctest --rerun-failed --output-on-failure
fi
cd "$source_dir" || exit
