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
      else cout<<"could not find h_mt2 histogram in "<<regions.at(j)<<endl;
  }
  
  return output;
    
}

void fillFileWithPlots(TString filein, TString fileout, std::vector<std::vector<TString>> regionsSet, std::vector<TString> namesSet) {
  
  TFile* outfile = new TFile(fileout, "RECREATE");
  outfile->cd();
  
  for (int i = 0; i < regionsSet.size(); i++) {
    TH1F * sum = sumMT2histograms(filein, regionsSet.at(i));
    TDirectory * dir = outfile->mkdir(namesSet.at(i));
    dir->cd();
    sum->Write();
  }
  
}

void macroRegionPlots(TString dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-09_25ns_skim_base_mt2gt200_ZinvV3") {
  

  
  //TFile* f_zinv = new TFile(dir+"/zinv_ht.root", "READ");
  
  std::vector<std::vector<TString>> regionsSet;
  std::vector<TString> namesSet;

  std::vector<TString> r1;
  r1.push_back("sr1VL");
  r1.push_back("sr1L");
  r1.push_back("sr1M");
  regionsSet.push_back(r1);
  namesSet.push_back("srMacro1");
  
  std::vector<TString> r2;
  r2.push_back("sr2VL");
  r2.push_back("sr2L");
  r2.push_back("sr2M");
  regionsSet.push_back(r2);
  namesSet.push_back("srMacro2");

  fillFileWithPlots(dir+"/data_Run2015CD.root", dir+"/MacroData.root", regionsSet, namesSet);
  fillFileWithPlots(dir+"/purity.root",         dir+"/MacroZinv.root", regionsSet, namesSet);
  fillFileWithPlots(dir+"/lostlepFromCRs.root", dir+"/MacroLL.root", regionsSet, namesSet);
  fillFileWithPlots(dir+"/qcd_ht.root",         dir+"/MacroQCD.root", regionsSet, namesSet);
  
  
 // TH1F * LLmacro1 = sumMT2histograms(dir+"/lostlepFromCRs.root", regions);
//  TH1F * QCDmacro1 = sumMT2histograms(dir+"/qcd_ht.root", regions);

}

