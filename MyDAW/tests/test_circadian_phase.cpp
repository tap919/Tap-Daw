#include <iostream>
#include <cassert>
#include <memory>
#include "CircadianPhaseManager.h"
#include "AlignmentLedger.h"
#include "TelemetrySystem.h"

void testCircadianPhaseManager() {
    std::cout << "Testing CircadianPhaseManager...\n";
    
    mydaw::CircadianPhaseManager manager;
    
    // Test initial state - should be automatic (not overridden)
    assert(!manager.isOverrideActive());
    std::cout << "  ✓ Initial state is not overridden\n";
    
    // Test current phase determination (will vary by time of day)
    auto currentPhase = manager.getCurrentPhase();
    std::string phaseStr = manager.getCurrentPhaseString();
    std::cout << "  ✓ Current phase: " << phaseStr << "\n";
    
    // Test phase override
    manager.overridePhase(mydaw::CircadianPhase::LEARN, "Testing override");
    assert(manager.isOverrideActive());
    assert(manager.getCurrentPhase() == mydaw::CircadianPhase::LEARN);
    assert(manager.getOverrideReason() == "Testing override");
    std::cout << "  ✓ Override to LEARN phase successful\n";
    
    // Test clearing override
    manager.clearOverride();
    assert(!manager.isOverrideActive());
    assert(manager.getOverrideReason().empty());
    std::cout << "  ✓ Clear override successful\n";
    
    std::cout << "CircadianPhaseManager tests passed!\n\n";
}

void testAlignmentLedger() {
    std::cout << "Testing AlignmentLedger...\n";
    
    mydaw::AlignmentLedger ledger("/tmp/test_alignment_ledger.log");
    
    // Create and record an override entry
    mydaw::OverrideEntry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.fromPhase = mydaw::CircadianPhase::REST;
    entry.toPhase = mydaw::CircadianPhase::SHIP;
    entry.reason = "Urgent deadline";
    
    ledger.recordOverride(entry);
    
    // Verify entry was recorded
    const auto& entries = ledger.getEntries();
    assert(entries.size() == 1);
    assert(entries[0].reason == "Urgent deadline");
    std::cout << "  ✓ Override entry recorded\n";
    
    ledger.flush();
    std::cout << "  ✓ Ledger flushed to disk\n";
    
    std::cout << "AlignmentLedger tests passed!\n\n";
}

void testTelemetrySystem() {
    std::cout << "Testing TelemetrySystem...\n";
    
    mydaw::TelemetrySystem telemetry("/tmp/test_telemetry.log");
    
    // Log an override event
    telemetry.logOverride(mydaw::CircadianPhase::LEARN, 
                         mydaw::CircadianPhase::SHIP,
                         "Need to ship feature");
    std::cout << "  ✓ Override logged to telemetry\n";
    
    // Log a general event
    telemetry.logEvent("TEST_EVENT", "Test data");
    std::cout << "  ✓ General event logged\n";
    
    telemetry.flush();
    std::cout << "  ✓ Telemetry flushed to disk\n";
    
    std::cout << "TelemetrySystem tests passed!\n\n";
}

void testPhaseToString() {
    std::cout << "Testing phaseToString utility...\n";
    
    assert(mydaw::phaseToString(mydaw::CircadianPhase::LEARN) == "LEARN");
    assert(mydaw::phaseToString(mydaw::CircadianPhase::SHIP) == "SHIP");
    assert(mydaw::phaseToString(mydaw::CircadianPhase::REST) == "REST");
    
    std::cout << "  ✓ All phase strings correct\n";
    std::cout << "phaseToString tests passed!\n\n";
}

int main() {
    std::cout << "\n=== Running Circadian Phase System Tests ===\n\n";
    
    try {
        testPhaseToString();
        testCircadianPhaseManager();
        testAlignmentLedger();
        testTelemetrySystem();
        
        std::cout << "===========================================\n";
        std::cout << "All tests passed successfully! ✓\n";
        std::cout << "===========================================\n\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
