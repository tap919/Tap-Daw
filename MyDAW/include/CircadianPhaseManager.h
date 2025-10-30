#pragma once
#include <string>
#include <chrono>

namespace mydaw {

enum class CircadianPhase {
    LEARN,  // Learning/creative phase
    SHIP,   // Shipping/production phase
    REST    // Rest/recovery phase
};

class CircadianPhaseManager {
public:
    CircadianPhaseManager();
    
    // Get current phase based on time
    CircadianPhase getCurrentPhase() const;
    
    // Get current phase as string
    std::string getCurrentPhaseString() const;
    
    // Override current phase
    void overridePhase(CircadianPhase phase, const std::string& reason);
    
    // Clear override and return to automatic phase
    void clearOverride();
    
    // Check if override is active
    bool isOverrideActive() const;
    
    // Get override reason (if any)
    std::string getOverrideReason() const;

private:
    CircadianPhase determinePhaseFromTime() const;
    
    bool overrideActive_;
    CircadianPhase overriddenPhase_;
    std::string overrideReason_;
};

std::string phaseToString(CircadianPhase phase);

} // namespace mydaw
