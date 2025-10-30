#include "CircadianPhaseManager.h"
#include <ctime>

namespace mydaw {

CircadianPhaseManager::CircadianPhaseManager()
    : overrideActive_(false)
    , overriddenPhase_(CircadianPhase::LEARN)
    , overrideReason_("") {
}

CircadianPhase CircadianPhaseManager::getCurrentPhase() const {
    if (overrideActive_) {
        return overriddenPhase_;
    }
    return determinePhaseFromTime();
}

std::string CircadianPhaseManager::getCurrentPhaseString() const {
    return phaseToString(getCurrentPhase());
}

void CircadianPhaseManager::overridePhase(CircadianPhase phase, const std::string& reason) {
    overrideActive_ = true;
    overriddenPhase_ = phase;
    overrideReason_ = reason;
}

void CircadianPhaseManager::clearOverride() {
    overrideActive_ = false;
    overrideReason_ = "";
}

bool CircadianPhaseManager::isOverrideActive() const {
    return overrideActive_;
}

std::string CircadianPhaseManager::getOverrideReason() const {
    return overrideReason_;
}

CircadianPhase CircadianPhaseManager::determinePhaseFromTime() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_c);
    
    int hour = local_time->tm_hour;
    
    // Define phase schedule:
    // LEARN: 8am - 2pm (08:00 - 14:00)
    // SHIP: 2pm - 8pm (14:00 - 20:00)
    // REST: 8pm - 8am (20:00 - 08:00)
    
    if (hour >= 8 && hour < 14) {
        return CircadianPhase::LEARN;
    } else if (hour >= 14 && hour < 20) {
        return CircadianPhase::SHIP;
    } else {
        return CircadianPhase::REST;
    }
}

std::string phaseToString(CircadianPhase phase) {
    switch (phase) {
        case CircadianPhase::LEARN:
            return "LEARN";
        case CircadianPhase::SHIP:
            return "SHIP";
        case CircadianPhase::REST:
            return "REST";
        default:
            return "UNKNOWN";
    }
}

} // namespace mydaw
