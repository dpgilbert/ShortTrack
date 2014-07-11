// C++
#include <iostream>
#include <vector>
#include <set>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"

// CMS2
#include "../MT2CORE/CMS2.h"
#include "../MT2CORE/tools.h"
#include "../MT2CORE/selections.h"
#include "../MT2CORE/hemJet.h"
#include "../MT2CORE/MT2/MT2.h"
#include "../MT2CORE/IsoTrackVeto.h"

// header
#include "ScanChain.h"

using namespace std;
using namespace tas;


void babyMaker::ScanChain(TChain* chain, std::string baby_name){

  MakeBabyNtuple( Form("%s.root", baby_name.c_str()) );

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("Events");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    cms2.Init(tree);
    
    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    //for( unsigned int event = 0; event < 10000; ++event) {
    

      // Get Event Content
      tree->LoadTree(event);
      cms2.GetEntry(event);
      ++nEventsTotal;
    
      // Progress
      CMS2::progress( nEventsTotal, nEventsChain );

      InitBabyNtuple();

      run  = cms2.evt_run();
      lumi = cms2.evt_lumiBlock();
      evt  = cms2.evt_event();
      isData = cms2.evt_isRealData();

      //crossSection = ;
      puWeight = 1.;
      nVert = cms2.evt_nvtxs();
      nTrueInt = cms2.puInfo_trueNumInteractions().at(0);
      rho = cms2.evt_fixgrid_all_rho(); // evt_fixgrid_all_rho() comes from miniAOD, while evt_fixgrid_rho_all() is calculated by the PFCandidateMaker. They are be identical.
      //rho25 = ;
      
      met_pt  = cms2.evt_pfmet();
      met_phi = cms2.evt_pfmetPhi();

      //TRIGGER
      HLT_HT650        = passHLTTriggerPattern("HLT_PFHT650_v") ||  passHLTTriggerPattern("HLT_PFNoPUHT650_v");
      HLT_MET150       = passHLTTriggerPattern("HLT_PFMET150_v"); 
      HLT_ht350met100  = passHLTTriggerPattern("HLT_PFHT350_PFMET100_v") || passHLTTriggerPattern("HLT_PFNoPUHT350_PFMET100_v"); 
      
      //ELECTRONS
      nlep = 0;
      nElectrons10 = 0;
      for(unsigned int iEl = 0; iEl < cms2.els_p4().size(); iEl++){
        if(cms2.els_p4().at(iEl).pt() < 10.0) continue;
        if(!isVetoElectron(iEl)) continue;
        nElectrons10++;
        lep_pt[nlep]   = cms2.els_p4().at(iEl).pt();
        lep_eta[nlep]  = cms2.els_p4().at(iEl).eta(); //save eta, even though we use SCeta for ID
        lep_phi[nlep]  = cms2.els_p4().at(iEl).phi();
        lep_mass[nlep] = cms2.els_mass().at(iEl);
        lep_charge[nlep] = cms2.els_charge().at(iEl);
        lep_pdgId[nlep] = (-11)*cms2.els_charge().at(iEl);
        lep_dxy[nlep] = cms2.els_dxyPV().at(iEl);
        lep_dz[nlep] = cms2.els_dzPV().at(iEl);
        lep_tightId[nlep] = eleTightID(iEl);
        lep_relIso03[nlep] =  eleRelIso03(iEl);
        //lep_relIso04[nlep] = ;
        //lep_mcMatchId[nlep] = ;
        lep_lostHits[nlep] = cms2.els_exp_innerlayers().at(iEl); //cms2.els_lost_pixelhits().at(iEl);
        lep_convVeto[nlep] = cms2.els_conv_vtx_flag().at(iEl);
        lep_tightCharge[nlep] = tightChargeEle(iEl);

        nlep++;
      }

      //MUONS
      nMuons10 = 0;
      for(unsigned int iMu = 0; iMu < cms2.mus_p4().size(); iMu++){
        if(cms2.mus_p4().at(iMu).pt() < 10.0) continue;
        if(fabs(cms2.mus_p4().at(iMu).eta()) > 2.4) continue;
        if(!isLooseMuon(iMu)) continue;
        nMuons10++;
        lep_pt[nlep]   = cms2.mus_p4().at(iMu).pt();
        lep_eta[nlep]  = cms2.mus_p4().at(iMu).eta();
        lep_phi[nlep]  = cms2.mus_p4().at(iMu).phi();
        lep_mass[nlep] = cms2.mus_mass().at(iMu);
        lep_charge[nlep] = cms2.mus_charge().at(iMu);
        lep_pdgId[nlep] = (-13)*cms2.mus_charge().at(iMu);
        lep_dxy[nlep] = cms2.mus_dxyPV().at(iMu); // this uses the silicon track. should we use best track instead?
        lep_dz[nlep] = cms2.mus_dzPV().at(iMu); // this uses the silicon track. should we use best track instead?
        lep_tightId[nlep] = muTightID(iMu);
        lep_relIso03[nlep] = muRelIso03(iMu);
        lep_relIso04[nlep] = muRelIso04(iMu);
        //lep_mcMatchId[nlep] = ;
        lep_lostHits[nlep] = 0; // use defaults as if "good electron"
        lep_convVeto[nlep] = 1;// use defaults as if "good electron"
        lep_tightCharge[nlep] = tightChargeMuon(iMu);

        nlep++;
      }
    

      //JETS
      njet = 0;
      nJet40 = 0;
      nBJet40 = 0;
      ht = 0;
      LorentzVector sumJetp4 = LorentzVector(0,0,0,0);
      vector<LorentzVector> goodJets;
      vector<LorentzVector> hemJets;
      deltaPhiMin = 999.9;
      for(unsigned int iJet = 0; iJet < cms2.pfjets_p4().size(); iJet++){
        if(cms2.pfjets_p4().at(iJet).pt() < 10.0) continue;
        if(fabs(cms2.pfjets_p4().at(iJet).eta()) > 5.2) continue;
        if(!isLoosePFJet(iJet)) continue;
        
        bool lepOverlap = false;
        for(int iLep = 0; iLep < nlep; iLep++){
          if(DeltaR(pfjets_p4().at(iJet).eta(), lep_eta[iLep], pfjets_p4().at(iJet).phi(), lep_phi[iLep]) < 0.5) {
            lepOverlap = true; 
            break;
          }
        }
        if(lepOverlap) continue;

        jet_pt[njet]   = cms2.pfjets_p4().at(iJet).pt();
        jet_eta[njet]  = cms2.pfjets_p4().at(iJet).eta();
        jet_phi[njet]  = cms2.pfjets_p4().at(iJet).phi();
        jet_mass[njet] = cms2.pfjets_mass().at(iJet);
        jet_btagCSV[njet] = cms2.pfjets_combinedSecondaryVertexBJetTag().at(iJet); 
        //jet_mcPt: // need to add jet->genJet() to CMS3
        jet_mcFlavour[njet] = cms2.pfjets_partonFlavour().at(iJet);
        //jet_quarkGluonID
        jet_area[njet] = cms2.pfjets_area().at(iJet);
	      jet_rawPt[njet] = cms2.pfjets_p4().at(iJet).pt() * cms2.pfjets_undoJEC().at(iJet);

        if(isTightPFJet(iJet))  jet_id[njet] = 2;
        else if(isMediumPFJet(iJet)) jet_id[njet] = 1;
        else jet_id[njet] = 0;

        jet_puId[njet] = cms2.pfjets_pileupJetId().at(iJet);

        if( (jet_pt[njet] > 40.0) && (fabs(jet_eta[njet]) < 2.5) ){ 
          goodJets.push_back(cms2.pfjets_p4().at(iJet));
          nJet40++;
          ht+= jet_pt[njet];
          if(jet_btagCSV[njet] >= 0.679) nBJet40++; //CSVM

          sumJetp4 += cms2.pfjets_p4().at(iJet); 

          if(nJet40 <= 4){
            deltaPhiMin = min(deltaPhiMin, DeltaPhi(met_phi, jet_phi[njet]));
          }

        }

        njet++;
      }

      if(goodJets.size() > 1){

        //Hemispheres used in MT2 calculation
        hemJets = getHemJets(goodJets);  

        mt2 = MT2(met_pt, met_phi, hemJets.at(0), hemJets.at(1));
      
        pseudojet1_pt   = hemJets.at(0).pt();
        pseudojet1_eta  = hemJets.at(0).eta();
        pseudojet1_phi  = hemJets.at(0).phi();
        pseudojet1_mass = hemJets.at(0).mass();
        pseudojet2_pt   = hemJets.at(1).pt();
        pseudojet2_eta  = hemJets.at(1).eta();
        pseudojet2_phi  = hemJets.at(1).phi();
        pseudojet2_mass = hemJets.at(1).mass();

      }

      mht_pt  = sumJetp4.pt();
      mht_phi = -sumJetp4.phi();

      //TAUS
      ntau = 0;
      for(unsigned int iTau = 0; iTau < cms2.taus_pf_p4().size(); iTau++){
        if(cms2.taus_pf_p4().at(iTau).pt() < 20.0) continue; 
        if(fabs(cms2.taus_pf_p4().at(iTau).eta()) > 2.3) continue; 
	if (!cms2.taus_pf_byLooseCombinedIsolationDeltaBetaCorr3Hits().at(iTau)) continue; // HPS3 hits taus
	if (!cms2.taus_pf_againstElectronLoose().at(iTau)) continue; // loose electron rejection 
	if (!cms2.taus_pf_againstMuonTight().at(iTau)) continue; // loose muon rejection 
        

        tau_pt[ntau]   = cms2.taus_pf_p4().at(iTau).pt();
        tau_eta[ntau]  = cms2.taus_pf_p4().at(iTau).eta();
        tau_phi[ntau]  = cms2.taus_pf_p4().at(iTau).phi();
        tau_mass[ntau] = cms2.taus_pf_mass().at(iTau);
        tau_charge[ntau] = cms2.taus_pf_charge().at(iTau);
        tau_dxy[ntau] = 0; // could use the tau->dxy() function instead, but not sure what it does
        tau_dz[ntau] = 0; // not sure how to get this. 
        tau_isoCI3hit[ntau] = cms2.taus_pf_byCombinedIsolationDeltaBetaCorrRaw3Hits().at(iTau);
	int temp = 0;
	if (cms2.taus_pf_byLooseCombinedIsolationDeltaBetaCorr3Hits().at(iTau)) temp = 1;
	if (cms2.taus_pf_byMediumCombinedIsolationDeltaBetaCorr3Hits().at(iTau)) temp = 2;
	if (cms2.taus_pf_byTightCombinedIsolationDeltaBetaCorr3Hits().at(iTau)) temp = 3;
	tau_idCI3hit[ntau] = temp;
        //tau_mcMatchId[ntau] = ;

        ntau++;
      }

      //PHOTONS
      ngamma = 0;
      for(unsigned int iGamma = 0; iGamma < cms2.photons_p4().size(); iGamma++){
        if(cms2.photons_p4().at(iGamma).pt() < 20.0) continue;
        if(fabs(cms2.photons_p4().at(iGamma).pt()) > 2.5) continue;
	//WAIT FOR NEW CMS3 if (cms2.photons_photonID_loose().at(iGamma)==0) continue;
        gamma_pt[ngamma]   = cms2.photons_p4().at(iGamma).pt();
        gamma_eta[ngamma]  = cms2.photons_p4().at(iGamma).eta();
        gamma_phi[ngamma]  = cms2.photons_p4().at(iGamma).phi();
        gamma_mass[ngamma] = cms2.photons_mass().at(iGamma);

        gamma_sigmaIetaIeta[ngamma] = cms2.photons_sigmaIEtaIEta().at(iGamma);

        //WAIT FOR NEW CMS3 gamma_chHadIso[ngamma] = photons_chargedHadronIso().at(iGamma);
        //WAIT FOR NEW CMS3 gamma_neuHadIso[ngamma] = photons_neutralHadronIso().at(iGamma);
        //WAIT FOR NEW CMS3 gamma_photIso[ngamma] = photons_photonIso().at(iGamma);
        //WAIT FOR NEW CMS3 gamma_r9[ngamma] =  photons_full5x5_r9().at(iGamma);
        //WAIT FOR NEW CMS3 gamma_hOverE[ngamma] =  photons_full5x5_hOverEtowBC().at(iGamma);
        //WAIT FOR NEW CMS3 gamma_id[ngamma] =  photons_photonID_tight().at(iGamma) ? 2 : 0; // Medium working point is not saved in miniAOD, should implement on our own if we want it


        //gamma_mcMatchId[ngamma] = ;
        
        ngamma++;
      }

      //GEN PARTICLES
      ngenPart = 0;
      for(unsigned int iGen = 0; iGen < cms2.genps_p4().size(); iGen++){
        genPart_pt[ngenPart] = cms2.genps_p4().at(iGen).pt();
        genPart_eta[ngenPart] = cms2.genps_p4().at(iGen).eta();
        genPart_phi[ngenPart] = cms2.genps_p4().at(iGen).phi();
        genPart_mass[ngenPart] = cms2.genps_mass().at(iGen);
        genPart_pdgId[ngenPart] = cms2.genps_id().at(iGen);
        //genPart_charge[ngenPart] = ;
        genPart_motherId[ngenPart] = cms2.genps_id_mother().at(iGen);
        //genPart_grandmaId[ngenPart] = ;

        ngenPart++;
      }


      //ISOTRACK
      std::map<float, int> pt_ordering;
      vector<float>vec_isoTrack_pt;
      vector<float>vec_isoTrack_eta;
      vector<float>vec_isoTrack_phi;
      vector<float>vec_isoTrack_mass;
      vector<float>vec_isoTrack_absIso;
      vector<float>vec_isoTrack_dz;
      vector<int>  vec_isoTrack_pdgId;

      nisoTrack = 0;
      for (unsigned int ipf = 0; ipf < pfcands_p4().size(); ipf++) {
 
        if(cms2.pfcands_charge().at(ipf) == 0) continue;
        if(fabs(cms2.pfcands_dz().at(ipf)) > 0.1) continue;

        float cand_pt = cms2.pfcands_p4().at(ipf).pt();
        if(cand_pt < 5) continue;
 
        float absiso  = TrackIso(ipf);
        if(absiso >= min(0.2*cand_pt, 8.0)) continue;

        pt_ordering[cand_pt] = nisoTrack;

        vec_isoTrack_pt.push_back    ( cand_pt                          );
        vec_isoTrack_eta.push_back   ( cms2.pfcands_p4().at(ipf).eta()  );
        vec_isoTrack_phi.push_back   ( cms2.pfcands_p4().at(ipf).phi()  );
        //vec_isoTrack_mass.push_back  ( cms2.pfcands_p4().at(ipf).mass() );
        vec_isoTrack_mass.push_back  ( cms2.pfcands_mass().at(ipf)      );
        vec_isoTrack_absIso.push_back( absiso                           );
        vec_isoTrack_dz.push_back    ( cms2.pfcands_dz().at(ipf)        );
        vec_isoTrack_pdgId.push_back ( cms2.pfcands_particleId().at(ipf));

        nisoTrack++;
      }  

      //now fill arrays from vectors, isotracks with largest pt first
      int i = 0;
      for(std::map<float, int>::reverse_iterator it = pt_ordering.rbegin(); it!= pt_ordering.rend(); ++it){
        isoTrack_pt[i]     = vec_isoTrack_pt.at(it->second);
        isoTrack_eta[i]    = vec_isoTrack_eta.at(it->second);
        isoTrack_phi[i]    = vec_isoTrack_phi.at(it->second);
        isoTrack_mass[i]   = vec_isoTrack_mass.at(it->second);
        isoTrack_absIso[i] = vec_isoTrack_absIso.at(it->second);
        isoTrack_dz[i]     = vec_isoTrack_dz.at(it->second);
        isoTrack_pdgId[i]  = vec_isoTrack_pdgId.at(it->second);
        i++;
      }
        


      FillBabyNtuple();

   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  CloseBabyNtuple();

  return;
}
