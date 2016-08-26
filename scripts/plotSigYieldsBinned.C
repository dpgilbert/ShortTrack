std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}

std::string getJetBJetPlotLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);
  
  if (dir.Contains("srbaseJ0B")) return "1j, 0b";
  if (dir.Contains("srbaseJ1B")) return "1j, #geq 1b";

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



void plotSigYieldsBinned(TString HTregion = "ALL"){

  float sf_for_lumi = 1.0;///0.209;//to renormalize MC

  std::string central_histname = "h_mt2bins";
  std::vector<std::string> vars;
  std::vector<TH1D*> vars_hists;
  std::vector<TString> regions;
  std::vector<int> mt2bins;

  //  TString sample = "T1bbbb_900_825";
  //  TString sample = "T1bbbb_900_850";
  //  TString sample = "T1bbbb_900_875";
  //  TString sample = "T1bbbb_1400_900";
  //  TString sample = "T1bbbb_1600_1";
  TString sample = "data_Run2016B";
  
  //  TFile* f1 = new TFile(Form("/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-08_25ns_miniaodv2_fastsim_sfs_2p1fb_sigtest2/%s.root",sample.Data()), "READ");
  TFile* f1 = new TFile(Form("/home/users/gzevi/MT2/MT2Analysis80X/MT2Analysis/MT2looper/output/V00-08-00_json_Cert_271036-273450_skim_base_mt2gt200_ZinvV4/%s.root",sample.Data()), "READ");

  //vars.push_back("btagsf_UP");
  //vars.push_back("btagsf_DN");

  if(HTregion == "J"){
   regions.push_back("sr1J"); mt2bins.push_back(1);
   regions.push_back("sr2J"); mt2bins.push_back(1);
   regions.push_back("sr3J"); mt2bins.push_back(1);
   regions.push_back("sr4J"); mt2bins.push_back(1);
   regions.push_back("sr5J"); mt2bins.push_back(1);
   regions.push_back("sr6J"); mt2bins.push_back(1);
   regions.push_back("sr7J"); mt2bins.push_back(1);
   regions.push_back("sr11J"); mt2bins.push_back(1);
   regions.push_back("sr12J"); mt2bins.push_back(1);
   regions.push_back("sr13J"); mt2bins.push_back(1);
   regions.push_back("sr14J"); mt2bins.push_back(1);
   regions.push_back("sr15J"); mt2bins.push_back(1);
  }
  if(HTregion == "VL"){
    regions.push_back("sr1VL"); mt2bins.push_back(3);
    regions.push_back("sr2VL"); mt2bins.push_back(3);
    regions.push_back("sr3VL"); mt2bins.push_back(3);
    regions.push_back("sr4VL"); mt2bins.push_back(3);
    regions.push_back("sr5VL"); mt2bins.push_back(3);
    regions.push_back("sr6VL"); mt2bins.push_back(3);
    regions.push_back("sr7VL"); mt2bins.push_back(1);
    regions.push_back("sr8VL"); mt2bins.push_back(1);
    regions.push_back("sr9VL"); mt2bins.push_back(1);
    regions.push_back("sr10VL"); mt2bins.push_back(2);
    regions.push_back("sr11VL"); mt2bins.push_back(1);
  }
  if(HTregion == "L"){
    regions.push_back("sr1L"); mt2bins.push_back(4);
    regions.push_back("sr2L"); mt2bins.push_back(4);
    regions.push_back("sr3L"); mt2bins.push_back(4);
    regions.push_back("sr4L"); mt2bins.push_back(4);
    regions.push_back("sr5L"); mt2bins.push_back(4);
    regions.push_back("sr6L"); mt2bins.push_back(4);
    regions.push_back("sr7L"); mt2bins.push_back(1);
    regions.push_back("sr8L"); mt2bins.push_back(2);
    regions.push_back("sr9L"); mt2bins.push_back(1);
    regions.push_back("sr10L"); mt2bins.push_back(2);
    regions.push_back("sr11L"); mt2bins.push_back(1);
  }
  if(HTregion == "M"){
    regions.push_back("sr1M"); mt2bins.push_back(5);
    regions.push_back("sr2M"); mt2bins.push_back(5);
    regions.push_back("sr3M"); mt2bins.push_back(4);
    regions.push_back("sr4M"); mt2bins.push_back(5);
    regions.push_back("sr5M"); mt2bins.push_back(4);
    regions.push_back("sr6M"); mt2bins.push_back(4);
    regions.push_back("sr7M"); mt2bins.push_back(3);
    regions.push_back("sr8M"); mt2bins.push_back(3);
    regions.push_back("sr9M"); mt2bins.push_back(3);
    regions.push_back("sr10M"); mt2bins.push_back(3);
    regions.push_back("sr11M"); mt2bins.push_back(3);
  }
  if(HTregion == "H"){
    regions.push_back("sr1H"); mt2bins.push_back(5);
    regions.push_back("sr2H"); mt2bins.push_back(4);
    regions.push_back("sr3H"); mt2bins.push_back(2);
    regions.push_back("sr4H"); mt2bins.push_back(5);
    regions.push_back("sr5H"); mt2bins.push_back(4);
    regions.push_back("sr6H"); mt2bins.push_back(3);
    regions.push_back("sr7H"); mt2bins.push_back(3);
    regions.push_back("sr8H"); mt2bins.push_back(3);
    regions.push_back("sr9H"); mt2bins.push_back(2);
    regions.push_back("sr10H"); mt2bins.push_back(2);
    regions.push_back("sr11H"); mt2bins.push_back(2);
  }
  if(HTregion == "UH"){
    regions.push_back("sr1UH"); mt2bins.push_back(5);
    regions.push_back("sr2UH"); mt2bins.push_back(3);
    regions.push_back("sr3UH"); mt2bins.push_back(1);
    regions.push_back("sr4UH"); mt2bins.push_back(5);
    regions.push_back("sr5UH"); mt2bins.push_back(3);
    regions.push_back("sr6UH"); mt2bins.push_back(3);
    regions.push_back("sr7UH"); mt2bins.push_back(2);
    regions.push_back("sr8UH"); mt2bins.push_back(2);
    regions.push_back("sr9UH"); mt2bins.push_back(2);
    regions.push_back("sr10UH"); mt2bins.push_back(2);
    regions.push_back("sr11UH"); mt2bins.push_back(2);
  }
  
  if(HTregion == "ALL"){
    regions.push_back("sr1J"); mt2bins.push_back(1);
    regions.push_back("sr2J"); mt2bins.push_back(1);
    regions.push_back("sr3J"); mt2bins.push_back(1);
    regions.push_back("sr4J"); mt2bins.push_back(1);
    regions.push_back("sr5J"); mt2bins.push_back(1);
    regions.push_back("sr6J"); mt2bins.push_back(1);
    regions.push_back("sr7J"); mt2bins.push_back(1);
    regions.push_back("sr11J"); mt2bins.push_back(1);
    regions.push_back("sr12J"); mt2bins.push_back(1);
    regions.push_back("sr13J"); mt2bins.push_back(1);
    regions.push_back("sr14J"); mt2bins.push_back(1);
    regions.push_back("sr15J"); mt2bins.push_back(1);
    regions.push_back("sr1VL"); mt2bins.push_back(3);
    regions.push_back("sr2VL"); mt2bins.push_back(3);
    regions.push_back("sr3VL"); mt2bins.push_back(3);
    regions.push_back("sr4VL"); mt2bins.push_back(3);
    regions.push_back("sr5VL"); mt2bins.push_back(3);
    regions.push_back("sr6VL"); mt2bins.push_back(3);
    regions.push_back("sr7VL"); mt2bins.push_back(1);
    regions.push_back("sr8VL"); mt2bins.push_back(1);
    regions.push_back("sr9VL"); mt2bins.push_back(1);
    regions.push_back("sr10VL"); mt2bins.push_back(2);
    regions.push_back("sr11VL"); mt2bins.push_back(1);
    regions.push_back("sr1L"); mt2bins.push_back(4);
    regions.push_back("sr2L"); mt2bins.push_back(4);
    regions.push_back("sr3L"); mt2bins.push_back(4);
    regions.push_back("sr4L"); mt2bins.push_back(4);
    regions.push_back("sr5L"); mt2bins.push_back(4);
    regions.push_back("sr6L"); mt2bins.push_back(4);
    regions.push_back("sr7L"); mt2bins.push_back(1);
    regions.push_back("sr8L"); mt2bins.push_back(2);
    regions.push_back("sr9L"); mt2bins.push_back(1);
    regions.push_back("sr10L"); mt2bins.push_back(2);
    regions.push_back("sr11L"); mt2bins.push_back(1);
    regions.push_back("sr1M"); mt2bins.push_back(5);
    regions.push_back("sr2M"); mt2bins.push_back(5);
    regions.push_back("sr3M"); mt2bins.push_back(4);
    regions.push_back("sr4M"); mt2bins.push_back(5);
    regions.push_back("sr5M"); mt2bins.push_back(4);
    regions.push_back("sr6M"); mt2bins.push_back(4);
    regions.push_back("sr7M"); mt2bins.push_back(3);
    regions.push_back("sr8M"); mt2bins.push_back(3);
    regions.push_back("sr9M"); mt2bins.push_back(3);
    regions.push_back("sr10M"); mt2bins.push_back(3);
    regions.push_back("sr11M"); mt2bins.push_back(3);
    regions.push_back("sr1H"); mt2bins.push_back(5);
    regions.push_back("sr2H"); mt2bins.push_back(4);
    regions.push_back("sr3H"); mt2bins.push_back(2);
    regions.push_back("sr4H"); mt2bins.push_back(5);
    regions.push_back("sr5H"); mt2bins.push_back(4);
    regions.push_back("sr6H"); mt2bins.push_back(3);
    regions.push_back("sr7H"); mt2bins.push_back(3);
    regions.push_back("sr8H"); mt2bins.push_back(3);
    regions.push_back("sr9H"); mt2bins.push_back(2);
    regions.push_back("sr10H"); mt2bins.push_back(2);
    regions.push_back("sr11H"); mt2bins.push_back(2);
    regions.push_back("sr1UH"); mt2bins.push_back(5);
    regions.push_back("sr2UH"); mt2bins.push_back(3);
    regions.push_back("sr3UH"); mt2bins.push_back(1);
    regions.push_back("sr4UH"); mt2bins.push_back(5);
    regions.push_back("sr5UH"); mt2bins.push_back(3);
    regions.push_back("sr6UH"); mt2bins.push_back(3);
    regions.push_back("sr7UH"); mt2bins.push_back(2);
    regions.push_back("sr8UH"); mt2bins.push_back(2);
    regions.push_back("sr9UH"); mt2bins.push_back(2);
    regions.push_back("sr10UH"); mt2bins.push_back(2);
    regions.push_back("sr11UH"); mt2bins.push_back(2);
  }

  if(HTregion == "ALLCRSL"){
    regions.push_back("crsl1J"); mt2bins.push_back(1);
    regions.push_back("crsl2J"); mt2bins.push_back(1);
    regions.push_back("crsl3J"); mt2bins.push_back(1);
    regions.push_back("crsl4J"); mt2bins.push_back(1);
    regions.push_back("crsl5J"); mt2bins.push_back(1);
    regions.push_back("crsl6J"); mt2bins.push_back(1);
    regions.push_back("crsl7J"); mt2bins.push_back(1);
    regions.push_back("crsl11J"); mt2bins.push_back(1);
    regions.push_back("crsl12J"); mt2bins.push_back(1);
    regions.push_back("crsl13J"); mt2bins.push_back(1);
    regions.push_back("crsl14J"); mt2bins.push_back(1);
    regions.push_back("crsl15J"); mt2bins.push_back(1);
    regions.push_back("crsl1VL"); mt2bins.push_back(3);
    regions.push_back("crsl2VL"); mt2bins.push_back(3);
    regions.push_back("crsl3VL"); mt2bins.push_back(3);
    regions.push_back("crsl4VL"); mt2bins.push_back(3);
    regions.push_back("crsl5VL"); mt2bins.push_back(3);
    regions.push_back("crsl6VL"); mt2bins.push_back(3);
    regions.push_back("crsl7VL"); mt2bins.push_back(1);
    regions.push_back("crsl8VL"); mt2bins.push_back(1);
    regions.push_back("crsl9VL"); mt2bins.push_back(1);
    regions.push_back("crsl10VL"); mt2bins.push_back(2);
    regions.push_back("crsl11VL"); mt2bins.push_back(1);
    regions.push_back("crsl1L"); mt2bins.push_back(4);
    regions.push_back("crsl2L"); mt2bins.push_back(4);
    regions.push_back("crsl3L"); mt2bins.push_back(4);
    regions.push_back("crsl4L"); mt2bins.push_back(4);
    regions.push_back("crsl5L"); mt2bins.push_back(4);
    regions.push_back("crsl6L"); mt2bins.push_back(4);
    regions.push_back("crsl7L"); mt2bins.push_back(1);
    regions.push_back("crsl8L"); mt2bins.push_back(2);
    regions.push_back("crsl9L"); mt2bins.push_back(1);
    regions.push_back("crsl10L"); mt2bins.push_back(2);
    regions.push_back("crsl11L"); mt2bins.push_back(1);
    regions.push_back("crsl1M"); mt2bins.push_back(5);
    regions.push_back("crsl2M"); mt2bins.push_back(5);
    regions.push_back("crsl3M"); mt2bins.push_back(4);
    regions.push_back("crsl4M"); mt2bins.push_back(5);
    regions.push_back("crsl5M"); mt2bins.push_back(4);
    regions.push_back("crsl6M"); mt2bins.push_back(4);
    regions.push_back("crsl7M"); mt2bins.push_back(3);
    regions.push_back("crsl8M"); mt2bins.push_back(3);
    regions.push_back("crsl9M"); mt2bins.push_back(3);
    regions.push_back("crsl10M"); mt2bins.push_back(3);
    regions.push_back("crsl11M"); mt2bins.push_back(3);
    regions.push_back("crsl1H"); mt2bins.push_back(5);
    regions.push_back("crsl2H"); mt2bins.push_back(4);
    regions.push_back("crsl3H"); mt2bins.push_back(2);
    regions.push_back("crsl4H"); mt2bins.push_back(5);
    regions.push_back("crsl5H"); mt2bins.push_back(4);
    regions.push_back("crsl6H"); mt2bins.push_back(3);
    regions.push_back("crsl7H"); mt2bins.push_back(3);
    regions.push_back("crsl8H"); mt2bins.push_back(3);
    regions.push_back("crsl9H"); mt2bins.push_back(2);
    regions.push_back("crsl10H"); mt2bins.push_back(2);
    regions.push_back("crsl11H"); mt2bins.push_back(2);
    regions.push_back("crsl1UH"); mt2bins.push_back(5);
    regions.push_back("crsl2UH"); mt2bins.push_back(3);
    regions.push_back("crsl3UH"); mt2bins.push_back(1);
    regions.push_back("crsl4UH"); mt2bins.push_back(5);
    regions.push_back("crsl5UH"); mt2bins.push_back(3);
    regions.push_back("crsl6UH"); mt2bins.push_back(3);
    regions.push_back("crsl7UH"); mt2bins.push_back(2);
    regions.push_back("crsl8UH"); mt2bins.push_back(2);
    regions.push_back("crsl9UH"); mt2bins.push_back(2);
    regions.push_back("crsl10UH"); mt2bins.push_back(2);
    regions.push_back("crsl11UH"); mt2bins.push_back(2);
  }
  if(HTregion == "ALLCRGJ"){
    regions.push_back("crgj1J"); mt2bins.push_back(1);
    regions.push_back("crgj2J"); mt2bins.push_back(1);
    regions.push_back("crgj3J"); mt2bins.push_back(1);
    regions.push_back("crgj4J"); mt2bins.push_back(1);
    regions.push_back("crgj5J"); mt2bins.push_back(1);
    regions.push_back("crgj6J"); mt2bins.push_back(1);
    regions.push_back("crgj7J"); mt2bins.push_back(1);
    regions.push_back("crgj11J"); mt2bins.push_back(1);
    regions.push_back("crgj12J"); mt2bins.push_back(1);
    regions.push_back("crgj13J"); mt2bins.push_back(1);
    regions.push_back("crgj14J"); mt2bins.push_back(1);
    regions.push_back("crgj15J"); mt2bins.push_back(1);
    regions.push_back("crgj1VL"); mt2bins.push_back(3);
    regions.push_back("crgj2VL"); mt2bins.push_back(3);
    regions.push_back("crgj3VL"); mt2bins.push_back(3);
    regions.push_back("crgj4VL"); mt2bins.push_back(3);
    regions.push_back("crgj5VL"); mt2bins.push_back(3);
    regions.push_back("crgj6VL"); mt2bins.push_back(3);
    regions.push_back("crgj7VL"); mt2bins.push_back(1);
    regions.push_back("crgj8VL"); mt2bins.push_back(1);
    regions.push_back("crgj9VL"); mt2bins.push_back(1);
    regions.push_back("crgj10VL"); mt2bins.push_back(2);
    regions.push_back("crgj11VL"); mt2bins.push_back(1);
    regions.push_back("crgj1L"); mt2bins.push_back(4);
    regions.push_back("crgj2L"); mt2bins.push_back(4);
    regions.push_back("crgj3L"); mt2bins.push_back(4);
    regions.push_back("crgj4L"); mt2bins.push_back(4);
    regions.push_back("crgj5L"); mt2bins.push_back(4);
    regions.push_back("crgj6L"); mt2bins.push_back(4);
    regions.push_back("crgj7L"); mt2bins.push_back(1);
    regions.push_back("crgj8L"); mt2bins.push_back(2);
    regions.push_back("crgj9L"); mt2bins.push_back(1);
    regions.push_back("crgj10L"); mt2bins.push_back(2);
    regions.push_back("crgj11L"); mt2bins.push_back(1);
    regions.push_back("crgj1M"); mt2bins.push_back(5);
    regions.push_back("crgj2M"); mt2bins.push_back(5);
    regions.push_back("crgj3M"); mt2bins.push_back(4);
    regions.push_back("crgj4M"); mt2bins.push_back(5);
    regions.push_back("crgj5M"); mt2bins.push_back(4);
    regions.push_back("crgj6M"); mt2bins.push_back(4);
    regions.push_back("crgj7M"); mt2bins.push_back(3);
    regions.push_back("crgj8M"); mt2bins.push_back(3);
    regions.push_back("crgj9M"); mt2bins.push_back(3);
    regions.push_back("crgj10M"); mt2bins.push_back(3);
    regions.push_back("crgj11M"); mt2bins.push_back(3);
    regions.push_back("crgj1H"); mt2bins.push_back(5);
    regions.push_back("crgj2H"); mt2bins.push_back(4);
    regions.push_back("crgj3H"); mt2bins.push_back(2);
    regions.push_back("crgj4H"); mt2bins.push_back(5);
    regions.push_back("crgj5H"); mt2bins.push_back(4);
    regions.push_back("crgj6H"); mt2bins.push_back(3);
    regions.push_back("crgj7H"); mt2bins.push_back(3);
    regions.push_back("crgj8H"); mt2bins.push_back(3);
    regions.push_back("crgj9H"); mt2bins.push_back(2);
    regions.push_back("crgj10H"); mt2bins.push_back(2);
    regions.push_back("crgj11H"); mt2bins.push_back(2);
    regions.push_back("crgj1UH"); mt2bins.push_back(5);
    regions.push_back("crgj2UH"); mt2bins.push_back(3);
    regions.push_back("crgj3UH"); mt2bins.push_back(1);
    regions.push_back("crgj4UH"); mt2bins.push_back(5);
    regions.push_back("crgj5UH"); mt2bins.push_back(3);
    regions.push_back("crgj6UH"); mt2bins.push_back(3);
    regions.push_back("crgj7UH"); mt2bins.push_back(2);
    regions.push_back("crgj8UH"); mt2bins.push_back(2);
    regions.push_back("crgj9UH"); mt2bins.push_back(2);
    regions.push_back("crgj10UH"); mt2bins.push_back(2);
    regions.push_back("crgj11UH"); mt2bins.push_back(2);
  }
  
  int totalmt2bins = 0;
  for(unsigned int i = 0; i < mt2bins.size(); i++) totalmt2bins += mt2bins[i];
  TH1D* h_sr_yields = new TH1D("h_sr_yields", "h_sr_yields", totalmt2bins, 0, totalmt2bins);
  int savedbins = 0;
  for(unsigned int j = 0; j < regions.size(); j++) {
    TString fullhistname = Form("%s/h_mt2bins", regions.at(j).Data());
    TH1D* h = (TH1D*) f1->Get(fullhistname);
    for (unsigned int k = 1; k <= mt2bins.at(j); k++) {
      savedbins++;
      double yield = 0.;
      double err = 0.;
      if (!h) yield = 0.0;
      else {
	yield = h->GetBinContent(k);
	err = h->GetBinError(k);
      }
      yield *= sf_for_lumi;
      err *= sf_for_lumi;
      h_sr_yields->SetBinContent(savedbins, yield);
      h_sr_yields->SetBinError(savedbins, err);
      //cout<<"i "<<i<<" j "<<j<<" k "<<k<<" yield "<<yield<<" err "<<err<<endl;
    }
    delete h;
  }

  // do same for variations
  for(unsigned int i = 0; i < vars.size(); ++i) {
    TH1D* h_sr_yields_var = new TH1D(Form("h_sr_yields_%s",vars.at(i).c_str()), Form("h_sr_yields_%s",vars.at(i).c_str()), totalmt2bins, 0, totalmt2bins);
    savedbins = 0;
    for(unsigned int j = 0; j < regions.size(); j++) {
      TString fullhistname = Form("%s/h_mt2bins_%s", regions.at(j).Data(),vars.at(i).c_str());
      TH1D* h = (TH1D*) f1->Get(fullhistname);
      for (unsigned int k = 1; k <= mt2bins.at(j); k++) {
	savedbins++;
	double yield = 0.;
	double err = 0.;
	if (!h) yield = 0.0;
	else {
	  yield = h->GetBinContent(k);
	  err = h->GetBinError(k);
	}
	yield *= sf_for_lumi;
	err *= sf_for_lumi;
	h_sr_yields_var->SetBinContent(savedbins, yield);
	h_sr_yields_var->SetBinError(savedbins, err);
	//cout<<"i "<<i<<" j "<<j<<" k "<<k<<" yield "<<yield<<" err "<<err<<endl;
      }
      delete h;
    }
    vars_hists.push_back(h_sr_yields_var);
  }

  std::vector<TString> labels;
  for(int i=0; i<regions.size(); i++){
    for (unsigned int k = 1; k <= mt2bins.at(i); k++) {
      TString label = getJetBJetPlotLabel(f1, regions.at(i).Data());
      label += ", ";
//      if (regions.at(i).Contains("J")) {
//        if (i < 7) label += i+1;
//        else label += i-7;
//      }
//      else label += k;
      label += k;
      labels.push_back(label);
    }
  }

  TCanvas* c1 = new TCanvas;
  c1->SetLogy();
  for(int i=0; i<labels.size(); i++){
    std::cout << labels.at(i) << std::endl;
    h_sr_yields->GetXaxis()->SetBinLabel(i+1, labels.at(i));
    for (unsigned int ivar=0; ivar<vars.size(); ++ivar) {
      vars_hists.at(ivar)->GetXaxis()->SetBinLabel(i+1, labels.at(i));
    }
  }

  std::vector<TH1D*> ratios;
  for(unsigned int i=0; i<vars.size(); i++){
    vars_hists.at(i)->Draw("same");
    TH1D* ratio = (TH1D*) vars_hists.at(i)->Clone(Form("h_ratio_%s",vars.at(i).c_str()));
    ratio->Divide(h_sr_yields);
    ratios.push_back(ratio);
  }
  

  TString outfilename = "yield_histograms_SigBinned"+HTregion+"_"+sample+".root";
  TFile* outfile = new TFile(outfilename, "RECREATE");
  outfile->cd();
  h_sr_yields->Write();
  for(int i=0; i<vars_hists.size(); i++){
    vars_hists.at(i)->Write();
    ratios.at(i)->Write();
  }
  outfile->Close();

}
