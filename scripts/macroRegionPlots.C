std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}


// return h_mt2 histogram made by summing regions defined in input
TH1F* sumMT2histograms(TString filein, std::vector<TString> regions){
  
  TFile* f1 = new TFile(filein, "READ");
  if(f1->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return 0;
  }
  TH1F* model = (TH1F*) f1->Get("sr1VL/h_mt2");
  if (!model) {
    std::cerr << "Histogram sr1VL/h_mt2 does not exist" << std::endl;
    return 0;
  }
  
  TH1F* output = (TH1F*) model->Clone("h_mt2");
  output->SetDirectory(0);
  output->Reset();
  for(unsigned int j = 0; j < regions.size(); j++) {
    TString fullhistname = Form("%s/h_mt2", regions.at(j).Data());
    TH1F* h = (TH1F*) f1->Get(fullhistname);
    if (h) output->Add(h);
    else cout<<"Looking at file "<<filein<<". Could not find h_mt2 histogram in "<<regions.at(j)<<endl;
  }
  
  return output;
  
}

// return h_mt2 histogram made by summing regions defined in input
TH1F* sumMT2histogramsAndRebin(TString filein, std::vector<TString> regions, std::vector<float> bins){

  TFile* f1 = new TFile(filein, "READ");
  if(f1->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return 0;
  }
  TH1F* model = (TH1F*) f1->Get("sr1VL/h_mt2");
  if (!model) {
    std::cerr << "Histogram sr1VL/h_mt2 does not exist" << std::endl;
    return 0;
  }
  
  TH1F* output = (TH1F*) model->Clone("h_mt2");
  output->SetDirectory(0);
  output->Reset();
  for(unsigned int j = 0; j < regions.size(); j++) {
      TString fullhistname = Form("%s/h_mt2", regions.at(j).Data());
      TH1F* h = (TH1F*) f1->Get(fullhistname);
      if (h) output->Add(h);
      else cout<<"Looking at file "<<filein<<". Could not find h_mt2 histogram in "<<regions.at(j)<<endl;
  }
  
  // Now rebin:
  const int nbins = bins.size()-1;
  float binsTemp[nbins+1];
  for (unsigned int i = 0; i < bins.size(); i++) binsTemp[i] = bins[i];
  //const float constbins[nbins+1] = binsTemp;
  TH1F* rebinned = new TH1F("h_mt2rebin","h_mt2rebin", nbins, binsTemp);
  rebinned->SetDirectory(0);
  //cout<<"Input integral is "<<output->Integral(0, -1)<<endl;
  for (unsigned int i = 0; i < nbins; i++) {
    int firstBin = output->FindBin(bins[i]);
    int lastBin = output->FindBin(bins[i+1]);
    double error;
    double integral = output->IntegralAndError(firstBin, lastBin-1, error);
    if (i == nbins-1) integral = output->IntegralAndError(firstBin, -1, error); // overflow
    //cout<<"Looking at bin "<<i<<". Integral between "<<bins[i]<<" and "<<bins[i+1]<<" is "<<integral<<" +/- "<<error<<endl;
    rebinned->SetBinContent(i+1, integral);
    rebinned->SetBinError(i+1, error);
  }
  //cout<<"Output integral is "<<rebinned->Integral(0, -1)<<endl;

  
  return rebinned;
    
}

void fillFileWithPlots(TString filein, TString fileout, std::vector<std::vector<TString>> regionsSet, std::vector<TString> namesSet, std::vector< std::vector<float>> binsSet) {
  
  TFile* outfile = new TFile(fileout, "RECREATE");
  outfile->cd();
  
  for (int i = 0; i < regionsSet.size(); i++) {
    
//    for (int j = 0; j < binsSet.at(i).size(); j++) {
//      
//      cout<<binsSet.at(i)[j]<<endl;
//      
//    }
    TH1F * sum = sumMT2histograms(filein, regionsSet.at(i));
    TH1F * sumAndRebin = sumMT2histogramsAndRebin(filein, regionsSet.at(i), binsSet.at(i));
    TDirectory * dir = outfile->mkdir(namesSet.at(i));
    dir->cd();
    sum->Write();
    sumAndRebin->Write();
  }
  
}

