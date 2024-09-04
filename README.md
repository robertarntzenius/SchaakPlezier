# SchaakPlezier

## Setting Up the Development Environment

Follow these steps to set up the development environment for the SchaakPlezier project.

### 1. Install build tools
#### Windows
- Install python (if you dont have it already) form LINK_HERE.
- Install chocolatey by downloading it from LINK_HERE.
- Run the following commands from a terminal with admin rights to install c++ and it's build tools:
```cmd
choco install mingw
choco install cmake
choco install ninja
```
#### Linux

```bash
sudo apt-get install python g++ cmake
```

### 2. Build and install

Run a script that will retrieve, build and install all dependencies (Python and C++)

#### Windows
```cmd
.\scripts\build.bat
```

#### Linux
```bash
./scripts/build.sh
```

Manual configuration (Linux only)
```bash
./scripts/BuildAndRunAdvanced.sh -h
```

### 3. Run
#### Windows
```cmd
.\scripts\run.bat
```

#### Linux
```bash
./scripts/run.sh
```
