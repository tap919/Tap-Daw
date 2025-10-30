#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include "CircadianPhaseManager.h"

namespace mydaw {

struct OverrideEntry {
    std::chrono::system_clock::time_point timestamp;
    CircadianPhase fromPhase;
    CircadianPhase toPhase;
    std::string reason;
};

class AlignmentLedger {
public:
    AlignmentLedger(const std::string& filepath);
    
    // Record an override event
    void recordOverride(const OverrideEntry& entry);
    
    // Get all override entries
    const std::vector<OverrideEntry>& getEntries() const;
    
    // Flush to disk
    void flush();
    
private:
    std::string filepath_;
    std::vector<OverrideEntry> entries_;
    std::ofstream logFile_;
    
    void writeEntry(const OverrideEntry& entry);
};

} // namespace mydaw
