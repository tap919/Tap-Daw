# MyDAW

Digital Audio Workstation scaffold with MIDI scheduler and Circadian Phase Management.

## Features

- MIDI scheduler integration
- **Circadian Phase Management System**
  - Real-time phase display (LEARN/SHIP/REST)
  - Phase override controls with reason capture
  - Dual logging (Alignment Ledger + Telemetry)
  - Automatic phase determination based on time of day

## Quick Start

```bash
mkdir -p build && cd build
cmake ..
make
./MyDAW
```

## Documentation

- [Circadian Phase System Guide](docs/CIRCADIAN_PHASE_SYSTEM.md)
- [Plugin Quick Start](../QUICK_START.md)

## Testing

Run the test suite:
```bash
cd build
./test_circadian_phase
```

