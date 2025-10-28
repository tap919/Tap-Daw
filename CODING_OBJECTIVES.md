# TAP-DAW Phase 1 Completion - Coding Objectives

## Project Context
- **Build System**: CMake 3.22+ with JUCE framework
- **Target Platform**: Windows (Visual Studio 2022, x64)
- **Language**: C++17
- **Current State**: Phase 1 already has JUCE integrated and basic build structure

## Phase 1: Session Core - OBJECTIVES

### 1. Complete Session Management (HIGH PRIORITY)
**Files**: `phases/daw_phase_1_session_core/src/model/Session.h`, `Session.cpp`

**Requirements**:
- Implement full session schema with:
  - Tempo map (BPM, time signature changes)
  - Track list (name, color, volume, pan, mute, solo)
  - Clip system (start time, duration, file reference, gain, fade in/out)
  - Global settings (sample rate, buffer size, master volume)

- JSON Serialization:
  - Save session to `.tapdaw.json` format
  - Load session from JSON with validation
  - Handle malformed JSON gracefully
  - Version control for file format

**Code Structure**:
```cpp
class Session {
    double bpm_;
    int sampleRate_;
    std::vector<Track> tracks_;
    std::vector<TempoChange> tempoMap_;
    
    bool saveToFile(const std::string& path);
    bool loadFromFile(const std::string& path);
    nlohmann::json toJSON() const;
    void fromJSON(const nlohmann::json& j);
};
```

### 2. Timeline View Component (HIGH PRIORITY)
**Files**: `phases/daw_phase_1_session_core/src/ui/Timeline.h`, `Timeline.cpp`

**Requirements**:
- Visual timeline with:
  - Horizontal time ruler (bars/beats, timecode)
  - Track lanes (vertically stacked)
  - Clip rectangles with labels
  - Zoom controls (horizontal and vertical)
  - Playhead/cursor position
  
- Rendering:
  - Draw grid lines for bars and beats
  - Color-coded tracks
  - Clip names overlaid on rectangles
  - Handle resize and scrolling
  
