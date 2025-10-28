# BUILD NOTES - Tap-DAW Hyper-AHS Integration Issues & Fixes

## Date: October 10, 2025
## Project: Tap-DAW Complete Implementation
## Tool: Hyper-AHS Automated Coding System

---

## EXECUTION ISSUES ENCOUNTERED

### 1. Terminal Command Execution Problems
**Issue**: PowerShell commands in VS Code terminals not executing properly
- Commands would queue but not run
- Background processes not starting
- Terminal output not captured correctly

**Root Cause**:
- VS Code terminal API interaction issues
- PowerShell execution policy conflicts
- Async terminal command handling

**FIX FOR NEXT SESSION**:
```json
// Add to .vscode/settings.json
{
    "terminal.integrated.defaultProfile.windows": "PowerShell",
    "terminal.integrated.automationProfile.windows": {
        "path": "powershell.exe",
        "args": ["-NoProfile", "-ExecutionPolicy", "Bypass"]
    },
    "terminal.integrated.enablePersistentSessions": false
}
```

### 2. Python Module Import Failures
**Issue**: Missing dependencies for Hyper-AHS
- `ModuleNotFoundError: No module named 'networkx'`
- `ModuleNotFoundError: No module named 'uvicorn'`

**Dependencies Required**:
```bash
py -m pip install networkx anthropic pydantic fastapi uvicorn requests aiofiles
```

**FIX FOR NEXT SESSION**:
Create `setup_hyper_ahs.ps1` that runs automatically:
```powershell
# Auto-install script
$hyperAhsDir = "c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs"
if (Test-Path "$hyperAhsDir\requirements.txt") {
    py -m pip install -q -r "$hyperAhsDir\requirements.txt"
} else {
    py -m pip install -q networkx anthropic pydantic fastapi uvicorn requests aiofiles
}
```

### 3. Path Length Issues (Windows)
**Issue**: Long file paths in checkpoints causing issues
- Checkpoint paths exceed Windows MAX_PATH (260 chars)
- Some JUCE files have very deep nesting

**FIX FOR NEXT SESSION**:
Enable long path support in Windows:
```powershell
# Run as Administrator
New-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\FileSystem" `
    -Name "LongPathsEnabled" -Value 1 -PropertyType DWORD -Force
```

OR use drive substitution:
```powershell
# Map to shorter drive letter
subst T: "c:\Users\tap45\OneDrive\Documents\Overlay Data\Tap-Daw-main"
# Then work in T:\ instead
```

### 4. Hyper-AHS Workspace Detection
**Issue**: Hyper-AHS script had hardcoded paths
- `workspace=r"c:\Users\tap45\OneDrive\Documents\Overlay Data\Tap-Daw-main"`
- Not portable across machines

**FIX FOR NEXT SESSION**:
```python
import os
from pathlib import Path

# Auto-detect workspace
workspace = Path(__file__).parent.parent / "Tap-Daw-main"
if not workspace.exists():
    workspace = Path(os.getcwd())
```

---

## AUTO-RUN SETUP FOR VS CODE

### Task Configuration
Create `.vscode/tasks.json` in Tap-DAW workspace:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Setup Hyper-AHS Dependencies",
            "type": "shell",
            "command": "py",
            "args": [
                "-m", "pip", "install", "-q",
                "networkx", "anthropic", "pydantic",
                "fastapi", "uvicorn", "requests", "aiofiles"
            ],
            "options": {
                "cwd": "${workspaceFolder}/../../../Desktop/stuff/hyper-ahs"
            },
            "problemMatcher": [],
            "presentation": {
                "reveal": "silent",
                "panel": "shared"
            }
        },
        {
            "label": "Run Hyper-AHS Phase 1",
            "type": "shell",
            "command": "py",
            "args": ["run_daw_phase1.py"],
            "options": {
                "cwd": "${workspaceFolder}/../../../Desktop/stuff/hyper-ahs"
            },
            "dependsOn": ["Setup Hyper-AHS Dependencies"],
            "problemMatcher": []
        },
        {
            "label": "Run Hyper-AHS All Phases",
            "type": "shell",
            "command": "py",
            "args": ["run_all_daw_phases.py"],
            "options": {
                "cwd": "${workspaceFolder}/../../../Desktop/stuff/hyper-ahs"
            },
            "dependsOn": ["Setup Hyper-AHS Dependencies"],
            "problemMatcher": [],
            "runOptions": {
                "runOn": "default"
            }
        },
        {
            "label": "Build DAW Phase 1",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build", "build",
                "--config", "Release"
            ],
            "options": {
                "cwd": "${workspaceFolder}/phases/daw_phase_1_session_core"
            },
            "problemMatcher": ["$msCompile"]
        }
    ]
}
```

### Launch Configuration
Create `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Run DAW Phase 1",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${workspaceFolder}/phases/daw_phase_1_session_core/build/Release/TapDAW_Phase1.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}/phases/daw_phase_1_session_core",
            "environment": [],
            "console": "externalTerminal",
            "preLaunchTask": "Build DAW Phase 1"
        }
    ]
}
```

