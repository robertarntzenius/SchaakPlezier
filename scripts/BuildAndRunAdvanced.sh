#!/bin/bash

######### Functions #########
function validate_build_type {
  if [ "$build_type" != "Release" ] && [ "$build_type" != "Debug" ] && [ "$build_type" != "Verbose" ]; then
      echo "Invalid build type, use Debug or Release. (Build type: $build_type)"
      exit 1
  fi
}

function clean_build_dir {
  if [ -d "$build_dir" ] && [ $rebuild == true ]; then
      echo "Rebuild flag is set. Deleting build directory: $build_dir"
      rm -r "$build_dir"
  fi
}

function set_profiler_opt {
  profiler_opt="OFF"
  if [ "$profiler" == true ]; then
      rebuild=true
      profiler_opt="ON"
      echo "Profiler flag is set. Compiling with symbols"
  fi
}

function run_cmake_make {
  set_profiler_opt

  mkdir -p "$build_dir" || exit
  if [ $clang == true ]; then
    cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="$build_type" -DPROFILER="$profiler_opt" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang"
  else
    cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="$build_type" -DPROFILER="$profiler_opt" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc"
  fi

  cd "$build_dir" || exit

  make || exit
}

function run_ctest {
  if [ "$build_type" == "Debug" ] || [ "$build_type" == "Verbose" ]; then
    if [ $test == true ]; then
      echo ""
      echo ""
      echo ""
      echo "===== Tests ====="
      ctest --rerun-failed --output-on-failure
      echo ""
    fi
  fi
}

function run_profiler {
  if [ $profiler == true ]; then
    echo ""
    echo ""
    echo ""
    echo "===== Profiler ====="
    echo ""

    local executable="$1"
    if [ ! -x "$executable" ]; then
        echo "Executable not found or not executable: $executable"
        exit 1
    fi
    local valgrind_output="callgrind.out"

    valgrind --tool=callgrind --callgrind-out-file="$valgrind_output" "$executable"
    kcachegrind "$valgrind_output"
  fi
}

function run_SchaakPlezier {
  if $execute; then
    ./SchaakPlezier -w "$1" -b "$2" || exit
  fi
}

function cat_logfile {
  if $verbose; then
      echo ""
      echo ""
      echo ""
      echo "===== Schaakplezier.log ====="
      echo ""
      cat "$build_dir/Schaakplezier.log"
  fi
}

function parse_args {
  while getopts "hb:rvcteugp:o:" opt; do
    case $opt in
      h) # Show usage
        show_usage
        exit
        ;;
      r) # rebuild
        rebuild=true
        ;;
      v) # cat the logfile
        verbose=true
        ;;
      e) # run ./Schaakplezier after building
        execute=true
        ;;
      g) # compile with -g, run valgrind profiler & analyze
        profiler=true
        ;;
      b) # set the build type using -b Debug or -b Release
        build_type="${OPTARG}"
        ;;
      p) # Player: set white player using -p human or -p random
        whitePlayer="${OPTARG}"
        ;;
      o) # Opponent: set black player using -o human or -o random
        blackPlayer="${OPTARG}"
        ;;
      c)
        clang=true
        ;;
      t)
        test=true
        ;;
      \?)
        echo "Invalid option: $opt"
        exit 1
        ;;
      :)
        echo "$opt requires an argument."
        exit 1
        ;;
    esac
  done
  shift $((OPTIND-1))

  # Remaining arguments
  if [ "$#" -gt 0 ]; then
      echo "Unsupported option(s) provided: " "$@"
      exit
  fi
}

function run_gui {
  if [ ! -d "$root_dir/.venv" ]; then
    python3 -m venv "$root_dir/.venv"
    # windows: venv\Scripts\activate
    source "$root_dir/.venv/bin/activate"
  fi

  pip install -e "$root_dir" && python3 "$source_dir/main.py"
}

function show_usage {
  echo "Usage: ./BuildAndRun.sh [OPTIONS]"
  echo ""
  echo "Options:"
  echo "  -b <BUILDTYPE>   Set the build type to either 'Debug' or 'Release'. Default is 'Release'."
  echo "  -p <PLAYERTYPE>  Set the white player to either 'human' or 'random'. Default is 'human'."
  echo "  -o <PLAYERTYPE>  Set the black player to either 'human' or 'random'. Default is 'random'."
  echo "  -r               Rebuild from scratch."
  echo "  -e               Execute SchaakPlezier after building."
  echo "  -g               Compile with debugging symbols to enable profiler."
  echo "  -v               Display the log file content after running."
  echo "  -c               Compile with clang"
  echo "  -t               Run tests"
  echo "  -h               Display this message."
}

function main {
  # Set default variables
  build_type="Release"
  verbose=false
  rebuild=false
  execute=false
  profiler=false
  clang=false
  test=false
  whitePlayer="human"
  blackPlayer="random"

  # Set paths
  root_dir="$PWD"
  source_dir="$root_dir/core"
  build_dir="$root_dir/build/$build_type"

  # User input
  parse_args "$@"

  # Set build & source dirs
  validate_build_type

  # Compile
  clean_build_dir
  run_cmake_make

  # Test & Analyze
  run_ctest

  # run_profiler "$build_dir/board/test/test_movegeneration"
  run_profiler "$build_dir/SchaakPlezier"

  # Run
  run_SchaakPlezier "$whitePlayer" "$blackPlayer"

  # Output
  cat_logfile

  # Run
  run_gui

  # Reset working directory
  cd "$root_dir" || exit
}

######### Main #########
main "$@"