- Mouse interaction (Phase 1 = read-only):
  - Scroll timeline
  - Zoom in/out
  - No editing yet (that's Phase 3)

### 3. Track List Component (MEDIUM PRIORITY)
**Files**: `phases/daw_phase_1_session_core/src/ui/TrackList.h`, `TrackList.cpp`

**Requirements**:
- Left sidebar showing:
  - Track names (editable labels)
  - Track colors
  - Volume faders
  - Mute/Solo buttons
  - Record arm button
  
- Layout:
  - Vertical list matching timeline lanes
  - Fixed width (200-250px)
  - Scrolls with timeline vertical scroll

### 4. Main Application Window (MEDIUM PRIORITY)
**Files**: `phases/daw_phase_1_session_core/src/MainComponent.h`, `MainComponent.cpp`

**Requirements**:
- Layout management:
  - Menu bar (File: New, Open, Save, Save As, Exit)
  - Transport bar (Play, Stop, Record buttons - disabled in Phase 1)
  - TrackList on left
  - Timeline fills remaining space
  
- File menu functionality:
  - New: Create blank session with 2 demo tracks
  - Open: File dialog → load .tapdaw.json
  - Save: Save to current file (or Save As if untitled)
  - Save As: File dialog → save with new name

### 5. Demo Content Generator (LOW PRIORITY)
**Files**: `phases/daw_phase_1_session_core/src/model/DemoSession.h`, `DemoSession.cpp`

**Requirements**:
- Create default session on launch:
  - 2 audio tracks ("Drums", "Bass")
  - 4 clips (2 per track)
  - Clips positioned at 0.0s, 4.0s per track
  - Duration: 2.0s each clip
  - BPM: 120
  - Time signature: 4/4

### 6. Build System Optimization (HIGH PRIORITY)
**Files**: `phases/daw_phase_1_session_core/CMakeLists.txt`, `setup.ps1`

**Requirements**:
- Ensure clean build process:
  - Check if JUCE submodule exists before adding
  - Handle incremental builds
  - Add install target for executable
  - Create package/distribution folder
  
- Add build configurations:
  - Debug (with symbols, no optimization)
  - Release (optimized, no debug info)
  - RelWithDebInfo (optimized + symbols)

### 7. Error Handling & Logging (MEDIUM PRIORITY)
**Create new files**: `phases/daw_phase_1_session_core/src/common/Logger.h`, `Logger.cpp`

**Requirements**:
- Implement logging system:
  - Log levels: DEBUG, INFO, WARN, ERROR
  - File output: logs/tapdaw.log
  - Console output in debug builds
  - Timestamp each message
  
- Error handling:
  - Try-catch in JSON parsing
  - File I/O error messages
  - User-friendly error dialogs

### 8. Testing & Validation (MEDIUM PRIORITY)
**Create**: `phases/daw_phase_1_session_core/tests/` directory

**Test Plan**:
1. Launch app → verify demo session displays
2. File → Save As → save test.tapdaw.json
3. File → New → verify reset to demo
4. File → Open → load test.tapdaw.json
5. Verify clip positions and names match saved data
6. Test zoom in/out on timeline
7. Test scroll timeline horizontally/vertically
8. Close and reopen → should start with demo again

### 9. Code Quality Requirements (ALL PRIORITIES)

**Standards**:
- Use modern C++17 features (smart pointers, auto, range-for)
- RAII for resource management
- Const-correctness everywhere
- Clear variable names (no single letters except loop counters)
- Comment complex algorithms
- Separate interface (.h) from implementation (.cpp)

**JUCE Best Practices**:
- Use `juce::` namespace explicitly
- Prefer `juce::String` over `std::string` for UI
- Use `juce::ValueTree` for complex hierarchical data
- Component ownership: parent owns children
- Use `MessageManager::callAsync` for thread safety

### 10. Documentation (LOW PRIORITY)
**Files**: Update `phases/daw_phase_1_session_core/README.md`

**Content**:
- Installation instructions
- Build instructions for Windows/Mac/Linux
- Basic usage guide
- JSON file format specification
- Known limitations
- Roadmap to Phase 2

## Success Criteria

**Phase 1 is complete when**:
✅ Application launches without errors
✅ Demo session displays 2 tracks with 4 clips
✅ File → Save As successfully saves .tapdaw.json
✅ File → Open successfully loads saved file
✅ Timeline shows correct track/clip layout
✅ TrackList shows track names and controls
✅ No audio playback (expected for Phase 1)
✅ No editing capabilities (expected for Phase 1)
✅ Build completes in < 5 minutes on modern hardware
✅ No compiler warnings with /W4 (MSVC) or -Wall (GCC/Clang)

## Dependencies

**Required**:
- JUCE 7.x (already in extern/JUCE as submodule)
- nlohmann/json (add as header-only library)
- CMake 3.22+
- Visual Studio 2022 (Windows) or Xcode 14+ (Mac) or GCC 11+ (Linux)

**Optional**:
- Google Test (for unit testing)
- Doxygen (for documentation generation)

## Build Commands

```powershell
# Setup (first time)
cd phases/daw_phase_1_session_core
git submodule update --init --recursive
cmake -B build -S . -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Release

# Run
build/Release/TapDAW_Phase1.exe

# Clean rebuild
cmake --build build --config Release --clean-first
```

## File Structure After Completion

```
phases/daw_phase_1_session_core/
├── CMakeLists.txt
├── setup.ps1
├── README.md
├── extern/
│   └── JUCE/ (submodule)
├── src/
│   ├── Main.cpp
│   ├── MainComponent.h
│   ├── MainComponent.cpp
│   ├── common/
│   │   ├── Logger.h
│   │   └── Logger.cpp
│   ├── model/
│   │   ├── Session.h
│   │   ├── Session.cpp
│   │   ├── DemoSession.h
│   │   └── DemoSession.cpp
│   └── ui/
│       ├── Timeline.h
│       ├── Timeline.cpp
│       ├── TrackList.h
│       └── TrackList.cpp
├── tests/
│   └── SessionTests.cpp
├── build/ (generated)
└── logs/ (generated at runtime)
```

## Notes for Automated Coder

1. **Preserve existing code**: JUCE is already integrated, don't re-download
2. **Follow JUCE patterns**: Use Component inheritance, MessageListener
3. **C++17 only**: Don't use C++20 features
4. **Windows-first**: Mac/Linux compatibility is nice-to-have
5. **No audio yet**: Phase 1 is purely visual/session management
6. **JSON library**: Use nlohmann/json (single header file)
7. **Focus on stability**: Better to have fewer features that work perfectly

## Audit Checklist for GitHub Copilot

After auto-coding, verify:
- [ ] All files compile without errors
- [ ] No memory leaks (smart pointers used correctly)
- [ ] JSON save/load works with malformed input
- [ ] UI renders without flickering
- [ ] File dialogs work on Windows
- [ ] Demo session loads on startup
- [ ] Zoom/scroll doesn't crash
- [ ] Code follows C++17 standards
- [ ] No hardcoded paths (use JUCE File class)
- [ ] Comments explain non-obvious code
