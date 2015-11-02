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



void plotZinvYields(){

  float sf_for_lumi = 1.0;///0.209;//to renormalize MC

  std::vector<TFile*> samples;
  std::vector<TH1F> sample_hists;
  std::vector<TString> regions;

  TFile* f1 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb/zinv_ht.root", "READ");
  samples.push_back(f1);
  TFile* f2 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb/purity.root", "READ");
  samples.push_back(f2);
  TFile* f3 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb/purityRL.root", "READ");
  //samples.push_back(f3);

  //TFile* f1 = new TFile("/home/users/jgran/temp/RS_update/MT2Analysis/MT2looper/output/25ns_golden_246908-258159_25nsV5JEC_RebalanceAndSmear/data.root", "READ");
  //samples.push_back(f1);
  //TFile* f2 = new TFile("/nfs-7/userdata/jgran/Smearing_corrTemplates_25ns_golden_246908-258159_25nsV5JEC_RebalanceAndSmear_data_Run2015D_JetHT_PromptReco/RS_data.root", "READ");
  //samples.push_back(f2);

  bool doSR = false;


  doSR = true;

  
//  regions.push_back("sr1J");
//  regions.push_back("sr2J");
//  regions.push_back("sr3J");
//  regions.push_back("sr4J");
//  regions.push_back("sr5J");
//  regions.push_back("sr6J");
//  regions.push_back("sr7J");
//  regions.push_back("sr11J");
//  regions.push_back("sr12J");
//  regions.push_back("sr13J");
//  regions.push_back("sr14J");
//  regions.push_back("sr15J");
//  regions.push_back("sr16J");
  

  if(doSR){
    regions.push_back("srbaseJ0B");
    regions.push_back("srbaseJ1B");
    regions.push_back("sr1VL");
    regions.push_back("sr2VL");
    regions.push_back("sr3VL");
    regions.push_back("sr4VL");
    regions.push_back("sr5VL");
    regions.push_back("sr6VL");
    regions.push_back("sr7VL");
    regions.push_back("sr8VL");
    regions.push_back("sr9VL");
    regions.push_back("sr10VL");
    regions.push_back("sr11VL");
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


  for(unsigned int i = 0; i < samples.size(); i++) {
    TH1F* h_sr_yields = new TH1F("h_sr_yields", "h_sr_yields", regions.size(), 0, regions.size());
    for(unsigned int j = 0; j < regions.size(); j++) {
      TString fullhistname = Form("%s/h_mt2bins", regions.at(j).Data());
      if (i==1) fullhistname = Form("%s/h_mt2binspredZFailSieieDatawithRuncInt", regions.at(j).Data());
      if (i==2) fullhistname = Form("%s/h_mt2binspredZ", regions.at(j).Data());
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

//  //use mc hist from Francesco
//  TFile* francesco_file = new TFile("qcd_histo.root", "READ");
//  TH1F* hfrancesco = (TH1F*) francesco_file->Get("h_mcTruth_tot");
//  sample_hists.at(0) = *hfrancesco;

  std::vector<std::string> labels;
  for(int i=0; i<regions.size(); i++){
    if (regions.at(i).Contains("srbaseJ")) {
      if (regions.at(i).Contains("srbaseJ0B")) labels.push_back("1j, 0b");
      if (regions.at(i).Contains("srbaseJ1B")) labels.push_back("1j, 1b");
    }
    else labels.push_back(getJetBJetPlotLabel(samples.at(0), regions.at(i).Data()));
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
      h_ratio->SetBinContent(i+1, rs/mc);
      h_ratio->SetBinError(i+1, (rs/mc)*sqrt( (rs_err/rs)*(rs_err/rs) + (mc_err/mc)*(mc_err/mc) ) );
    }
  }
  

  TFile* outfile = 0;
  if(doSR) outfile = new TFile("yield_histograms_Zinv.root", "RECREATE");
  else outfile = new TFile("yield_histograms.root", "RECREATE");
  outfile->cd();
  for(int i=0; i<sample_hists.size(); i++){
    if(i==0) sample_hists.at(i).Write("yields_MC");
    if(i==1) sample_hists.at(i).Write("yields_GJ");
    if(i==2) sample_hists.at(i).Write("yields_W");
  }
  h_pull->Write();
  h_ratio->Write();
  outfile->Close();

}