void macroRegionPlots(TString dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-10_25ns_data_json_246908-260627_v2_skim_base_V4_mt2gt200_2p26fb_newtxtfilters_jet1ptqcd/") {
  

  
  //TFile* f_zinv = new TFile(dir+"/zinv_ht.root", "READ");
  
  std::vector<std::vector<TString>> regionsSet;
  std::vector<TString> namesSet;
  //std::vector< int> nbinsSet;
  std::vector< std::vector< float> > binsSet;

//  std::vector<TString> r0;
//  r0.push_back("sr1VL");
//  r0.push_back("sr1L");
//  r0.push_back("sr1M");
//  regionsSet.push_back(r0);
//  namesSet.push_back("srMacroHT1NJ1NB1orig");
  
  
//  std::vector<TString> r9; // HT all, NJ [1], NB [0]
//  r9.push_back("sr1J");
//  r9.push_back("sr2J");
//  r9.push_back("sr3J");
//  r9.push_back("sr4J");
//  r9.push_back("sr5J");
//  r9.push_back("sr6J");
//  r9.push_back("sr7J");
//  regionsSet.push_back(r9);
//  namesSet.push_back("srMacroHT0NJ0NB1");
//  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});
//  
//  
//  std::vector<TString> r10; // HT all, NJ [1], NB [0]
//  r10.push_back("sr11J");
//  r10.push_back("sr12J");
//  r10.push_back("sr13J");
//  r10.push_back("sr14J");
//  r10.push_back("sr15J");
//  regionsSet.push_back(r10);
//  namesSet.push_back("srMacroHT0NJ0NB2");
//  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});

  
  std::vector<TString> r1; // HT [200,1000], NJ [2, 3], NB [0]
  r1.push_back("sr1VL");
  r1.push_back("sr1L");
  r1.push_back("sr1M");
  regionsSet.push_back(r1);
  namesSet.push_back("srMacroHT1NJ1NB1");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});

  std::vector<TString> r2; // HT [200,1000], NJ [2, 3], NB [1, inf]
  r2.push_back("sr2VL");
  r2.push_back("sr2L");
  r2.push_back("sr2M");
  r2.push_back("sr3VL");
  r2.push_back("sr3L");
  r2.push_back("sr3M");
  regionsSet.push_back(r2);
  namesSet.push_back("srMacroHT1NJ1NB2");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});

  std::vector<TString> r3; // HT [200,1000], NJ [4, inf], NB [0]
  r3.push_back("sr4VL");
  r3.push_back("sr4L");
  r3.push_back("sr4M");
  r3.push_back("sr7VL");
  r3.push_back("sr7L");
  r3.push_back("sr7M");
  regionsSet.push_back(r3);
  namesSet.push_back("srMacroHT1NJ2NB1");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});

  std::vector<TString> r4; // HT [200,1000], NJ [4, inf], NB [1, inf]
  r4.push_back("sr5VL");
  r4.push_back("sr5L");
  r4.push_back("sr5M");
  r4.push_back("sr6VL");
  r4.push_back("sr6L");
  r4.push_back("sr6M");
  r4.push_back("sr8VL");
  r4.push_back("sr8L");
  r4.push_back("sr8M");
  r4.push_back("sr9VL");
  r4.push_back("sr9L");
  r4.push_back("sr9M");
//  r4.push_back("sr10VL");
//  r4.push_back("sr10L");
//  r4.push_back("sr10M"); // leaving out region 10: 2-6 J, >3 B, since it does not quite belong
//  r4.push_back("sr11VL");
//  r4.push_back("sr11L");
//  r4.push_back("sr11M"); // leaving out region 10: 2-6 J, >3 B, since it does not quite belong
  regionsSet.push_back(r4);
  namesSet.push_back("srMacroHT1NJ2NB2");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 1000.});

  std::vector<TString> r5; // HT [1000, inf], NJ [2, 3], NB [0]
  r5.push_back("sr1H");
  r5.push_back("sr1UH");
  regionsSet.push_back(r5);
  namesSet.push_back("srMacroHT2NJ1NB1");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000., 1500.});

  std::vector<TString> r6; // HT [1000, inf], NJ [2, 3], NB [1, inf]
  r6.push_back("sr2H");
  r6.push_back("sr2UH");
  r6.push_back("sr3H");
  r6.push_back("sr3UH");
  regionsSet.push_back(r6);
  namesSet.push_back("srMacroHT2NJ1NB2");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});

  std::vector<TString> r7; // HT [1000, inf], NJ [4, inf], NB [0]
  r7.push_back("sr4H");
  r7.push_back("sr4UH");
  r7.push_back("sr7H");
  r7.push_back("sr7UH");
  regionsSet.push_back(r7);
  namesSet.push_back("srMacroHT2NJ2NB1");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000., 1500.});

  std::vector<TString> r8; // HT [1000, inf], NJ [4, inf], NB [1, inf]
  r8.push_back("sr5H");
  r8.push_back("sr5UH");
  r8.push_back("sr6H");
  r8.push_back("sr6UH");
  r8.push_back("sr8H");
  r8.push_back("sr8UH");
  r8.push_back("sr9H");
  r8.push_back("sr9UH");
//  r8.push_back("sr10H");
//  r8.push_back("sr10UH"); // leaving out region 10: 2-6 J, >3 B, since it does not quite belong
//  r8.push_back("sr11H");
//  r8.push_back("sr11UH"); // leaving out region 10: 7 J, >3 B, since it does not quite belong
  regionsSet.push_back(r8);
  namesSet.push_back("srMacroHT2NJ2NB2");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 800., 1000.});
  
  std::vector<TString> r9; // HT [1000, inf], NJ [4, inf], NB [1, inf]
  r9.push_back("sr10VL");
  r9.push_back("sr10L");
  r9.push_back("sr10M");
  r9.push_back("sr10H");
  r9.push_back("sr10UH");
  r9.push_back("sr11VL");
  r9.push_back("sr11L");
  r9.push_back("sr11M");
  r9.push_back("sr11H");
  r9.push_back("sr11UH");
  regionsSet.push_back(r9);
  namesSet.push_back("srMacroHT1NJ1NB3");
  binsSet.push_back((std::vector<float>) {200, 300., 400., 600., 1000.});



  fillFileWithPlots(dir+"/data_Run2015CD.root", dir+"/MacroData.root", regionsSet, namesSet, binsSet);
  fillFileWithPlots(dir+"/purity.root",         dir+"/MacroZinv.root", regionsSet, namesSet, binsSet);
  fillFileWithPlots(dir+"/lostlepFromCRs.root", dir+"/MacroLL.root", regionsSet, namesSet, binsSet);
  fillFileWithPlots(dir+"/qcd_ht.root",         dir+"/MacroQCD.root", regionsSet, namesSet, binsSet);
  
  
 // TH1F * LLmacro1 = sumMT2histograms(dir+"/lostlepFromCRs.root", regions);
//  TH1F * QCDmacro1 = sumMT2histograms(dir+"/qcd_ht.root", regions);

}

