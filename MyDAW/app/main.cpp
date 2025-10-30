#include <iostream>
#include <memory>
#include "CircadianPhaseManager.h"
#include "AlignmentLedger.h"
#include "TelemetrySystem.h"
#include "CircadianGUI.h"

int main(){
  std::cout << "MyDAW - Digital Audio Workstation with Circadian Phase Management\n\n";
  
  // Initialize core systems
  auto phaseManager = std::make_shared<mydaw::CircadianPhaseManager>();
  auto ledger = std::make_shared<mydaw::AlignmentLedger>("alignment_ledger.log");
  auto telemetry = std::make_shared<mydaw::TelemetrySystem>("telemetry.log");
  
  // Create and run GUI
  mydaw::CircadianGUI gui(phaseManager, ledger, telemetry);
  gui.run();
  
  return 0;
}
