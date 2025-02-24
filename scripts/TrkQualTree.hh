//
// An example of how to create a flatter tree if you want to do an unbinned lieklihood fit
// This uses cut functions defined in common_cuts.hh
//

#include "EventNtuple/utils/rooutil/inc/RooUtil.hh"
#include "EventNtuple/utils/rooutil/inc/common_cuts.hh"

#include "TH1F.h"

void TrkQualTree(std::string filename, std::string outfilename) {

  std::cout << "AE: here" << std::endl;
  // Create an output file
  //  TFile* outfile = new TFile("/exp/mu2e/data/users/edmonds/TrkQualTraining/trkqual_tree.root", "RECREATE");
  //  TFile* outfile = new TFile("/exp/mu2e/data/users/edmonds/TrkQualTraining/trkqual_tree_ceEndpoint.root", "RECREATE");
  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  // Create the TTree
  TTree* tree = new TTree("trkqualtree", "Tree for training TrkQual");
  // Here we will write out one track per row along with its track segment at the tracker entrance
  mu2e::EventInfo evtinfo;
  mu2e::TrkInfo trk;
  mu2e::TrkSegInfo trk_ent;
  mu2e::SurfaceStepInfo trk_ent_mc;
  mu2e::LoopHelixInfo trk_pars;
  mu2e::SimInfo trk_sim;
  tree->Branch("evtinfo.", &evtinfo);
  tree->Branch("trk.", &trk);
  tree->Branch("trk_ent.", &trk_ent);
  tree->Branch("trk_ent_mc.", &trk_ent_mc);
  tree->Branch("trk_pars.", &trk_pars);
  tree->Branch("trk_sim.", &trk_sim);

  // Set up RooUtil
  RooUtil util(filename);
  //  util.Debug(true);
  // Loop through the events
  for (int i_event = 0; i_event < util.GetNEvents(); ++i_event) {
    // Get the next event
    auto& event = util.GetEvent(i_event);

    if (event.trk->size() != event.trkmc->size()) { continue; } //temp fix

    evtinfo = *(event.evtinfo);
    // Get the e_minus tracks from the event
    auto e_minus_tracks = event.GetTracks([](Track& track) { return is_e_minus(track) && !is_reflected(track); });

    // Loop through the e_minus tracks
    for (auto& track : e_minus_tracks) {
      trk = *(track.trk); // we will fill this data into the tree

      // Get the track segments at the tracker entrance and has an MC step
      auto trk_ent_segments = track.GetSegments([](TrackSegment& segment){ return tracker_entrance(segment) && has_mc_step(segment) && has_reco_step(segment) && is_downstream(segment); });

      // Loop through the tracker entrance track segments
      for (auto& segment : trk_ent_segments) {
        trk_ent = *(segment.trkseg); // we will fill this data into the tree
        trk_ent_mc = *(segment.trksegmc); // we will fill this data into the tree
        trk_pars = *(segment.trksegpars_lh);
      }
      auto sim_particles = track.GetMCParticles(is_track_particle);
      trk_sim = *(sim_particles[0].mcsim);

      // Fill the tree once per track
      tree->Fill();
    }
    //    break;
  }

  outfile->cd();
  tree->Write();
  outfile->Close();
}
