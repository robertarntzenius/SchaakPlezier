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

REM ====================
REM Create and activate the virtual environment
REM ====================
if not exist "%root_dir%\.venv" (
    echo Creating virtual environment...
    python -m venv "%root_dir%\.venv"
    call "%root_dir%\.venv\Scripts\activate.bat" && pip install -e "%root_dir%"
)
call "%root_dir%\.venv\Scripts\activate.bat"

REM ====================
REM Run CMake and Ninja
REM ====================
if not exist "%build_dir%" (
    echo Creating build directory at "%build_dir%" ...
    mkdir "%build_dir%" || exit /b 1
)
cmake -S "%source_dir%" -B "%build_dir%" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -G Ninja || exit /b 1
cmake --build "%build_dir%" || exit /b 1
cmake --install "%build_dir%" || exit /b 1

cd "%root_dir%" || exit /b 1

endlocal
exit /b 0
