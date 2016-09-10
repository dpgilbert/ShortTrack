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
#include "TMath.h"
#include "TMinuit.h"
#include "TList.h"
#include "TCollection.h"
#include "TKey.h"
#include "TF1.h"
#include "TStyle.h"
#include "Math/GSLIntegrator.h"
#include "Math/WrappedTF1.h"

#include "../../CORE/Tools/utils.h"

// header
#include "RebalanceLooper.h"

//MT2
#include "../../MT2CORE/Plotting/PlotUtilities.h"

using namespace std;

class mt2tree;

JRTreader reader;
bool isData = false;
std::vector<float> jetpt;
std::vector<float> jeteta;
std::vector<float> jetphi;
std::vector<float> jetbtagcsv;
std::map<std::string, std::vector<double> > responseMap;
std::map<std::string, std::pair<TF1, float> > fitMap;
//float sigma_soft;
float pt_soft_x;
float pt_soft_y;

float GetValue(float xvalue, TH1F &hist){
    float xmax = hist.GetXaxis()->GetXmax()-0.01;
    int xbin   = hist.GetXaxis()->FindBin(min(xvalue, xmax));
    return hist.GetBinContent(xbin);
}

double ComputeGaussianValue(double scale, double mean, double sigma, float x){
    return scale*TMath::Exp(-0.5*((x - mean)/sigma)*((x - mean)/sigma));
}

float GetProb(float pt, float eta, bool isBjet, double par){
    double my_prob = reader.GetValue(pt,eta,isBjet,par);
    return my_prob;
}

void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg) {
    float likelihood = 0;
    float pt_constrained_x = 0.0;
    float pt_constrained_y = 0.0;
    float min_prob = 1E-20;
    for(unsigned int i=0; i<jetpt.size(); i++){
        bool isBjet = (jetbtagcsv.at(i) > 0.800);
        // bool isBjet = false;
        float prob = GetProb((jetpt.at(i))/par[i], fabs(jeteta.at(i)), isBjet, par[i]);
        prob = max(prob, min_prob);
        likelihood += log(prob);
        pt_constrained_x -= (jetpt.at(i))*cos(jetphi.at(i))/par[i];
        pt_constrained_y -= (jetpt.at(i))*sin(jetphi.at(i))/par[i];
    }
    //float x1 = (pt_soft_x - pt_constrained_x)/sigma_soft;
    //float x2 = (pt_soft_y - pt_constrained_y)/sigma_soft;
    float x1 = (pt_soft_x - pt_constrained_x)/20.0;
    float x2 = (pt_soft_y - pt_constrained_y)/20.0;
    likelihood += -x1*x1/2;
    likelihood += -x2*x2/2;

    result = -likelihood;
}

RebalanceLooper::RebalanceLooper(){};
RebalanceLooper::~RebalanceLooper(){};

