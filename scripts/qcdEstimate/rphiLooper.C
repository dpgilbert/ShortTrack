// usage: root rphiLooper.C
// Run this first
// calculates the numerator/denominator histograms for computing
// r_phi and puts in the rphi_hists directory
//
// takes roughly 15 minutes to run over all qcd babies

void rphiLooper(){

    // add unskimmed babies to chain
    TChain *chain = new TChain("mt2");
    chain->Add("/nfs-6/userdata/mt2/V00-08-01_json_Cert_271036-274240/qcd_ht*.root");
    
    // something to identify these
    TString tag = "newmcv7";

    TString baseline;
    baseline  = "nJet30>=2 && diffMetMht/met_pt<0.5 && (nMuons10 + nElectrons10 + nPFLep5LowMT + nPFHad10LowMT)==0 && nVert>0 && met_pt>30 && nJet30FailId==0";
    // spike rejection
    // baseline += " && (evt_id>150&&(evt_id>151||ht<450)&&(evt_id>152||ht<575)&&(evt_id>153||ht<1000)&&(evt_id>154||ht<1500))";
    baseline += " && (evt_id>150&&(evt_id>151||ht<450)&&(evt_id>152||ht<575)&&(evt_id>153||ht<575)&&(evt_id>154||ht<1000))";
    // remove anomalous charged candidates
    baseline += " && Flag_badChargedCandidateFilter";
    
    TString regions[5] = {"VL","L","M","H","UH"};
    TString ht_sels[5] = {"ht>200 && ht<450","ht>450 && ht<575","ht>575 && ht<1000","ht>1000 && ht<1500","ht>1500"};
    TH1D *h_rphi_num = new TH1D("h_rphi_num","",290,50,1500);
    TH1D *h_rphi_den = new TH1D("h_rphi_den","",290,50,1500);
    for(int i=0; i<5; i++){
        cout << "Doing region " << regions[i] << endl;

        TString sel = ht_sels[i] + " && " + baseline;
        chain->Draw("mt2>>h_rphi_num",Form("evt_scale1fb*(mt2>50 && deltaPhiMin>0.3 && (%s))",sel.Data()),"goff");
        chain->Draw("mt2>>h_rphi_den",Form("evt_scale1fb*(mt2>50 && deltaPhiMin<0.3 && (%s))",sel.Data()),"goff");

        TFile f(Form("rphi_hists/rphi_hists_%s_%s.root",tag.Data(),regions[i].Data()),"RECREATE");
    
        h_rphi_num->Write();
        h_rphi_den->Write();

        f.Close();
    }

}
