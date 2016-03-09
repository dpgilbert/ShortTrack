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



void plotZinvYieldsBinned(TString HTregion = "VL"){

  float sf_for_lumi = 1.0;///0.209;//to renormalize MC

  std::vector<TFile*> samples;
  std::vector<TH1F> sample_hists;
  std::vector<TString> regions;
  std::vector<int> mt2bins;

//  TFile* f1 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb/zinv_ht.root", "READ");
//  samples.push_back(f1);
//  TFile* f2 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb/purity.root", "READ");
  TFile* f1 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-10_25ns_data_json_246908-260627_v2_skim_base_V4_mt2gt200_2p26fb_newtxtfilters_jet1ptqcd/zinv_ht.root", "READ");
  samples.push_back(f1);
  TFile* f2 = new TFile("/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-10_25ns_data_json_246908-260627_v2_skim_base_V4_mt2gt200_2p26fb_newtxtfilters_jet1ptqcd/purity.root", "READ");
  samples.push_back(f2);
  samples.push_back(f2);

  //TFile* f1 = new TFile("/home/users/jgran/temp/RS_update/MT2Analysis/MT2looper/output/25ns_golden_246908-258159_25nsV5JEC_RebalanceAndSmear/data.root", "READ");
  //samples.push_back(f1);
  //TFile* f2 = new TFile("/nfs-7/userdata/jgran/Smearing_corrTemplates_25ns_golden_246908-258159_25nsV5JEC_RebalanceAndSmear_data_Run2015D_JetHT_PromptReco/RS_data.root", "READ");
  //samples.push_back(f2);

  bool doSR = false;


  doSR = true;

  
  if(HTregion == "J"){
//    regions.push_back("sr1J"); mt2bins.push_back(1);
//    regions.push_back("sr2J"); mt2bins.push_back(1);
//    regions.push_back("sr3J"); mt2bins.push_back(1);
//    regions.push_back("sr4J"); mt2bins.push_back(1);
//    regions.push_back("sr5J"); mt2bins.push_back(1);
//    regions.push_back("sr6J"); mt2bins.push_back(1);
//    regions.push_back("sr7J"); mt2bins.push_back(1);
//    regions.push_back("sr11J"); mt2bins.push_back(1);
//    regions.push_back("sr12J"); mt2bins.push_back(1);
//    regions.push_back("sr13J"); mt2bins.push_back(1);
//    regions.push_back("sr14J"); mt2bins.push_back(1);
//    regions.push_back("sr15J"); mt2bins.push_back(1);
//    regions.push_back("sr16J"); mt2bins.push_back(1);
    regions.push_back("srbaseJ0B"); mt2bins.push_back(7);
    regions.push_back("srbaseJ1B"); mt2bins.push_back(6);

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
    regions.push_back("sr10UH"); mt2bins.push_back(1);
    regions.push_back("sr11UH"); mt2bins.push_back(1);
  }
  
  int totalmt2bins = 0;
  for(unsigned int i = 0; i < mt2bins.size(); i++) totalmt2bins += mt2bins[i];

  for(unsigned int i = 0; i < samples.size(); i++) {
    TH1F* h_sr_yields = new TH1F("h_sr_yields", "h_sr_yields", totalmt2bins, 0, totalmt2bins);
    int savedbins = 0;
    for(unsigned int j = 0; j < regions.size(); j++) {
      TString fullhistname = Form("%s/h_mt2bins", regions.at(j).Data());
// With uncertainties on R(Z/Gamma)
//      if (i==1) fullhistname = Form("%s/h_mt2binspredZFailSieieDatawithRuncBin", regions.at(j).Data());
//      if (i==2) fullhistname = Form("%s/h_mt2binspredZFailSieieDatawithRuncInt", regions.at(j).Data());
// Without uncertainties on R(Z/Gamma). Only includes P and   F (10%)
      if (i==1) fullhistname = Form("%s/h_mt2binspredZFailSieieData", regions.at(j).Data());
      if (i==2) fullhistname = Form("%s/h_mt2binspredZFailSieieData", regions.at(j).Data());
      samples.at(i)->cd();
      TH1F* h = (TH1F*) samples.at(i)->Get(fullhistname);
      // for BinByBin estimate, assign binomial uncertainty to all bins with respect to total
      // ignore the initial uncertainty (on f, P, stats)
      float integral = 0;
      if (i==1 && h) {
        double err = 0;
        integral = h->IntegralAndError(0, -1, err);
        // ok, now each bin needs to get the binomial error
        for (unsigned int k = 1; k <= h->GetNbinsX(); k++) {
          float cont = h->GetBinContent(k);
          float p;
          if (integral>0) p = cont / integral;
          else p = 0.;
          float err = TMath::Sqrt( integral*p*(1-p) );
          h->SetBinError( k, err );
        }
      }
      // for integrated estimate, just rescale Zinv MC to the prediction in each bin ;-)
      // but add a 0-40% uncertainty on all bins
      if (i==2 && h) {
        double err = 0;
        integral = h->IntegralAndError(0, -1, err);
        float percenterr = err*1./integral;
        fullhistname = Form("%s/h_mt2bins", regions.at(j).Data());
        h = (TH1F*) samples.at(0)->Get(fullhistname);
        if (h) h->Scale(1.*integral/h->Integral());
        // ok, now each bin needs to get the shape uncertainty
        // first bin needs to recover the normalization if all other bins go high
        // so first bin uncertainty  = sum (all other uncertainties)
        int nbins = h->GetNbinsX();
        float firstBinUnc = 0;
        for (unsigned int k = 1; k <= h->GetNbinsX(); k++) {
          float cont = h->GetBinContent(k);
          float percenterr = 0.4 / (nbins-1) * (k-1);
          if (k==1) h->SetBinError( k, 0. );
          else {
            h->SetBinError( k, percenterr*cont );
            firstBinUnc += percenterr*cont;
          }
        }
        if (firstBinUnc > h->GetBinContent(1)) firstBinUnc = h->GetBinContent(1); // max to 100%
        h->SetBinError( 1,  firstBinUnc);
      }
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
    sample_hists.push_back(*h_sr_yields);
    delete h_sr_yields;
  }
  //sample_hists.at(0).Print("all");
//  //use mc hist from Francesco
//  TFile* francesco_file = new TFile("qcd_histo.root", "READ");
//  TH1F* hfrancesco = (TH1F*) francesco_file->Get("h_mcTruth_tot");
//  sample_hists.at(0) = *hfrancesco;

  std::vector<TString> labels;
  for(int i=0; i<regions.size(); i++){
    for (unsigned int k = 1; k <= mt2bins.at(i); k++) {
      TString label = getJetBJetPlotLabel(samples.at(0), regions.at(i).Data());
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
    sample_hists.at(0).GetXaxis()->SetBinLabel(i+1, labels.at(i));
    sample_hists.at(1).GetXaxis()->SetBinLabel(i+1, labels.at(i));
    sample_hists.at(2).GetXaxis()->SetBinLabel(i+1, labels.at(i));
  }
  sample_hists.at(0).Draw();
  sample_hists.at(0).SetLineColor(kRed);
  for(unsigned int i=1; i<sample_hists.size(); i++){
    sample_hists.at(i).Draw("same");
  }
  //c1->Print("extremeHT.pdf");

  //hPull is filled with a SetBinContent (iBin, (estimate-MC)/sqrt(sigma(estimate)^2+sigma(MC)^2)), and a SetBinError(iBin, 1.0)
  TH1F* h_pull = new TH1F("h_pull", "h_pull", totalmt2bins, 0, totalmt2bins);
  TH1F* h_ratio = new TH1F("h_ratio", "h_ratio", totalmt2bins, 0, totalmt2bins);
  TH1F* h_ratioMC   = new TH1F("h_ratioMC",   "h_ratioMC",   totalmt2bins, 0, totalmt2bins);
  TH1F* h_ratioData = new TH1F("h_ratioData", "h_ratioData", totalmt2bins, 0, totalmt2bins);
//  for(int i=0; i<regions.size(); i++){
  for(int i=0; i<totalmt2bins; i++){
    float mc = sample_hists.at(2).GetBinContent(i+1);
    float mc_err = sample_hists.at(2).GetBinError(i+1);
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
      h_ratioMC->SetBinContent(i+1, 1);
      h_ratioMC->SetBinError(i+1, mc_err / mc );
      h_ratioData->SetBinContent(i+1, rs/mc);
      h_ratioData->SetBinError(i+1, rs_err / rs );
    }
  }
  //h_ratioData->Print("all");
  //h_ratioMC->Print("all");
  

  TFile* outfile = 0;
  TString outfilename = "yield_histograms_ZinvBinned"+HTregion+".root";
  if(doSR) outfile = new TFile(outfilename, "RECREATE");
  else outfile = new TFile("yield_histograms.root", "RECREATE");
  outfile->cd();
  for(int i=0; i<sample_hists.size(); i++){
    if(i==0) sample_hists.at(i).Write("yields_MC");
    if(i==1) sample_hists.at(i).Write("yields_Binned");
    if(i==2) sample_hists.at(i).Write("yields_Integrated");
  }
  h_pull->Write();
  h_ratio->Write();
  h_ratioData->Write();
  h_ratioMC->Write();
  outfile->Close();

}
