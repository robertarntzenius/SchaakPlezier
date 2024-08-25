#!/bin/bash

function run_cmake_make {
  mkdir -p "$build_dir" || exit
  cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc"
  cd "$build_dir" || exit
  make || exit
}

function create_env {
  if [ ! -d "$root_dir/.venv" ]; then
    python3 -m venv "$root_dir/.venv"
  fi

  # windows: venv\Scripts\activate
  source "$root_dir/.venv/bin/activate" && pip install -e "$root_dir" --upgrade
}


function run_gui {
  python3 "$source_dir/main.py"
}

function main {
  root_dir="$PWD"
  source_dir="$root_dir/core"
  build_dir="$root_dir/build/$build_type"

  run_cmake_make
  create_env
  run_gui

  cd "$root_dir" || exit
}

main
