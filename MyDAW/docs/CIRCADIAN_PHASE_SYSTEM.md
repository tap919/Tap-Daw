# Circadian Phase System

## Overview

The MyDAW Circadian Phase System helps you align your work with natural productivity rhythms by tracking and managing three distinct phases throughout the day:

- **LEARN** (8am - 2pm): Optimal for learning, creative exploration, and experimentation
- **SHIP** (2pm - 8pm): Best for production work, shipping features, and completing tasks
- **REST** (8pm - 8am): Time for recovery, reflection, and rest

## Features

### 1. Real-time Phase Display
The GUI displays your current circadian phase in real-time based on the time of day. When in automatic mode, the phase adjusts automatically according to the schedule above.

### 2. Phase Override Controls
You can manually override the current phase when needed. Common reasons include:
- Emergency deadlines requiring work during REST phase
- Need for creative exploration during SHIP phase
- Taking a well-deserved break during LEARN or SHIP phase

### 3. Override Reason Capture
Every time you override a phase, the system prompts you to enter a reason. This creates accountability and helps you reflect on your work patterns.

### 4. Dual Logging System

#### Alignment Ledger
Located at `alignment_ledger.log`, this file maintains a structured history of all phase overrides:
```
[2025-10-30 12:52:04] Override: LEARN -> SHIP | Reason: Need to ship critical feature before deadline
```

#### Telemetry System
Located at `telemetry.log`, this file provides comprehensive event logging including:
- GUI start/stop events
- Phase override events with full details
- Override clear events
```
[2025-10-30 12:53:11] PHASE_OVERRIDE | From: LEARN | To: SHIP | Reason: Emergency deadline
```

## Usage

### Running the GUI

```bash
cd MyDAW/build
./MyDAW
```

### Menu Options

1. **Display current phase** - Shows the current phase and override status
2. **Override phase** - Trigger a phase override with reason capture
3. **Clear override** - Return to automatic phase determination
4. **Exit** - Close the application

### Example Workflow

1. Start MyDAW: `./MyDAW`
2. Select option **1** to see your current phase
3. If you need to override:
   - Select option **2**
   - Choose the desired phase (1=LEARN, 2=SHIP, 3=REST)
   - Enter a meaningful reason when prompted
4. The system logs the override to both ledger and telemetry
5. When done, select option **3** to return to automatic mode
6. Select option **4** to exit

## Testing

Run the test suite to verify the circadian phase system:

```bash
cd MyDAW/build
./test_circadian_phase
```

This validates:
- Phase determination logic
- Override functionality
- Alignment ledger recording
- Telemetry event logging

## Log Files

Both log files are automatically created in the same directory as the MyDAW executable:
- `alignment_ledger.log` - Override history
- `telemetry.log` - Complete event log

These files are excluded from version control (via .gitignore) but preserved for your review and analysis.

## Benefits

1. **Awareness**: Stay conscious of your work rhythms
2. **Accountability**: Document reasons for working outside optimal times
3. **Analysis**: Review logs to understand work patterns and improve planning
4. **Balance**: Encourage healthy work-life boundaries

## Phase Schedule Rationale

The default schedule is based on common productivity patterns:
- Morning (LEARN): Fresh mind, high creativity
- Afternoon (SHIP): Peak execution, task completion
- Evening (REST): Wind down, recovery

You can modify the schedule by editing `CircadianPhaseManager::determinePhaseFromTime()` in `src/CircadianPhaseManager.cpp`.
