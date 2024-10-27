#!/bin/bash

root_dir="$PWD"
source_dir="$root_dir/core"
build_dir="$root_dir/build/Release"
pybind11_src="$root_dir/extern/pybind11"
pybind11_build="$root_dir/extern/pybind11/build"
pygui_dir="$root_dir/pygui"
install_dir="$HOME/.local"

git submodule update --init --recursive
if [ ! -d "$pybind11_build" ]; then
  echo "Installing pybind11..."
  mkdir -p "$pybind11_build" || exit 1
  cmake -S "$pybind11_src" -B "$pybind11_build" -DCMAKE_CXX_COMPILER="g++" || exit 1
  cmake --build "$pybind11_build" || exit 1
  cmake --install "$pybind11_build" || exit 1
fi

mkdir -p "$build_dir" || exit 1
cmake -S "$source_dir" -B "$build_dir" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -DCMAKE_INSTALL_PREFIX="$install_dir" || exit 1
cmake --build "$build_dir" || exit 1
cmake --install "$build_dir" || exit 1

if [ ! -d "$root_dir/.venv" ]; then
  python3 -m venv "$root_dir/.venv" || exit 1
  source "$root_dir/.venv/bin/activate" && pip install -e "$root_dir[dev]" --upgrade || exit 1
fi

source "$root_dir/.venv/bin/activate" || exit 1

cd "$build_dir/lib" && stubgen --package wrappers --output "$install_dir" && cd "$root_dir" || exit 1

pyreverse "$pygui_dir" -o pdf --output-directory "$root_dir/notes" || exit 1

cp "$build_dir/lib/libGame.so" "$root_dir/archive"
