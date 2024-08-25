# SchaakPlezier

## Setting Up the Development Environment

Follow these steps to set up the development environment for the SchaakPlezier project.

### 1. Create a Virtual Environment

First, create a virtual environment using the following command:

```bash
python -m venv .venv
```

### 2. Activate the Virtual Environment
Activate the virtual environment using the appropriate command for your operating system and shell:

| Platform | Shell         | Command                                      |
|----------|---------------|----------------------------------------------|
| POSIX    | `bash/zsh`    | `source .venv/bin/activate`                  |
|          | `fish`        | `source .venv/bin/activate.fish`             |
|          | `csh/tcsh`    | `source .venv/bin/activate.csh`              |
|          | `PowerShell`  | `.venv/bin/Activate.ps1`                     |
| Windows  | `cmd.exe`     | `.venv\Scripts\activate.bat`                 |
|          | `PowerShell`  | `.venv\Scripts\Activate.ps1`                 |

For more information on creating and managing virtual environments, refer to the [Python documentation](https://docs.python.org/3/library/venv.html).


### 3. Initialize submodules
```bash
git submodule update --init --recursive
```


### 3. Build, install and run

Run a script that will retrieve, build and install all dependencies (Python and C++)

### Linux
```bash
./scripts/run_schaakplezier.sh
```

### Windows
```cmd
.\scripts\run_schaakplezier.bat
```

Manual configuration (Linux only)
```bash
./scripts/BuildAndRunAdvanced.sh -h
```
