std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}

std::string getJetBJetPlotLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_njets_LOW = (TH1D*) f->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_UP = (TH1D*) f->Get(dir+"/h_njets_HI");
  int njets_LOW;
  int njets_UP;
  if(h_njets_LOW && h_njets_UP){
    njets_LOW = h_njets_LOW->GetBinContent(1);
    njets_UP = h_njets_UP->GetBinContent(1);
  }
  else{
    njets_LOW = 0;
    njets_UP = -1;
  }

  TH1D* h_nbjets_LOW = (TH1D*) f->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_UP = (TH1D*) f->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW;
  int nbjets_UP;
  if(h_nbjets_LOW && h_nbjets_UP){
    nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    nbjets_UP = h_nbjets_UP->GetBinContent(1);
  }
  else{
    nbjets_LOW = 0;
    nbjets_UP = -1;
  }

  if(njets_UP != -1) njets_UP--;
  if(nbjets_UP != -1) nbjets_UP--;

  std::string jet_string;
  std::string bjet_string;

  if( (njets_UP - njets_LOW) == 0) jet_string = toString(njets_LOW) + "j";
  else if( njets_UP != -1) jet_string = toString(njets_LOW) + "-" + toString(njets_UP) + "j";
  else jet_string = "#geq " + toString(njets_LOW) + "j";

  if( (nbjets_UP - nbjets_LOW) == 0) bjet_string = toString(nbjets_LOW) + "b";
  else if( nbjets_UP != -1) bjet_string = toString(nbjets_LOW) + "-" + toString(nbjets_UP) + "b";
  else bjet_string = "#geq " + toString(nbjets_LOW) + "b";

  return jet_string + ", " + bjet_string;

}



