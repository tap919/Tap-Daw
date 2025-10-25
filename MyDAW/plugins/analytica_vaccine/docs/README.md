# Analytica Vaccine

## Overview
A/B comparison plugin with intelligent problem detection and automatic correction.

## Type
Metric A/B Analysis Hybrid Plugin

## Priority
7

## Description
Professional analysis and correction tool combining A/B comparison, visual difference analysis, and intelligent problem detection with automatic fixes.

## Architecture

### Analysis Section
- A/B comparison: True bypass with level matching
- Difference analysis: Visual display of processing changes
- Problem detection: Clipping, phase issues, mud, harshness

### Vaccine Processing
- Auto de-esser: Intelligent sibilance detection and reduction
- Dynamic rumble filter: Subsonic cleanup with threshold
- Clipping prevention: Soft clipping with oversampling
- Phase correction: Mid-side balance optimization

### Metering
- Loudness: LUFS integrated and short-term
- Phase correlation: Stereo field analysis
- Dynamics: Peak to RMS ratio

## File Structure
```
analytica_vaccine/
├── src/
│   ├── AnalyticaVaccine.cpp   # Main plugin
│   ├── ABComparator.cpp        # A/B switching
│   ├── ProblemDetector.cpp     # Issue detection
│   └── AutoCorrection.cpp      # Fixes
├── include/
│   ├── AnalyticaVaccine.h
│   ├── ABComparator.h
│   ├── ProblemDetector.h
│   └── AutoCorrection.h
└── docs/
    └── README.md (this file)
```

## Usage
1. Enable analysis mode
2. Review detected problems
3. Apply automatic corrections
4. A/B compare processed vs original
5. Fine-tune correction parameters
6. Export or bypass as needed
