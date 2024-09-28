#!/bin/bash

root_dir="$PWD"
source_dir="$root_dir/pygui"

source "$root_dir/.venv/bin/activate"
python3 "$source_dir/main.py"
