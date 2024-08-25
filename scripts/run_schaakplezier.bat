@echo off
setlocal

:run_cmake_make
if not exist "%build_dir%" (
    mkdir "%build_dir%" || exit /b 1
)
cmake -S "%source_dir%" -B "%build_dir%" -DBUILD_TYPE="Release" -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc"
cd "%build_dir%" || exit /b 1
make || exit /b 1
cd "%root_dir%" || exit /b 1
goto :eof

REM Function to create and activate the virtual environment
:create_env
if not exist "%root_dir%\.venv" (
    python -m venv "%root_dir%\.venv"
)

REM Activate virtual environment and install dependencies
call "%root_dir%\.venv\Scripts\activate.bat" && pip install -e "%root_dir%" --upgrade
goto :eof

REM Function to run the GUI
:run_gui
python "%source_dir%\main.py"
goto :eof

REM Main function
:main
set "root_dir=%CD%"
set "source_dir=%root_dir%\core"
set "build_dir=%root_dir%\build\%build_type%"

call :run_cmake_make
call :create_env
call :run_gui

cd "%root_dir%" || exit /b 1
endlocal
exit /b 0

REM Start the main function
call :main
