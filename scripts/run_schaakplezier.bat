@echo off
setlocal

REM ====================
REM Set Paths. (script_path -> remove trailing `\`, get parent, remove trailing `\`)
REM ====================
set "script_dir=%~dp0"
set "script_dir=%script_dir:~0,-1%"
for %%I in ("%script_dir%") do set "root_dir=%%~dpI"
set "root_dir=%root_dir:~0,-1%"


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
echo Creating build directory if it does not exist...
if not exist "%build_dir%" (
    mkdir "%build_dir%" || exit /b 1
)
echo Running CMake...
cmake -S "%source_dir%" -B "%build_dir%" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -G Ninja || exit /b 1

echo Running Generator...
ninja -C "%build_dir%" || exit /b 1


REM ====================
REM Make created binaries available
REM ====================
set API_DIR="%build_dir%\api"
set PATH=%PATH%;%API_DIR%

REM Attempt to import the module to verify the copy
python -c "import wrappers" 2>stderr.log

REM Check if there were errors
if exist stderr.log (
    type stderr.log
    del stderr.log
    exit /b 1
)

REM ====================
REM Run the GUI
REM ====================
echo Running GUI...
python "%source_dir%\main.py"

cd "%root_dir%" || exit /b 1

endlocal
exit /b 0
