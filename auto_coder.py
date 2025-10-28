#!/usr/bin/env python3
"""
Auto-Coder: Simple, self-contained coding assistant.
No external APIs, no Copilot. Runs locally with built-in planning.

Usage:
1. Edit objectives.txt with your goals.
2. Run: python auto_coder.py
3. It will plan, apply patches, and commit.

For LLM integration, replace the plan() function with your LLM call.
"""

import os
import sys
import subprocess
import yaml
import json
from pathlib import Path
from typing import Dict, List

# Config (edit these)
PROJECT_NAME = "overlay_cheetah"
TEST_COMMAND = "pytest -q"  # or "ruff check ."
ENTRY_POINTS = ["src/overlay_cheetah/", "Overlay_Cheetah/"]
MAX_FILES = 12

# Built-in plan templates (replace with LLM)
def plan(objectives: str) -> str:
    return f"PLAN:\n- Fix imports and paths\n- Update GUI server to start fast\n- Build EXE with PyInstaller\n- Run tests\nObjectives: {objectives}"

def apply_patch(patch_text: str, allowlist: List[str]) -> bool:
    # Simple patch application (use git apply in real impl)
    # For now, just print the patch
    print("Applying patch:")
    print(patch_text)
    return True

def run_tests() -> int:
    try:
        result = subprocess.run(TEST_COMMAND, shell=True, capture_output=True, text=True)
        print("Test output:")
        print(result.stdout)
        if result.stderr:
            print("Errors:")
            print(result.stderr)
        return result.returncode
    except Exception as e:
        print(f"Test run failed: {e}")
        return 1

def main():
    # Load objectives
    objectives_file = Path("objectives.txt")
    if not objectives_file.exists():
        print("Create objectives.txt with your goals.")
        return

    objectives = objectives_file.read_text()

    # Plan
    plan_text = plan(objectives)
    print("Plan:")
    print(plan_text)

    # Simulate checkpoints (in real, pause for user)
    print("50% checkpoint: Press Enter to continue")
    input()

    # Apply half plan
    half_patch = "Patch for first 50%: Fix imports"  # Replace with real patch generation
    if not apply_patch(half_patch, []):  # allowlist empty for simplicity
        print("Patch failed")
        return

    # Run tests
    if run_tests() != 0:
        print("Tests failed")
        return

    print("98% checkpoint: Press Enter to continue")
    input()

    # Apply final patch
    final_patch = "Final patch: Build EXE"  # Replace
    if not apply_patch(final_patch, []):
        print("Final patch failed")
        return

    # Commit
    subprocess.run(["git", "add", "."], check=True)
    subprocess.run(["git", "commit", "-m", f"Auto: {objectives[:50]}"], check=True)
    print("Committed changes")

if __name__ == "__main__":
    main()</content>
<parameter name="filePath">C:/Users/tap45/OneDrive/Documents/Overlay Projects/Overlay Music/Tap-Daw-main/auto_coder.py
