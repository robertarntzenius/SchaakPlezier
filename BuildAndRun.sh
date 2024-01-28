#!/bin/bash

build_type="Release"
verbose=false

# Parse all args here using getopts
while getopts ":hvb:" opt; do
  case $opt in
    i) # Show usage
      echo "Usage: ./BuildAndRun.sh [OPTIONS]"
      echo ""
      echo "Options:"
      echo "  -b <BUILDTYPE>   Set the build type to either 'Debug' or 'Release'. Default is 'Release'."
      echo "  -v               Display the log file content after running."
      echo "  -h               Display this message."
      exit
      ;;
    v) # cat the logfile if you add -v to this script call
      verbose=true
      ;;
    b) # set the build type using -b Debug or -b Release
      build_type=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG"
      exit 1
      ;;
  esac
done

shift $((OPTIND-1))

# Check for any remaining arguments (unsupported options)
if [ "$#" -gt 0 ]; then
    echo "Unsupported option(s) provided: $@"
    exit
fi


# Set the build type
if [ "$build_type" != "Release" ] && [ "$build_type" != "Debug" ] && [ "$build_type" != "Verbose" ]; then
    echo "Invalid build type, use Debug or Release. (Build type: $build_type)"
    exit 1
fi

source_dir="$PWD"
build_dir="$source_dir/build/$build_type"

# Clean the build directory
if [ -d "$build_dir" ]; then
    echo "Build directory already exists. Deleting..."
    rm -r "$build_dir"
fi

mkdir -p "$build_dir"
cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="$build_type"
cd "$build_dir" || exit

make && ./SchaakPlezier

# Test if Debug
if [ "$build_type" == "Debug" ] || [ "$build_type" != "Verbose" ]; then
    echo ""
    echo ""
    echo ""
    echo "===== Tests ====="
    ctest --rerun-failed --output-on-failure
    echo ""
fi

# Output log to terminal if verbose
if $verbose; then
    echo ""
    echo ""
    echo ""
    echo "===== Schaakplezier.log ====="
    echo ""
    cat "$build_dir/Schaakplezier.log"
fi

cd "$source_dir" || exit
