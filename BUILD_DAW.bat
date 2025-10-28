@echo off
title Tap-DAW Complete Build
color 0A

echo.
echo  ╔════════════════════════════════════════╗
echo  ║   TAP-DAW COMPLETE BUILD SYSTEM       ║
echo  ║   Automated by Hyper-AHS              ║
echo  ╚════════════════════════════════════════╝
echo.
echo  This will build all 9 phases of your DAW
echo  Estimated time: 60 minutes
echo.

timeout /t 3 /nobreak >nul

cd /d "c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs"

REM Install dependencies silently
echo [░░░░░░░░░░] Installing dependencies...
py -m pip install -q networkx anthropic pydantic fastapi uvicorn requests aiofiles >nul 2>&1

echo [████░░░░░░] Running Hyper-AHS...
py run_all_daw_phases.py

echo [████████░░] Building with CMake...
call BUILD_ALL_PHASES.bat

echo [██████████] COMPLETE!
echo.
echo  ✓ Your DAW is ready!
echo.
echo  Launch: phases\daw_phase_1_session_core\build\Release\TapDAW_Phase1.exe
echo.
pause
