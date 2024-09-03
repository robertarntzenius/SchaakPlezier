#!/bin/bash

root_dir="$PWD"
source_dir="$root_dir/core"
build_dir="$root_dir/build/Release"

mkdir -p "$build_dir" || exit
cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc"
cmake --build "$build_dir"
cmake --install "$build_dir"

if [ ! -d "$root_dir/.venv" ]; then
  python3 -m venv "$root_dir/.venv"
fi

source "$root_dir/.venv/bin/activate" && pip install -e "$root_dir" --upgrade
