#!/bin/bash

root_dir="$PWD"
source_dir="$root_dir/core"
build_dir="$root_dir/build/Release"
pybind11_src="$root_dir/extern/pybind11"
pybind11_build="$root_dir/extern/pybind11/build"

git submodule update --init --recursive
if [ ! -d "$pybind11_build" ]; then
  echo "Installing pybind11..."
  mkdir -p "$pybind11_build" || exit 1
  cmake -S "$pybind11_src" -B "$pybind11_build" -DCMAKE_CXX_COMPILER="g++"
  cmake --build "$pybind11_build"
  cmake --install "$pybind11_build"
fi

mkdir -p "$build_dir" || exit 1
cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc"
cmake --build "$build_dir"
cmake --install "$build_dir"

if [ ! -d "$root_dir/.venv" ]; then
  python3 -m venv "$root_dir/.venv"
fi

source "$root_dir/.venv/bin/activate" && pip install -e "$root_dir" --upgrade