void RebalanceLooper::loop(TChain* chain, std::string output_name){
  
    reader.Init("JetResponseTemplates.root");

    // Benchmark
    TBenchmark *bmark = new TBenchmark();
    bmark->Start("benchmark");

    MakeNtuple( Form("%s", output_name.c_str()) );

    // File Loop
    int nDuplicates = 0;
    int nEvents = chain->GetEntries();
    unsigned int nEventsChain = nEvents;
    cout << "[RebalanceLooper::loop] running on " << nEventsChain << " events" << endl;
    unsigned int nEventsTotal = 0;
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        cout << "[RebalanceLooper::loop] running on file: " << currentFile->GetTitle() << endl;

        // Get File Content
        TFile f( currentFile->GetTitle() );
        TTree *tree = (TTree*)f.Get("mt2");
        TTreeCache::SetLearnEntries(10);
        tree->SetCacheSize(128*1024*1024);

        t.Init(tree);

        // Event Loop
        unsigned int nEventsTree = tree->GetEntriesFast();
        unsigned int maxEvents = nEventsTree;
        // unsigned int maxEvents = 5000;
        for( unsigned int event = 0; event < maxEvents; ++event) {

            if(event%20 !=0 )
                continue;

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
                }
                else {
                    cout<<i_permille/10.<<" ";
                    if (nEventsTotal%100000==0) cout<<endl;
                }
            }

            InitNtuple();

            if(t.njet < 2){
                status = -1;
                FillNtuple(); 
                continue;
            }
    

            if(isData){
                // bool pass_trigger = false;
                // if(t.HLT_PFHT800 > 0 && t.ht > 900.0) pass_trigger = true;
                // else if(t.HLT_PFHT600_Prescale > 0 && t.ht > 700.0) pass_trigger = true;
                // else if(t.HLT_PFHT475_Prescale > 0 && t.ht > 575.0) pass_trigger = true;
                // else if(t.HLT_PFHT350_Prescale > 0 && t.ht > 450.0) pass_trigger = true;
                // else if(t.HLT_PFHT300_Prescale > 0 && t.ht > 400.0) pass_trigger = true;
                // else if(t.HLT_PFHT200_Prescale > 0 && t.ht > 300.0) pass_trigger = true;

                // if(!pass_trigger) {
                //     status = -1;
                //     FillNtuple(); 
                //     continue;
                // }

                // prescale = 9999999;
                // if(t.HLT_PFHT600_Prescale > 0 && t.ht > 700.0) prescale = min(t.HLT_PFHT600_Prescale, prescale);
                // if(t.HLT_PFHT475_Prescale > 0 && t.ht > 575.0) prescale = min(t.HLT_PFHT475_Prescale, prescale);
                // if(t.HLT_PFHT350_Prescale > 0 && t.ht > 450.0) prescale = min(t.HLT_PFHT350_Prescale, prescale);
                // if(t.HLT_PFHT300_Prescale > 0 && t.ht > 400.0) prescale = min(t.HLT_PFHT300_Prescale, prescale);
                // if(t.HLT_PFHT200_Prescale > 0 && t.ht > 300.0) prescale = min(t.HLT_PFHT200_Prescale, prescale);
                // if(t.HLT_PFHT800 > 0 && t.ht > 900.0) prescale = 1;

                // if(prescale < 0){
                //     std::cout << "WARNING: prescale_lt_zero" << std::endl;
                //     std::cout << t.HLT_PFHT600_Prescale << std::endl;
                //     std::cout << t.HLT_PFHT475_Prescale << std::endl;
                //     std::cout << t.HLT_PFHT350_Prescale << std::endl;
                //     std::cout << t.HLT_PFHT300_Prescale << std::endl;
                //     std::cout << t.HLT_PFHT200_Prescale << std::endl;
                // }
            }
            else prescale = 1;


            //---------------------
            // skip duplicates -- will need this eventually
            //---------------------
            //if( isData ) {
            //  DorkyEventIdentifier id = {stopt.run(), stopt.event(), stopt.lumi() };
            //  if (is_duplicate(id, already_seen) ){
            //    continue;
            //  }
            //}
            //

            ////////////////////////// rebalance //////////////////////////

            jetpt.clear();
            jeteta.clear();
            jetphi.clear();
            // jetbtagcsv.clear();

            float met_x = (t.met_pt)*cos(t.met_phi);
            float met_y = (t.met_pt)*sin(t.met_phi);

            float jet_x = 0;
            float jet_y = 0;
            for(int i=0; i<t.njet; i++){                
                if(t.jet_pt[i] < 10.0){
                    useJet.push_back(0);
                  continue;
                }

                if(t.jet_pt[i] < 100.0 && t.jet_puId[i] < 1){
                    useJet.push_back(0);
                    met_x += (t.jet_pt[i])*cos(t.jet_phi[i]);//FIXME
                    met_y += (t.jet_pt[i])*sin(t.jet_phi[i]);//FIXME
                    continue;
                }

                jet_x += (t.jet_pt[i])*cos(t.jet_phi[i]);
                jet_y += (t.jet_pt[i])*sin(t.jet_phi[i]);
                useJet.push_back(1);
                //float sf = GetValue(t.jet_pt[i], *h_sf);
                //float corr_pt = (t.jet_pt[i])/sf;
                //jetpt.push_back(corr_pt);
                jetpt.push_back(t.jet_pt[i]);
                jeteta.push_back(t.jet_eta[i]);
                jetphi.push_back(t.jet_phi[i]);
                jetbtagcsv.push_back(t.jet_btagCSV[i]);
            }

            if(jetpt.size() < 2){
                for(unsigned int i=0; i<jetpt.size(); i++){
                    rebalanceFactors.push_back(1.0);
                }
                status = -1;
                FillNtuple(); 
                continue;
            }

            pt_soft_x = -met_x - jet_x;
            pt_soft_y = -met_y - jet_y;
            // float pt_soft = sqrt(pt_soft_x*pt_soft_x + pt_soft_y*pt_soft_y);
            //sigma_soft = 20.0;
            //sigma_soft = 18.0;
            //sigma_soft = 1.5*t.nVert;
            //sigma_soft = 5.0*sqrt(t.nVert);

            TMinuit* minimizer = new TMinuit(jetpt.size());
            minimizer->SetFCN(minuitFunction);
            int iflag = 0;
            Double_t arglist[10];

            // suppress warnings so the log files aren't multiple GB
            minimizer->SetPrintLevel(-1);
            minimizer->mnexcm("SET NOWarnings",0,0,iflag);

            arglist[0] = -1;
            minimizer->mnexcm("SET PRI", arglist, 1, iflag);

            arglist[0] = 1;
            minimizer->mnexcm("SET STRATEGY", arglist, 1, iflag);

            minimizer->SetErrorDef(0.5);

            for(unsigned int i=0; i<jetpt.size(); i++){
                std::string name = Form("c%d", i);
                minimizer->mnparm(i,name,1.0,0.05,0.2,5,iflag);
            }

            arglist[0] = 10000;
            arglist[1] = 1.0;

            minimizer->mnexcm("MIGRAD", arglist, 2, iflag);
            std::cout << "MIGRAD iflag = " << iflag << std::endl;
            status = iflag;
      
            if(iflag !=0){
                arglist[1] = 10.0;//easier threshold for convergence
                minimizer->mnexcm("MIGRAD", arglist, 2, iflag);
                std::cout << "second MIGRAD iflag = " << iflag << std::endl;
                status = iflag;
            }

            arglist[0] = 5000;
            arglist[1] = 0;
            arglist[2] = 1;

            if(iflag !=0){
                minimizer->mnexcm("MINOS", arglist, 3, iflag);
                status = iflag;
            }
         
            for(unsigned int i=0; i<jetpt.size(); i++){
                double par_value;
                double par_error;
                minimizer->GetParameter(i, par_value, par_error);
                par_value = 1.0/par_value;
                jetpt.at(i) *= par_value;
                rebalanceFactors.push_back(par_value); 
            }

            delete minimizer;

            jet_x = 0;
            jet_y = 0;
            for(unsigned int i=0; i<jetpt.size(); i++){
                jet_x += (jetpt.at(i))*cos(jetphi.at(i));
                jet_y += (jetpt.at(i))*sin(jetphi.at(i));
            }
            float new_met_x = -pt_soft_x - jet_x;
            float new_met_y = -pt_soft_y - jet_y;
            new_met = sqrt(new_met_x*new_met_x + new_met_y*new_met_y);
            // float new_mht = sqrt(jet_x*jet_x + jet_y*jet_y);
     
            FillNtuple();

        }//end loop on events in a file

        delete tree;
        f.Close();
    }//end loop on files

    cout << "[RebalanceLooper::loop] processed " << nEventsTotal << " events" << endl;
    if ( nEventsChain != nEventsTotal ) {
        std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
    }

    cout << nDuplicates << " duplicate events were skipped." << endl;

    CloseNtuple();

    bmark->Stop("benchmark");
    cout << endl;
    cout << nEventsTotal << " Events Processed" << endl;
    cout << "------------------------------" << endl;
    cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
    cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
    cout << endl;
    delete bmark;

    //delete h_sf;
    //sf_file->Close();
    //delete sf_file;

    return;
}

void RebalanceLooper::MakeNtuple(const char *Filename){
    outfile_ = new TFile(Form("%s", Filename), "RECREATE");
    outfile_->cd();
    outTree_ = new TTree("rebalance", "rebalance");

    outTree_->Branch("rebalanceFactors", &rebalanceFactors );
    outTree_->Branch("useJet", &useJet );
    outTree_->Branch("status", &status );
    outTree_->Branch("prescale", &prescale );
    outTree_->Branch("new_met", &new_met);

    return;
}

void RebalanceLooper::InitNtuple () {
    rebalanceFactors.clear();
    useJet.clear();
    status = -999;
    prescale = -999;
    new_met = -999;
    return;
}

void RebalanceLooper::FillNtuple(){
    outTree_->Fill();
    return;
}

void RebalanceLooper::CloseNtuple(){
    outfile_->cd();
    outTree_->Write();
    outfile_->Close();
    return;
}
