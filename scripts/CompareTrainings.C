//
// A script to compare TrkQualTraining plots from different trainings
//
void CompareTrainings(bool save_plots = false) {

  std::string plotsdir = "/exp/mu2e/app/users/edmonds/plots/2025-01-29/";
  std::string plot_prefix = "Mu2eTrkQual_CompareTrainings_v1.1_v2.0";

  const int n_versions = 2;
  std::string versions[n_versions] = {"v1.1", "v2"};
  Int_t colors[n_versions] = {kRed, kBlue};

  TCanvas* c_roc_curve = new TCanvas("c_roc_curve", "c_roc_curve");
  TCanvas* c_mom_res = new TCanvas("c_mom_res", "c_mom_res");
  c_mom_res->SetLogy();

  const int n_features = 7;
  std::string features[n_features] = {"nactive", "factive", "t0err", "fambig", "fitcon", "momerr", "fstraws"};
  bool feature_log_scales[n_features] = {false, true, true, true, false, true, true};
  TCanvas* c_high_qual_features[n_features];
  TCanvas* c_low_qual_features[n_features];
  for (int i_feature = 0; i_feature < n_features; ++i_feature) {
    c_high_qual_features[i_feature] = new TCanvas(Form("c_high_qual_features_%s", features[i_feature].c_str()), Form("c_high_qual_features_%s", features[i_feature].c_str()));
    c_high_qual_features[i_feature]->SetLogy(feature_log_scales[i_feature]);

    c_low_qual_features[i_feature] = new TCanvas(Form("c_low_qual_features_%s", features[i_feature].c_str()), Form("c_low_qual_features_%s", features[i_feature].c_str()));
    c_low_qual_features[i_feature]->SetLogy(feature_log_scales[i_feature]);
  }

  TLegend* leg = new TLegend(0.75, 0.75, 0.9, 0.9);
  leg->SetTextSize(0.05);

  for (int i_version = 0; i_version < n_versions; ++i_version) {
    std::string version = versions[i_version];
    std::string filename = "TrkQual_ANN1_" + version + "_plots.root";
    Int_t color = colors[i_version];

    TFile* file = new TFile(filename.c_str(), "READ");

    c_roc_curve->cd();
    double fpr = 0;
    double tpr = 0;
    float thresh = 0;
    TTree* roc_curve = (TTree*) file->Get("roc_curve");
    roc_curve->SetBranchAddress("fpr", &fpr);
    roc_curve->SetBranchAddress("tpr", &tpr);
    roc_curve->SetBranchAddress("thresh", &thresh);
    int n_entries = roc_curve->GetEntries();
    TVectorD x;
    x.ResizeTo(n_entries);
    TVectorD y;
    y.ResizeTo(n_entries);
    for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
      roc_curve->GetEntry(i_entry);
      x[i_entry] = tpr;
      y[i_entry] = 1-fpr;
    }

    TGraph* gr_roc_curve = new TGraph(x, y);
    gr_roc_curve->SetLineColor(color);
    gr_roc_curve->SetLineWidth(2);
    if (i_version == 0) {
      gr_roc_curve->Draw("AL");
      gr_roc_curve->GetXaxis()->SetTitle("True Positive Rate");
      gr_roc_curve->GetYaxis()->SetTitle("1 - False Positive Rate");
    }
    else {
      gr_roc_curve->Draw("LSAME");
    }

    leg->AddEntry(gr_roc_curve, version.c_str(), "l");

    c_mom_res->cd();
    std::string histname = "all_mom_res";
    TH1F* h_mom_res = (TH1F*) file->Get(histname.c_str());
    h_mom_res->SetTitle("High-Quality Mom Res");
    h_mom_res->SetLineWidth(2);
    h_mom_res->SetLineColor(color);
    h_mom_res->SetMinimum(0.1);
    if (i_version == 0) {
      h_mom_res->Draw("HIST");
    }
    else {
      h_mom_res->Draw("HIST SAME");
    }

    for (int i_feature = 0; i_feature < n_features; ++i_feature) {
      c_high_qual_features[i_feature]->cd();

      std::string histname = "high_qual_feature" + std::to_string(i_feature) + "_" + features[i_feature] + "_norm";
      TH1F* h_high_qual_feature = (TH1F*) file->Get(histname.c_str());
      h_high_qual_feature->SetTitle(std::string(features[i_feature] + " (High-Quality Tracks)").c_str());
      h_high_qual_feature->SetYTitle("normalized to unit area");
      h_high_qual_feature->SetXTitle(features[i_feature].c_str());
      h_high_qual_feature->SetLineWidth(2);
      h_high_qual_feature->SetLineColor(color);
      h_high_qual_feature->SetStats(false);
      if (i_version == 0) {
        h_high_qual_feature->Draw("HIST");
      }
      else {
        h_high_qual_feature->Draw("HIST SAME");
      }

      c_low_qual_features[i_feature]->cd();

      histname = "low_qual_feature" + std::to_string(i_feature) + "_" + features[i_feature] + "_norm";
      TH1F* h_low_qual_feature = (TH1F*) file->Get(histname.c_str());
      h_low_qual_feature->SetTitle(std::string(features[i_feature] + " (Low-Quality Tracks)").c_str());
      h_low_qual_feature->SetYTitle("normalized to unit area");
      h_low_qual_feature->SetXTitle(features[i_feature].c_str());
      h_low_qual_feature->SetLineWidth(2);
      h_low_qual_feature->SetLineColor(color);
      h_low_qual_feature->SetStats(false);
      if (i_version == 0) {
        h_low_qual_feature->Draw("HIST");
      }
      else {
        h_low_qual_feature->Draw("HIST SAME");
      }
    }
  }

  c_roc_curve->cd();
  leg->Draw();
  if (save_plots) {
    std::string pdfname = plotsdir + "/" + plot_prefix + "_" + c_roc_curve->GetName() + ".pdf";
    c_roc_curve->SaveAs(pdfname.c_str());
  }

  c_mom_res->cd();
  leg->Draw();
  if (save_plots) {
    std::string pdfname = plotsdir + "/" + plot_prefix + "_" + c_mom_res->GetName() + ".pdf";
    c_mom_res->SaveAs(pdfname.c_str());
  }

  for (int i_feature = 0; i_feature < n_features; ++i_feature) {
    c_high_qual_features[i_feature]->cd();
    leg->Draw();

    c_low_qual_features[i_feature]->cd();
    leg->Draw();

    if (save_plots) {
      std::string pdfname = plotsdir + "/" + plot_prefix + "_" + c_high_qual_features[i_feature]->GetName() + ".pdf";
      c_high_qual_features[i_feature]->SaveAs(pdfname.c_str());

      pdfname = plotsdir + "/" + plot_prefix + "_" + c_low_qual_features[i_feature]->GetName() + ".pdf";
      c_low_qual_features[i_feature]->SaveAs(pdfname.c_str());
    }
  }
}
