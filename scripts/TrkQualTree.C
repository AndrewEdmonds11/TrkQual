//
// Select data for training TrkQual
// This uses cut functions defined in common_cuts.hh
//

#include "EventNtuple/rooutil/inc/RooUtil.hh"
#include "EventNtuple/rooutil/inc/common_cuts.hh"

#include "TH1F.h"

void TrkQualTree(std::string filename, std::string outfilename) {

  // Create an output file
  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  // Create the TTree
  TTree* tree = new TTree("trkqualtree", "Tree for training TrkQual");
  // Here we will write out one track per row along with its track segment at the tracker entrance
  mu2e::EventInfo evtinfo;
  mu2e::TrkInfo trk;
  mu2e::TrkSegInfo trk_ent;
  mu2e::SurfaceStepInfo trk_ent_mc;
  mu2e::TrkSegInfo trk_mid;
  mu2e::SurfaceStepInfo trk_mid_mc;
  mu2e::TrkSegInfo trk_xit;
  mu2e::SurfaceStepInfo trk_xit_mc;
  mu2e::LoopHelixInfo trk_ent_pars;
  mu2e::LoopHelixInfo trk_mid_pars;
  mu2e::LoopHelixInfo trk_xit_pars;
  mu2e::SimInfo trk_sim;
  tree->Branch("evtinfo.", &evtinfo);
  tree->Branch("trk.", &trk);
  tree->Branch("trk_ent.", &trk_ent);
  tree->Branch("trk_ent_mc.", &trk_ent_mc);
  tree->Branch("trk_mid.", &trk_mid);
  tree->Branch("trk_mid_mc.", &trk_mid_mc);
  tree->Branch("trk_xit.", &trk_xit);
  tree->Branch("trk_xit_mc.", &trk_xit_mc);
  tree->Branch("trk_ent_pars.", &trk_ent_pars);
  tree->Branch("trk_mid_pars.", &trk_mid_pars);
  tree->Branch("trk_xit_pars.", &trk_xit_pars);
  tree->Branch("trk_sim.", &trk_sim);

  // Set up RooUtil
  RooUtil util(filename);
  //  util.Debug(true);
  // Loop through the events
  int n_events = util.GetNEvents();
  std::cout << "Running over " << n_events << " events..." << std::endl;
  for (int i_event = 0; i_event < n_events; ++i_event) {

    // Get the next event
    auto& event = util.GetEvent(i_event);

    if (event.trk->size() != event.trkmc->size()) { continue; } //temp fix

    evtinfo = *(event.evtinfo);
    // Get the e_minus tracks from the event
    auto e_minus_tracks = event.GetTracks([](Track& track) { return is_e_minus(track); });

    // Loop through the e_minus tracks
    for (auto& track : e_minus_tracks) {
      trk = *(track.trk); // we will fill this data into the tree

      // Get the track segments at the tracker entrance and has an MC step
      auto trk_ent_segments = track.GetSegments([](TrackSegment& segment){ return tracker_entrance(segment) && has_mc_step(segment) && has_reco_step(segment) && is_downstream(segment); });
      auto trk_mid_segments = track.GetSegments([](TrackSegment& segment){ return tracker_middle(segment) && has_mc_step(segment) && has_reco_step(segment) && is_downstream(segment); });
      auto trk_xit_segments = track.GetSegments([](TrackSegment& segment){ return tracker_exit(segment) && has_mc_step(segment) && has_reco_step(segment) && is_downstream(segment); });
      if (trk_ent_segments.size() != 1 || trk_mid_segments.size() != 1 || trk_xit_segments.size() != 1) {
        // there should only be one of each
        continue; // to next track
      }

      auto trk_ent_seg = trk_ent_segments.at(0);
      auto trk_mid_seg = trk_mid_segments.at(0);
      auto trk_xit_seg = trk_xit_segments.at(0);

      // Want to require that the difference in momentum between the entrance and exit is less than 2 MeV
      if (std::fabs(trk_ent_seg.trkseg->mom.R() - trk_xit_seg.trkseg->mom.R()) > 2.0) {
        continue; // to next track
      }

      // Want to record this data in the tree
      trk_ent = *trk_ent_seg.trkseg;
      trk_ent_mc = *trk_ent_seg.trksegmc;
      trk_ent_pars = *trk_ent_seg.trksegpars_lh;

      trk_mid = *trk_mid_seg.trkseg;
      trk_mid_mc = *trk_mid_seg.trksegmc;
      trk_mid_pars = *trk_mid_seg.trksegpars_lh;

      trk_xit = *trk_xit_seg.trkseg;
      trk_xit_mc = *trk_xit_seg.trksegmc;
      trk_xit_pars = *trk_xit_seg.trksegpars_lh;

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
  std::cout << "Script complete." << std::endl;
}
