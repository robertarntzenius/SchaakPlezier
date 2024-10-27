#!/bin/bash

root_dir="$PWD"
source_dir="$root_dir/pygui"
export LD_LIBRARY_PATH="$HOME/.local/lib:$LD_LIBRARY_PATH"

source "$root_dir/.venv/bin/activate"
python3 "$source_dir/main.py"
