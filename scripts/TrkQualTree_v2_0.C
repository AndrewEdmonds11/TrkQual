#include "TrkQualTree.hh"

void TrkQualTree_v2_0() {
  std::string input_filename = "filelists/nts.mu2e.CeEndpointMix1BBTriggered.MDC2020an_v06_01_01_best_v1_3.root.100-file-list";
  std::string output_filename = "/exp/mu2e/data/users/edmonds/TrkQualTraining/trkqual_tree_v2.0.root";
  TrkQualTree(input_filename, output_filename);
}
