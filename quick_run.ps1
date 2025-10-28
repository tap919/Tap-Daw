# Quick Hyper-AHS Launcher
# Run this with: powershell -ExecutionPolicy Bypass -File quick_run.ps1

$ErrorActionPreference = "Continue"

Write-Host "Installing Hyper-AHS dependencies..." -ForegroundColor Cyan
cd "c:\Users\tap45\OneDrive\Desktop\stuff\hyper-ahs"

# Install dependencies
if (Test-Path requirements.txt) {
    py -m pip install -q -r requirements.txt 2>&1 | Out-Null
} else {
    py -m pip install -q uvicorn fastapi anthropic 2>&1 | Out-Null
}

Write-Host "Launching Hyper-AHS..." -ForegroundColor Green
Write-Host ""

# Run the autorun script
# The autorun.py appears to be a web server, so let's start it
py ".\hyper_ahs\gui\autorun.py"

# If that doesn't work, try as a module
if ($LASTEXITCODE -ne 0) {
    py -m hyper_ahs.gui.autorun
}
