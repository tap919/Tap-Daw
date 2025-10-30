#include "TelemetrySystem.h"
#include <iomanip>
#include <sstream>

namespace mydaw {

TelemetrySystem::TelemetrySystem(const std::string& filepath)
    : filepath_(filepath) {
    logFile_.open(filepath_, std::ios::app);
    
    if (logFile_.is_open()) {
        logFile_ << "=== Telemetry Log Started at " << getCurrentTimestamp() << " ===\n";
        logFile_.flush();
    }
}

TelemetrySystem::~TelemetrySystem() {
    if (logFile_.is_open()) {
        logFile_ << "=== Telemetry Log Ended at " << getCurrentTimestamp() << " ===\n";
        logFile_.close();
    }
}

void TelemetrySystem::logOverride(CircadianPhase fromPhase, CircadianPhase toPhase, const std::string& reason) {
    if (!logFile_.is_open()) {
        return;
    }
    
    logFile_ << "[" << getCurrentTimestamp() << "] "
             << "PHASE_OVERRIDE | From: " << phaseToString(fromPhase)
             << " | To: " << phaseToString(toPhase)
             << " | Reason: " << reason << "\n";
    logFile_.flush();
}

void TelemetrySystem::logEvent(const std::string& eventType, const std::string& data) {
    if (!logFile_.is_open()) {
        return;
    }
    
    logFile_ << "[" << getCurrentTimestamp() << "] "
             << eventType << " | " << data << "\n";
    logFile_.flush();
}

void TelemetrySystem::flush() {
    if (logFile_.is_open()) {
        logFile_.flush();
    }
}

std::string TelemetrySystem::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

} // namespace mydaw