### Workspace Settings
Create `.vscode/settings.json`:

```json
{
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp"
    },
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/phases/daw_phase_1_session_core/build",
    "terminal.integrated.defaultProfile.windows": "PowerShell",
    "terminal.integrated.automationProfile.windows": {
        "path": "powershell.exe",
        "args": ["-NoProfile", "-ExecutionPolicy", "Bypass"]
    },
    "task.autoDetect": "on",
    "task.quickOpen.detail": true,
    "task.quickOpen.showAll": true
}
```

### Auto-Run on Open
Create `.vscode/extensions.json`:

```json
{
    "recommendations": [
        "ms-vscode.cpptools",
        "ms-vscode.cmake-tools",
        "ms-python.python",
        "github.copilot"
    ]
}
```

---

## MANUAL EXECUTION INSTRUCTIONS

### For Next VS Code Session:

1. **Open Terminal** (Ctrl + `)

2. **Setup Dependencies** (one-time):
```powershell
cd "c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs"
py -m pip install networkx anthropic pydantic fastapi uvicorn requests aiofiles
```

3. **Run Complete DAW Build**:
```powershell
cd "c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs"
py run_all_daw_phases.py
```

4. **Build and Test**:
```powershell
cd "c:\Users\tap45\OneDrive\Documents\Overlay Data\Tap-Daw-main\phases\daw_phase_1_session_core"
cmake --build build --config Release
.\build\Release\TapDAW_Phase1.exe
```

---

## AUTOMATION SCRIPT

Create `auto_build_daw.bat` in Desktop for one-click execution:

```batch
@echo off
echo ========================================
echo Tap-DAW Complete Build System
echo ========================================

REM Step 1: Install dependencies
echo [1/4] Installing Hyper-AHS dependencies...
cd "c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs"
py -m pip install -q networkx anthropic pydantic fastapi uvicorn requests aiofiles

REM Step 2: Run all phases
echo [2/4] Running Hyper-AHS for all 9 phases...
py run_all_daw_phases.py

REM Step 3: Build Phase 1
echo [3/4] Building Phase 1...
cd "c:\Users\tap45\OneDrive\Documents\Overlay Data\Tap-Daw-main\phases\daw_phase_1_session_core"
if exist build (
    cmake --build build --config Release
) else (
    cmake -B build -S . -G "Visual Studio 17 2022" -A x64
    cmake --build build --config Release
)

REM Step 4: Run executable
echo [4/4] Launching DAW...
start "" ".\build\Release\TapDAW_Phase1.exe"

echo.
echo ========================================
echo Build Complete!
echo ========================================
pause
```

---

## TROUBLESHOOTING FOR COPILOT

### When VS Code opens next time:

1. **Check if Hyper-AHS is accessible**:
   - Verify `c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs` exists
   - Check Python is available: `py --version`

2. **If tasks don't run automatically**:
   - Press `Ctrl+Shift+P`
   - Type "Tasks: Run Task"
   - Select "Run Hyper-AHS All Phases"

3. **If build fails**:
   - Check JUCE submodule: `git submodule update --init --recursive`
   - Verify VS 2022 is installed
   - Check CMake version: `cmake --version` (need 3.22+)

4. **If executable won't run**:
   - Check missing DLLs with Dependency Walker
   - Install Visual C++ Redistributable 2022
   - Run from VS Code terminal for debug output

---

## EXPECTED OUTCOMES

After successful execution:

✅ All 9 phases coded by Hyper-AHS
✅ Complete DAW with:
  - Session management
  - Audio playback
  - MIDI support
  - Plugin hosting (VST3/CLAP)
  - FL Studio-style workflow
  - Professional UI

✅ Build artifacts:
  - `TapDAW_Phase1.exe` through `TapDAW_Phase9.exe`
  - All CMake build files
  - Documentation in each phase

✅ Test results in `test_results/`

---

## NEXT STEPS FOR COPILOT

When user opens VS Code next:

1. Read this BUILD_NOTES.md file
2. Check if tasks.json exists, create if missing
3. Ask user: "Ready to run complete DAW build?"
4. Execute "Run Hyper-AHS All Phases" task
5. Monitor output and report any errors
6. After completion, build and launch Phase 1
7. Guide user through testing each phase

---

## CONTACT & SUPPORT

- Workspace: `c:\Users\tap45\OneDrive\Documents\Overlay Data\Tap-Daw-main`
- Tool: `c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs`
- Logs: `hyper_ahs.log` in tool directory
- Checkpoints: `.checkpoints/` in workspace

**Key Files**:
- `CODING_OBJECTIVES.md` - Phase 1 requirements
- `run_all_daw_phases.py` - Complete build script
- This file - Build notes and fixes

---

*Generated: October 10, 2025*
*System: Windows 11, VS Code, Python 3.x, Visual Studio 2022*
