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
call "%root_dir%\.venv\Scripts\activate.bat"

REM ====================
REM Run the GUI
REM ====================
echo Running GUI...
python "%source_dir%\main.py"

cd "%root_dir%" || exit /b 1

endlocal
exit /b 0