void plotSRYields(){

  float sf_for_lumi = 1.0/0.209;//to renormalize MC

  std::vector<TFile*> samples;
  std::vector<TH1F> sample_hists;
  std::vector<std::string> regions;

  TFile* f1 = new TFile("/home/users/jgran/temp/RS_update/MT2Analysis/MT2looper/output/Spring15_QCD_MG_controlRegions/qcd_ht_MC.root", "READ");
  samples.push_back(f1);
  TFile* f2 = new TFile("/nfs-7/userdata/jgran/Smearing_controlRegions_Spring15_QCD_MG_sigmasoft20_25nsV2JEC_jetpt10_qcd_htALL/RS_qcd_ht.root", "READ");
  samples.push_back(f2);

  //TFile* f1 = new TFile("/home/users/jgran/temp/RS_update/MT2Analysis/MT2looper/output/25ns_golden_246908-258159_25nsV5JEC_RebalanceAndSmear/data.root", "READ");
  //samples.push_back(f1);
  //TFile* f2 = new TFile("/nfs-7/userdata/jgran/Smearing_corrTemplates_25ns_golden_246908-258159_25nsV5JEC_RebalanceAndSmear_data_Run2015D_JetHT_PromptReco/RS_data.root", "READ");
  //samples.push_back(f2);

  bool doSR = false;
  bool doInvertDPhi = false;
  bool doMT2SideBand = false;
  bool doDPhiMT2 = false;

  doSR = true;
  //doInvertDPhi = true;
  //doMT2SideBand = true;
  //doDPhiMT2 = true;
  

  if(doSR){
    regions.push_back("sr1L");
    regions.push_back("sr2L");
    regions.push_back("sr3L");
    regions.push_back("sr4L");
    regions.push_back("sr5L");
    regions.push_back("sr6L");
    regions.push_back("sr7L");
    regions.push_back("sr8L");
    regions.push_back("sr9L");
    regions.push_back("sr10L");
    regions.push_back("sr11L");
    regions.push_back("sr1M");
    regions.push_back("sr2M");
    regions.push_back("sr3M");
    regions.push_back("sr4M");
    regions.push_back("sr5M");
    regions.push_back("sr6M");
    regions.push_back("sr7M");
    regions.push_back("sr8M");
    regions.push_back("sr9M");
    regions.push_back("sr10M");
    regions.push_back("sr11M");
    regions.push_back("sr1H");
    regions.push_back("sr2H");
    regions.push_back("sr3H");
    regions.push_back("sr4H");
    regions.push_back("sr5H");
    regions.push_back("sr6H");
    regions.push_back("sr7H");
    regions.push_back("sr8H");
    regions.push_back("sr9H");
    regions.push_back("sr10H");
    regions.push_back("sr11H");
    regions.push_back("sr1UH");
    regions.push_back("sr2UH");
    regions.push_back("sr3UH");
    regions.push_back("sr4UH");
    regions.push_back("sr5UH");
    regions.push_back("sr6UH");
    regions.push_back("sr7UH");
    regions.push_back("sr8UH");
    regions.push_back("sr9UH");
    regions.push_back("sr10UH");
    regions.push_back("sr11UH");
  }
  else if(doInvertDPhi){
    regions.push_back("crRSInvertDPhi1L");
    regions.push_back("crRSInvertDPhi2L");
    regions.push_back("crRSInvertDPhi3L");
    regions.push_back("crRSInvertDPhi4L");
    regions.push_back("crRSInvertDPhi5L");
    regions.push_back("crRSInvertDPhi6L");
    regions.push_back("crRSInvertDPhi7L");
    regions.push_back("crRSInvertDPhi8L");
    regions.push_back("crRSInvertDPhi9L");
    regions.push_back("crRSInvertDPhi10L");
    regions.push_back("crRSInvertDPhi11L");
    regions.push_back("crRSInvertDPhi1M");
    regions.push_back("crRSInvertDPhi2M");
    regions.push_back("crRSInvertDPhi3M");
    regions.push_back("crRSInvertDPhi4M");
    regions.push_back("crRSInvertDPhi5M");
    regions.push_back("crRSInvertDPhi6M");
    regions.push_back("crRSInvertDPhi7M");
    regions.push_back("crRSInvertDPhi8M");
    regions.push_back("crRSInvertDPhi9M");
    regions.push_back("crRSInvertDPhi10M");
    regions.push_back("crRSInvertDPhi11M");
    regions.push_back("crRSInvertDPhi1H");
    regions.push_back("crRSInvertDPhi2H");
    regions.push_back("crRSInvertDPhi3H");
    regions.push_back("crRSInvertDPhi4H");
    regions.push_back("crRSInvertDPhi5H");
    regions.push_back("crRSInvertDPhi6H");
    regions.push_back("crRSInvertDPhi7H");
    regions.push_back("crRSInvertDPhi8H");
    regions.push_back("crRSInvertDPhi9H");
    regions.push_back("crRSInvertDPhi10H");
    regions.push_back("crRSInvertDPhi11H");
    regions.push_back("crRSInvertDPhi1UH");
    regions.push_back("crRSInvertDPhi2UH");
    regions.push_back("crRSInvertDPhi3UH");
    regions.push_back("crRSInvertDPhi4UH");
    regions.push_back("crRSInvertDPhi5UH");
    regions.push_back("crRSInvertDPhi6UH");
    regions.push_back("crRSInvertDPhi7UH");
    regions.push_back("crRSInvertDPhi8UH");
    regions.push_back("crRSInvertDPhi9UH");
    regions.push_back("crRSInvertDPhi10UH");
    regions.push_back("crRSInvertDPhi11UH");
  }
  else if(doMT2SideBand){
    regions.push_back("crRSMT2SideBand1L");
    regions.push_back("crRSMT2SideBand2L");
    regions.push_back("crRSMT2SideBand3L");
    regions.push_back("crRSMT2SideBand4L");
    regions.push_back("crRSMT2SideBand5L");
    regions.push_back("crRSMT2SideBand6L");
    regions.push_back("crRSMT2SideBand7L");
    regions.push_back("crRSMT2SideBand8L");
    regions.push_back("crRSMT2SideBand9L");
    regions.push_back("crRSMT2SideBand10L");
    regions.push_back("crRSMT2SideBand11L");
    regions.push_back("crRSMT2SideBand1M");
    regions.push_back("crRSMT2SideBand2M");
    regions.push_back("crRSMT2SideBand3M");
    regions.push_back("crRSMT2SideBand4M");
    regions.push_back("crRSMT2SideBand5M");
    regions.push_back("crRSMT2SideBand6M");
    regions.push_back("crRSMT2SideBand7M");
    regions.push_back("crRSMT2SideBand8M");
    regions.push_back("crRSMT2SideBand9M");
    regions.push_back("crRSMT2SideBand10M");
    regions.push_back("crRSMT2SideBand11M");
    regions.push_back("crRSMT2SideBand1H");
    regions.push_back("crRSMT2SideBand2H");
    regions.push_back("crRSMT2SideBand3H");
    regions.push_back("crRSMT2SideBand4H");
    regions.push_back("crRSMT2SideBand5H");
    regions.push_back("crRSMT2SideBand6H");
    regions.push_back("crRSMT2SideBand7H");
    regions.push_back("crRSMT2SideBand8H");
    regions.push_back("crRSMT2SideBand9H");
    regions.push_back("crRSMT2SideBand10H");
    regions.push_back("crRSMT2SideBand11H");
    regions.push_back("crRSMT2SideBand1UH");
    regions.push_back("crRSMT2SideBand2UH");
    regions.push_back("crRSMT2SideBand3UH");
    regions.push_back("crRSMT2SideBand4UH");
    regions.push_back("crRSMT2SideBand5UH");
    regions.push_back("crRSMT2SideBand6UH");
    regions.push_back("crRSMT2SideBand7UH");
    regions.push_back("crRSMT2SideBand8UH");
    regions.push_back("crRSMT2SideBand9UH");
    regions.push_back("crRSMT2SideBand10UH");
    regions.push_back("crRSMT2SideBand11UH");
  }
  else if(doDPhiMT2){
    regions.push_back("crRSDPhiMT21L");
    regions.push_back("crRSDPhiMT22L");
    regions.push_back("crRSDPhiMT23L");
    regions.push_back("crRSDPhiMT24L");
    regions.push_back("crRSDPhiMT25L");
    regions.push_back("crRSDPhiMT26L");
    regions.push_back("crRSDPhiMT27L");
    regions.push_back("crRSDPhiMT28L");
    regions.push_back("crRSDPhiMT29L");
    regions.push_back("crRSDPhiMT210L");
    regions.push_back("crRSDPhiMT211L");
    regions.push_back("crRSDPhiMT21M");
    regions.push_back("crRSDPhiMT22M");
    regions.push_back("crRSDPhiMT23M");
    regions.push_back("crRSDPhiMT24M");
    regions.push_back("crRSDPhiMT25M");
    regions.push_back("crRSDPhiMT26M");
    regions.push_back("crRSDPhiMT27M");
    regions.push_back("crRSDPhiMT28M");
    regions.push_back("crRSDPhiMT29M");
    regions.push_back("crRSDPhiMT210M");
    regions.push_back("crRSDPhiMT211M");
    regions.push_back("crRSDPhiMT21H");
    regions.push_back("crRSDPhiMT22H");
    regions.push_back("crRSDPhiMT23H");
    regions.push_back("crRSDPhiMT24H");
    regions.push_back("crRSDPhiMT25H");
    regions.push_back("crRSDPhiMT26H");
    regions.push_back("crRSDPhiMT27H");
    regions.push_back("crRSDPhiMT28H");
    regions.push_back("crRSDPhiMT29H");
    regions.push_back("crRSDPhiMT210H");
    regions.push_back("crRSDPhiMT211H");
    regions.push_back("crRSDPhiMT21UH");
    regions.push_back("crRSDPhiMT22UH");
    regions.push_back("crRSDPhiMT23UH");
    regions.push_back("crRSDPhiMT24UH");
    regions.push_back("crRSDPhiMT25UH");
    regions.push_back("crRSDPhiMT26UH");
    regions.push_back("crRSDPhiMT27UH");
    regions.push_back("crRSDPhiMT28UH");
    regions.push_back("crRSDPhiMT29UH");
    regions.push_back("crRSDPhiMT210UH");
    regions.push_back("crRSDPhiMT211UH");
  }

  for(unsigned int i = 0; i < samples.size(); i++) {
    TH1F* h_sr_yields = new TH1F("h_sr_yields", "h_sr_yields", regions.size(), 0, regions.size());
    for(unsigned int j = 0; j < regions.size(); j++) {
      TString fullhistname = Form("%s/h_Events_w", regions.at(j).c_str());
      samples.at(i)->cd();
      TH1F* h = (TH1F*) samples.at(i)->Get(fullhistname);
      double yield = 0.;
      double err = 0.;
      if (!h) yield = 0.0;
      else yield = h->IntegralAndError(0,-1,err);
      yield *= sf_for_lumi;
      err *= sf_for_lumi;
      h_sr_yields->SetBinContent(j+1, yield);
      h_sr_yields->SetBinError(j+1, err);
      delete h;
    }
    sample_hists.push_back(*h_sr_yields);
    delete h_sr_yields;
  }

  //use mc hist from Francesco
  TFile* francesco_file = new TFile("qcd_histo.root", "READ");
  TH1F* hfrancesco = (TH1F*) francesco_file->Get("h_mcTruth_tot");
  sample_hists.at(0) = *hfrancesco;

  std::vector<std::string> labels;
  for(int i=0; i<regions.size(); i++){
    labels.push_back(getJetBJetPlotLabel(samples.at(0), regions.at(i)));
  }

  TCanvas* c1 = new TCanvas;
  c1->SetLogy();
  for(int i=0; i<labels.size(); i++){
    std::cout << labels.at(i) << std::endl;
    sample_hists.at(0).GetXaxis()->SetBinLabel(i+1, labels.at(i).c_str());
  }
  sample_hists.at(0).Draw();
  sample_hists.at(0).SetLineColor(kRed);
  for(unsigned int i=1; i<sample_hists.size(); i++){
    sample_hists.at(i).Draw("same");
  }
  //c1->Print("extremeHT.pdf");

  //hPull is filled with a SetBinContent (iBin, (estimate-MC)/sqrt(sigma(estimate)^2+sigma(MC)^2)), and a SetBinError(iBin, 1.0)
  TH1F* h_pull = new TH1F("h_pull", "h_pull", regions.size(), 0, regions.size());
  TH1F* h_ratio = new TH1F("h_ratio", "h_ratio", regions.size(), 0, regions.size());
  for(int i=0; i<regions.size(); i++){
    float mc = sample_hists.at(0).GetBinContent(i+1);
    float mc_err = sample_hists.at(0).GetBinError(i+1);
    float rs = sample_hists.at(1).GetBinContent(i+1);
    float rs_err = sample_hists.at(1).GetBinError(i+1);
    if(mc == 0 || rs == 0){
      h_pull->SetBinContent(i+1, 0);
      h_pull->SetBinError(i+1, 0);
    }
    else{
      h_pull->SetBinContent(i+1, (rs-mc)/sqrt(rs_err*rs_err + mc_err*mc_err) );
      h_pull->SetBinError(i+1, 1.0);
      h_ratio->SetBinContent(i+1, mc/rs);
      h_ratio->SetBinError(i+1, (mc/rs)*sqrt( (mc_err/mc)*(mc_err/mc) + (rs_err/rs)*(rs_err/rs) ) );
    }
  }
  

  TFile* outfile = 0;
  if(doSR) outfile = new TFile("yield_histograms_sr.root", "RECREATE");
  else if(doInvertDPhi) outfile = new TFile("yield_histograms_InvertDPhi.root", "RECREATE");
  else if(doMT2SideBand) outfile = new TFile("yield_histograms_MT2SideBand.root", "RECREATE");
  else if(doDPhiMT2) outfile = new TFile("yield_histograms_DPhiMT2.root", "RECREATE");
  else outfile = new TFile("yield_histograms.root", "RECREATE");
  outfile->cd();
  for(int i=0; i<sample_hists.size(); i++){
    if(i==0) sample_hists.at(i).Write("yields_MC");
    if(i==1) sample_hists.at(i).Write("yields_RS");
  }
  h_pull->Write();
  h_ratio->Write();
  outfile->Close();

}
