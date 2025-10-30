#include "AlignmentLedger.h"
#include <iomanip>
#include <sstream>

namespace mydaw {

AlignmentLedger::AlignmentLedger(const std::string& filepath)
    : filepath_(filepath) {
    logFile_.open(filepath_, std::ios::app);
}

void AlignmentLedger::recordOverride(const OverrideEntry& entry) {
    entries_.push_back(entry);
    writeEntry(entry);
}

const std::vector<OverrideEntry>& AlignmentLedger::getEntries() const {
    return entries_;
}

void AlignmentLedger::flush() {
    if (logFile_.is_open()) {
        logFile_.flush();
    }
}

void AlignmentLedger::writeEntry(const OverrideEntry& entry) {
    if (!logFile_.is_open()) {
        return;
    }
    
    auto timestamp = std::chrono::system_clock::to_time_t(entry.timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");
    
    logFile_ << "[" << ss.str() << "] "
             << "Override: " << phaseToString(entry.fromPhase)
             << " -> " << phaseToString(entry.toPhase)
             << " | Reason: " << entry.reason << "\n";
    logFile_.flush();
}

} // namespace mydaw
