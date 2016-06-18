// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"

// Tools
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"

// header
#include "QCDLooper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;
using namespace mt2;
using namespace duplicate_removal;

class mt2tree;
class SR;

// turn on to apply json file to data
bool applyJSON = true;

//_______________________________________
QCDLooper::QCDLooper(){
}
//_______________________________________
QCDLooper::~QCDLooper(){

};

//_______________________________________
void QCDLooper::SetSignalRegions(){

    string HTnames[5] = {"ht200to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf"};
    double HTcuts[6] = {200,450,575,1000,1500,-1};
    for(unsigned int i=0; i<5; i++){
        SR sr;
        sr.SetName("rphi_"+HTnames[i]);
        sr.SetVar("ht",HTcuts[i],HTcuts[i+1]);
        sr.SetVar("njets",2,-1);
        sr.SetVar("mt2",50,-1);
        sr.SetVar("deltaPhiMin",0,-1);
        SRVec_rphi.push_back(sr);
        outfile_->mkdir(sr.GetName().c_str());

        sr.SetName("fj_"+HTnames[i]);
        sr.SetVar("ht",HTcuts[i],HTcuts[i+1]);
        sr.SetVar("njets",2,-1);
        sr.SetVar("mt2",100,200);
        sr.SetVar("deltaPhiMin",0,0.3);
        SRVec_fj.push_back(sr);
        outfile_->mkdir(sr.GetName().c_str());
    }

    string NJnames[3] = {"j2to3","j4to6","j7toInf"};
    int NJcuts[4] = {2,4,7,-1};
    for(unsigned int i=0; i<3; i++){
        SR sr;
        sr.SetName("rb_"+NJnames[i]);
        sr.SetVar("ht",200,-1);
        sr.SetVar("njets",NJcuts[i],NJcuts[i+1]);
        sr.SetVar("mt2",100,200);
        sr.SetVar("deltaPhiMin",0,0.3);
        SRVec_rb.push_back(sr);
        outfile_->mkdir(sr.GetName().c_str());
    }

}


//_______________________________________
void QCDLooper::loop(TChain* chain, std::string output_name){

  gROOT->cd();
  cout << "[QCDLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  const char* json_file = "../babymaker/jsons/Cert_271036-274421_13TeV_PromptReco_Collisions16_JSON_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  cout << "[QCDLooper::loop] setting up histos" << endl;

  SetSignalRegions();

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[QCDLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[QCDLooper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    
    // Use this to speed things up when not looking at genParticles
    //tree->SetBranchStatus("genPart_*", 0); 

    t.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      
      t.GetEntry(event);

      //---------------------
      // bookkeeping and progress report
      //---------------------
      ++nEventsTotal;
      if (nEventsTotal%10000==0) {
	ULong64_t i_permille = (int)floor(1000 * nEventsTotal / float(nEventsChain));
	if (isatty(1)) {
	  printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
		 "\033[0m\033[32m <---\033[0m\015", i_permille/10.);
	  fflush(stdout);
	}else {
            cout<<i_permille/10.<<" ";
            if (nEventsTotal%100000==0) cout<<endl;
        }
      }

      //---------------------
      // skip duplicates -- needed when running on mutiple streams in data
      //---------------------
      if( t.isData ) {
	DorkyEventIdentifier id(t.run, t.evt, t.lumi);
	if (is_duplicate(id) ){
	  ++nDuplicates;
	  continue;
	}
      }

      //---------------------
      // basic event selection and cleaning
      //---------------------

      if( applyJSON && t.isData && !goodrun(t.run, t.lumi) ) continue;

      // MET filters (data only)
      if (t.isData) {
	if (!t.Flag_goodVertices) continue;
	if (!t.Flag_CSCTightHaloFilter) continue;
	if (!t.Flag_eeBadScFilter) continue;
	if (!t.Flag_HBHENoiseFilter) continue;
      }
      if (!t.Flag_badChargedCandidateFilter) continue;
      

      // some simple baseline selections
      if (t.nVert == 0) continue;
      int nlepveto = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;
      if(nlepveto!=0) continue;

      if(t.met_pt < 30) continue;
      if(t.diffMetMht/t.met_pt > 0.5) continue;
      if(t.nJet30FailId!=0) continue;
      if(t.nJet30 < 2) continue;

      //-------------------
      // "spike rejection
      //-------------------
      int id = t.evt_id;
      bool pass_spike = (id>150&&(id>151||t.ht<450)&&(id>152||t.ht<575)&&(id>153||t.ht<575)&&(id>154||t.ht<1000))||id>=200||id<100;
      if(!pass_spike) continue;


      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 2.070;
      evtweight_ = 1.;

      // apply relevant weights to MC
      if (!t.isData) {
	evtweight_ = t.evt_scale1fb * lumi;
      } // !isData

      //---------------------------
      // fill histograms
      //---------------------------
      std::map<std::string, float> values;
      values["njets"]        = t.nJet30;
      values["ht"]         = t.ht;
      values["mt2"] = t.mt2;
      values["deltaPhiMin"] = t.deltaPhiMin;
      for(unsigned int i=0; i<SRVec_rphi.size(); i++){
          if(SRVec_rphi.at(i).PassesSelection(values))
              fillHistosRphi(SRVec_rphi.at(i).srHistMap,SRVec_rphi.at(i).GetName().c_str());
          if(SRVec_fj.at(i).PassesSelection(values))
              fillHistosFj(SRVec_fj.at(i).srHistMap,SRVec_fj.at(i).GetName().c_str());
      }
      for(unsigned int i=0; i<SRVec_rb.size(); i++){
          if(SRVec_rb.at(i).PassesSelection(values))
              fillHistosRb(SRVec_rb.at(i).srHistMap,SRVec_rb.at(i).GetName().c_str());
      }

    }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[QCDLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  for(unsigned int i=0; i<SRVec_rphi.size(); i++){
      savePlotsDir(SRVec_rphi.at(i).srHistMap,outfile_,SRVec_rphi.at(i).GetName().c_str());
      savePlotsDir(SRVec_fj.at(i).srHistMap,outfile_,SRVec_fj.at(i).GetName().c_str());
  }
  for(unsigned int i=0; i<SRVec_rb.size(); i++){
      savePlotsDir(SRVec_rb.at(i).srHistMap,outfile_,SRVec_rb.at(i).GetName().c_str());
  }

  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;

  return;
}

//_______________________________________
void QCDLooper::fillHistosRphi(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s) {

    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
        dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
    
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    if(t.deltaPhiMin>=0.3){
        plot1D("h_mt2_num"+s,        t.mt2,   evtweight_, h_1d, ";M_{T2} [GeV]",300,0,1500);
    }else{
        plot1D("h_mt2_den"+s,        t.mt2,   evtweight_, h_1d, ";M_{T2} [GeV]",300,0,1500);
    }
    
    outfile_->cd();
    return;
}

//_______________________________________
void QCDLooper::fillHistosFj(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s) {

    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
        dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
    
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_njets"+s,        t.nJet30,   evtweight_, h_1d, "N(jet)",9,2,11);
    
    outfile_->cd();
    return;
}
//_______________________________________
void QCDLooper::fillHistosRb(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s) {

    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
        dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
    
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_nbjets"+s,        t.nBJet20,   evtweight_, h_1d, "N(jet)",6,0,6);
    
    outfile_->cd();
    return;
}
