#include "CircadianGUI.h"
#include <iostream>
#include <limits>

namespace mydaw {

CircadianGUI::CircadianGUI(std::shared_ptr<CircadianPhaseManager> phaseManager,
                           std::shared_ptr<AlignmentLedger> ledger,
                           std::shared_ptr<TelemetrySystem> telemetry)
    : phaseManager_(phaseManager)
    , ledger_(ledger)
    , telemetry_(telemetry) {
}

void CircadianGUI::displayCurrentPhase() const {
    std::cout << "\n===========================================\n";
    std::cout << "        CIRCADIAN PHASE STATUS\n";
    std::cout << "===========================================\n";
    std::cout << "Current Phase: " << phaseManager_->getCurrentPhaseString() << "\n";
    
    if (phaseManager_->isOverrideActive()) {
        std::cout << "Override Active: YES\n";
        std::cout << "Override Reason: " << phaseManager_->getOverrideReason() << "\n";
    } else {
        std::cout << "Override Active: NO (automatic mode)\n";
    }
    std::cout << "===========================================\n\n";
}

void CircadianGUI::showOverrideControls() {
    std::cout << "\n--- Phase Override Controls ---\n";
    std::cout << "1. Override to LEARN phase\n";
    std::cout << "2. Override to SHIP phase\n";
    std::cout << "3. Override to REST phase\n";
    std::cout << "4. Clear override (return to automatic)\n";
    std::cout << "5. Return to main menu\n";
    std::cout << "-------------------------------\n";
}

bool CircadianGUI::triggerOverride(CircadianPhase newPhase, const std::string& reason) {
    if (reason.empty()) {
        std::cout << "Error: Override reason cannot be empty.\n";
        return false;
    }
    
    CircadianPhase fromPhase = phaseManager_->getCurrentPhase();
    
    // Override the phase
    phaseManager_->overridePhase(newPhase, reason);
    
    // Record in alignment ledger
    OverrideEntry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.fromPhase = fromPhase;
    entry.toPhase = newPhase;
    entry.reason = reason;
    ledger_->recordOverride(entry);
    
    // Log to telemetry
    telemetry_->logOverride(fromPhase, newPhase, reason);
    
    std::cout << "\n✓ Phase override applied successfully!\n";
    std::cout << "  From: " << phaseToString(fromPhase) << "\n";
    std::cout << "  To: " << phaseToString(newPhase) << "\n";
    std::cout << "  Reason: " << reason << "\n";
    
    return true;
}

void CircadianGUI::clearOverride() {
    if (!phaseManager_->isOverrideActive()) {
        std::cout << "\nNo active override to clear.\n";
        return;
    }
    
    phaseManager_->clearOverride();
    telemetry_->logEvent("OVERRIDE_CLEARED", "Returned to automatic phase determination");
    std::cout << "\n✓ Override cleared. Returned to automatic phase.\n";
}

void CircadianGUI::displayMenu() const {
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║      MyDAW - Circadian Phase      ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "1. Display current phase\n";
    std::cout << "2. Override phase\n";
    std::cout << "3. Clear override\n";
    std::cout << "4. Exit\n";
    std::cout << "────────────────────────────────────\n";
    std::cout << "Select option: ";
}

std::string CircadianGUI::promptForReason() const {
    std::cout << "\nEnter the reason for this override:\n> ";
    std::string reason;
    std::getline(std::cin, reason);
    return reason;
}

void CircadianGUI::run() {
    telemetry_->logEvent("GUI_STARTED", "Circadian GUI initialized");
    
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                displayCurrentPhase();
                break;
                
            case 2: {
                displayCurrentPhase();
                showOverrideControls();
                std::cout << "Select phase to override to: ";
                int phaseChoice;
                if (!(std::cin >> phaseChoice)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nInvalid input.\n";
                    break;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (phaseChoice == 5) {
                    break; // Return to main menu
                } else if (phaseChoice == 4) {
                    clearOverride();
                    break;
                }
                
                CircadianPhase newPhase;
                switch (phaseChoice) {
                    case 1:
                        newPhase = CircadianPhase::LEARN;
                        break;
                    case 2:
                        newPhase = CircadianPhase::SHIP;
                        break;
                    case 3:
                        newPhase = CircadianPhase::REST;
                        break;
                    default:
                        std::cout << "\nInvalid phase selection.\n";
                        continue;
                }
                
                std::string reason = promptForReason();
                triggerOverride(newPhase, reason);
                break;
            }
            
            case 3:
                clearOverride();
                break;
                
            case 4:
                std::cout << "\nExiting MyDAW Circadian Phase GUI...\n";
                telemetry_->logEvent("GUI_EXITED", "User requested exit");
                running = false;
                break;
                
            default:
                std::cout << "\nInvalid option. Please select 1-4.\n";
                break;
        }
    }
}

} // namespace mydaw
