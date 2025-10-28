# Launch the Auto-Coder UI on http://127.0.0.1:7998 and worker
$ErrorActionPreference = "Stop"
if (-not (Test-Path ".venv")) { python -m venv .venv }
& .\.venv\Scripts\Activate.ps1
python -m pip install -U pip wheel setuptools
pip install fastapi uvicorn pyyaml gitpython watchdog httpx colorama

# Optional LLM provider:
# pip install openai

$env:PYTHONPATH = (Resolve-Path ".").Path
Start-Process -FilePath ".venv\Scripts\python.exe" -ArgumentList "autocoder\app.py" -WindowStyle Normal
Write-Host "Auto-Coder UI: http://127.0.0.1:7998"
