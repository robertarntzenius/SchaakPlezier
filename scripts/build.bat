@echo off
setlocal enabledelayedexpansion

REM ====================
REM Set Paths. (script_path -> remove trailing `\`, get parent, remove trailing `\`)
REM ====================

set "script_dir=%~dp0"
set "script_dir=%script_dir:~0,-1%"
for %%I in ("%script_dir%") do set "root_dir=%%~dpI"
set "root_dir=%root_dir:~0,-1%"
set "build_dir=%root_dir%\build\Release"
set "source_dir=%root_dir%\core"
set "pybind11_src=%root_dir%\extern\pybind11"
set "pybind11_build=%pybind11_src%\build"


REM ====================
REM Build and install pybind11 if not already built
REM ====================
git submodule update --init --recursive || exit /b 1
if not exist "%pybind11_build%" (
    mkdir "%pybind11_build%" || exit /b 1
    cmake -S "%pybind11_src%" -B "%pybind11_build%" -DCMAKE_CXX_COMPILER="g++" -G Ninja || exit /b 1
    cmake --build "%pybind11_build%" || exit /b 1
    cmake --install "%pybind11_build%" || exit /b 1
)

REM ====================
REM Run CMake and Ninja
REM ====================
if not exist "%build_dir%" (
    mkdir "%build_dir%" || exit /b 1
)
cmake -S "%source_dir%" -B "%build_dir%" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -G Ninja || exit /b 1
cmake --build "%build_dir%" || exit /b 1
cmake --install "%build_dir%" || exit /b 1

REM ====================
REM Create and activate the virtual environment
REM ====================
if not exist "%root_dir%\.venv" (
    python -m venv "%root_dir%\.venv" || exit /b 1
)
call "%root_dir%\.venv\Scripts\activate.bat" || exit /b 1
call pip install -e "%root_dir%[dev]" --upgrade || exit /b 1

REM ====================
REM Create .pyi type stubs
REM ====================
cd "%build_dir%\bin" || exit /b 1
REM TODO add mingw paths here as well? (see schaak_plezier/__init__.py)
stubgen --package wrappers --output "%build_dir%\bin" || exit /b 1
cd "%root_dir%" || exit /b 1

REM ====================
REM Create UML diagrams
REM ====================
pyreverse "%source_dir%\schaak_plezier" -o pdf --output-directory "%root_dir%\notes" || exit /b 1

endlocal
exit /b 0
