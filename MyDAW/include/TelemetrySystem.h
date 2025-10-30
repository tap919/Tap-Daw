#pragma once
#include <string>
#include <chrono>
#include <fstream>
#include "CircadianPhaseManager.h"

namespace mydaw {

class TelemetrySystem {
public:
    TelemetrySystem(const std::string& filepath);
    ~TelemetrySystem();
    
    // Log phase override event
    void logOverride(CircadianPhase fromPhase, CircadianPhase toPhase, const std::string& reason);
    
    // Log general event
    void logEvent(const std::string& eventType, const std::string& data);
    
    // Flush to disk
    void flush();
    
private:
    std::string filepath_;
    std::ofstream logFile_;
    
    std::string getCurrentTimestamp() const;
};

} // namespace mydaw
