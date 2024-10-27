import os
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parents[1]
ASSETS_DIR = PROJECT_ROOT / "pygui" / "assets"
LIB_DIR = PROJECT_ROOT / "build" / "Release" / "lib"

assert LIB_DIR.is_dir(), f"Error: {LIB_DIR} does not exist. Build the backend first by running the correct build script in {PROJECT_ROOT / 'scripts'} depending on your os."
sys.path.append(str(LIB_DIR))

if os.name == "nt":
    # windows only. source: https://stackoverflow.com/questions/78124769/pybind11-how-do-i-truly-fix-missing-dll-error
    # TODO find out what is needed for linux & find out how to do this without hardcoding a path
    MINGW_BIN = Path(r"c:\programdata\mingw64\mingw64\bin").resolve()
    assert MINGW_BIN.is_dir(), f"error: {MINGW_BIN} is not a valid directory."

    for dir in [LIB_DIR, MINGW_BIN]:
        os.add_dll_directory(str(dir))

# import this here to fail fast
import wrappers  # noQa
