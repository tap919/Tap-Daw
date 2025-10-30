#pragma once
#include <string>
#include <memory>
#include "CircadianPhaseManager.h"
#include "AlignmentLedger.h"
#include "TelemetrySystem.h"

namespace mydaw {

class CircadianGUI {
public:
    CircadianGUI(std::shared_ptr<CircadianPhaseManager> phaseManager,
                 std::shared_ptr<AlignmentLedger> ledger,
                 std::shared_ptr<TelemetrySystem> telemetry);
    
    // Display the current phase to the user
    void displayCurrentPhase() const;
    
    // Show override controls and prompt for reason
    void showOverrideControls();
    
    // Trigger a phase override
    bool triggerOverride(CircadianPhase newPhase, const std::string& reason);
    
    // Clear current override
    void clearOverride();
    
    // Run the GUI (simple text-based interface)
    void run();
    
private:
    std::shared_ptr<CircadianPhaseManager> phaseManager_;
    std::shared_ptr<AlignmentLedger> ledger_;
    std::shared_ptr<TelemetrySystem> telemetry_;
    
    void displayMenu() const;
    std::string promptForReason() const;
};

} // namespace mydaw
