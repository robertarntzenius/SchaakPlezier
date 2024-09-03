import os
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parents[2]
ASSETS_DIR = PROJECT_ROOT / "core" / "schaak_plezier" / "assets"
BIN_DIR = PROJECT_ROOT / "build" / "bin"

assert BIN_DIR.is_dir(), f"Error: {BIN_DIR} does not exist. Build the backend first by running the build script corresponding to your os, located in {PROJECT_ROOT / 'scripts'}. "

if os.name == "nt":
    # windows only. source: https://stackoverflow.com/questions/78124769/pybind11-how-do-i-truly-fix-missing-dll-error
    # TODO find out what is needed for linux & find out how to do this without hardcoding a path
    MINGW_BIN = Path(r"c:\programdata\mingw64\mingw64\bin").resolve()
    assert MINGW_BIN.is_dir(), f"error: {MINGW_BIN} is not a valid directory."

    sys.path.append(str(BIN_DIR))
    for dir in [BIN_DIR, MINGW_BIN]:
        os.add_dll_directory(str(dir))

# import this here to fail fast
import wrappers  # noQa
