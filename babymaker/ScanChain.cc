// C++
#include <iostream>
#include <vector>
#include <set>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"

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

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  MakeBabyNtuple( Form("%s.root", baby_name.c_str()) );

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "Running on " << nEventsChain << " events" << endl;
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
    //for( unsigned int event = 0; event < 100; ++event) {

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

      nevents = cms2.evt_nEvts();
      scale1fb = cms2.evt_scale1fb();
      crossSection = cms2.evt_xsec_incl();
      puWeight = 1.;
      nTrueInt = cms2.puInfo_trueNumInteractions().at(0);
      rho = cms2.evt_fixgridfastjet_all_rho(); //this one is used in JECs
      //rho25 = ;

      //VERTICES
      nVert = 0;
      for(unsigned int ivtx=0; ivtx < cms2.evt_nvtxs(); ivtx++){

        if(cms2.vtxs_isFake().at(ivtx)) continue;
        if(cms2.vtxs_ndof().at(ivtx) <= 4) continue;
        if(fabs(cms2.vtxs_position().at(ivtx).z()) > 24) continue;
        if(cms2.vtxs_position().at(ivtx).Rho() > 2) continue;

        nVert++;
  
      }
      
      met_pt  = cms2.evt_pfmet();
      met_phi = cms2.evt_pfmetPhi();
      met_genPt  = cms2.gen_met();
      met_genPhi = cms2.gen_metPhi();

      //TRIGGER
      HLT_HT650        = passHLTTriggerPattern("HLT_PFHT650_v") ||  passHLTTriggerPattern("HLT_PFNoPUHT650_v");
      HLT_MET150       = passHLTTriggerPattern("HLT_PFMET150_v"); 
      HLT_ht350met100  = passHLTTriggerPattern("HLT_PFHT350_PFMET100_v") || passHLTTriggerPattern("HLT_PFNoPUHT350_PFMET100_v"); 
      
      //GEN PARTICLES
      ngenPart = 0;
      for(unsigned int iGen = 0; iGen < cms2.genps_p4().size(); iGen++){
	if (ngenPart >= max_ngenPart) {
          std::cout << "WARNING: attempted to fill more than " << max_ngenPart << " gen particles" << std::endl;
	  break;
	}
        genPart_pt[ngenPart] = cms2.genps_p4().at(iGen).pt();
        genPart_eta[ngenPart] = cms2.genps_p4().at(iGen).eta();
        genPart_phi[ngenPart] = cms2.genps_p4().at(iGen).phi();
        genPart_mass[ngenPart] = cms2.genps_mass().at(iGen);
        genPart_pdgId[ngenPart] = cms2.genps_id().at(iGen);
        genPart_charge[ngenPart] = cms2.genps_charge().at(iGen);
	genPart_motherId[ngenPart] =cms2.genps_id_simplemother().at(iGen);
	genPart_grandmaId[ngenPart] = cms2.genps_id_simplegrandma().at(iGen);
        ngenPart++;
      }


      //LEPTONS
      std::map<float, int> lep_pt_ordering;
      vector<float>vec_lep_pt;
      vector<float>vec_lep_eta;
      vector<float>vec_lep_phi;
      vector<float>vec_lep_mass;
      vector<float>vec_lep_charge;
      vector<int>  vec_lep_pdgId;
      vector<float>vec_lep_dxy;
      vector<float>vec_lep_dz;
      vector<int>  vec_lep_tightId;
      vector<float>vec_lep_relIso03;
      vector<float>vec_lep_relIso04;
      vector<int>  vec_lep_mcMatchId;
      vector<int>  vec_lep_lostHits;
      vector<int>  vec_lep_convVeto;
      vector<int>  vec_lep_tightCharge;


      //ELECTRONS
      nlep = 0;
      nElectrons10 = 0;
      for(unsigned int iEl = 0; iEl < cms2.els_p4().size(); iEl++){
        if(cms2.els_p4().at(iEl).pt() < 10.0) continue;
        if(fabs(cms2.els_p4().at(iEl).eta()) > 2.4) continue;
        if(!isVetoElectron(iEl)) continue;
        nElectrons10++;
        lep_pt_ordering[cms2.els_p4().at(iEl).pt()] = nlep;
        vec_lep_pt.push_back ( cms2.els_p4().at(iEl).pt());
        vec_lep_eta.push_back ( cms2.els_p4().at(iEl).eta()); //save eta, even though we use SCeta for ID
        vec_lep_phi.push_back ( cms2.els_p4().at(iEl).phi());
        vec_lep_mass.push_back ( cms2.els_mass().at(iEl));
        vec_lep_charge.push_back ( cms2.els_charge().at(iEl));
        vec_lep_pdgId.push_back ( (-11)*cms2.els_charge().at(iEl));
        vec_lep_dxy.push_back ( cms2.els_dxyPV().at(iEl));
        vec_lep_dz.push_back ( cms2.els_dzPV().at(iEl));
        vec_lep_tightId.push_back ( eleTightID(iEl));
        vec_lep_relIso03.push_back (  eleRelIso03(iEl));
        vec_lep_relIso04.push_back ( 0);
        if (cms2.els_mc3dr().at(iEl) < 0.2 && cms2.els_mc3idx().at(iEl) != -9999 && abs(cms2.els_mc3_id().at(iEl)) == 11) { // matched to a prunedGenParticle electron?
          int momid =  abs(genPart_motherId[cms2.els_mc3idx().at(iEl)]);
          vec_lep_mcMatchId.push_back ( momid != 11 ? momid : genPart_grandmaId[cms2.els_mc3idx().at(iEl)]); // if mother is different store mother, otherwise store grandmother
        }
	else vec_lep_mcMatchId.push_back (0);

        vec_lep_lostHits.push_back ( cms2.els_exp_innerlayers().at(iEl)); //cms2.els_lost_pixelhits().at(iEl);
        vec_lep_convVeto.push_back ( !cms2.els_conv_vtx_flag().at(iEl));
        vec_lep_tightCharge.push_back ( tightChargeEle(iEl));

        nlep++;
      }

      //MUONS
      nMuons10 = 0;
      for(unsigned int iMu = 0; iMu < cms2.mus_p4().size(); iMu++){
        if(cms2.mus_p4().at(iMu).pt() < 10.0) continue;
        if(fabs(cms2.mus_p4().at(iMu).eta()) > 2.4) continue;
        if(!isLooseMuon(iMu)) continue;
        if (muRelIso03(iMu) > 0.15) continue; // to avoid DR(jet,lepton) with leptons from b 
        nMuons10++;
        lep_pt_ordering[cms2.mus_p4().at(iMu).pt()] = nlep;
        vec_lep_pt.push_back ( cms2.mus_p4().at(iMu).pt());
        vec_lep_eta.push_back ( cms2.mus_p4().at(iMu).eta());
        vec_lep_phi.push_back ( cms2.mus_p4().at(iMu).phi());
        vec_lep_mass.push_back ( cms2.mus_mass().at(iMu));
        vec_lep_charge.push_back ( cms2.mus_charge().at(iMu));
        vec_lep_pdgId.push_back ( (-13)*cms2.mus_charge().at(iMu));
        vec_lep_dxy.push_back ( cms2.mus_dxyPV().at(iMu)); // this uses the silicon track. should we use best track instead?
        vec_lep_dz.push_back ( cms2.mus_dzPV().at(iMu)); // this uses the silicon track. should we use best track instead?
        vec_lep_tightId.push_back ( muTightID(iMu));
        vec_lep_relIso03.push_back ( muRelIso03(iMu));
        vec_lep_relIso04.push_back ( muRelIso04(iMu));
        if (cms2.mus_mc3dr().at(iMu) < 0.2 && cms2.mus_mc3idx().at(iMu) != -9999 && abs(cms2.mus_mc3_id().at(iMu)) == 13) { // matched to a prunedGenParticle electron?
          int momid =  abs(genPart_motherId[cms2.mus_mc3idx().at(iMu)]);
          vec_lep_mcMatchId.push_back ( momid != 13 ? momid : genPart_grandmaId[cms2.mus_mc3idx().at(iMu)]); // if mother is different store mother, otherwise store grandmother
	}
	else vec_lep_mcMatchId.push_back (0);
        vec_lep_lostHits.push_back ( cms2.mus_exp_innerlayers().at(iMu)); // use defaults as if "good electron"
        vec_lep_convVeto.push_back ( 1);// use defaults as if "good electron"
        vec_lep_tightCharge.push_back ( tightChargeMuon(iMu));

        nlep++;
      }

      // Implement pT ordering for leptons (it's irrelevant but easier for us to add than for ETH to remove)
      //now fill arrays from vectors, isotracks with largest pt first
      int i = 0;
      for(std::map<float, int>::reverse_iterator it = lep_pt_ordering.rbegin(); it!= lep_pt_ordering.rend(); ++it){

	if (i >= max_nlep) {
          std::cout << "WARNING: attempted to fill more than " << max_nlep << " leptons" << std::endl;
	  break;
	}

        lep_pt[i]     = vec_lep_pt.at(it->second);
        lep_eta[i]    = vec_lep_eta.at(it->second);
        lep_phi[i]    = vec_lep_phi.at(it->second);
        lep_mass[i]   = vec_lep_mass.at(it->second);
        lep_charge[i] = vec_lep_charge.at(it->second);
        lep_pdgId[i]  = vec_lep_pdgId.at(it->second);
        lep_dz[i]     = vec_lep_dz.at(it->second);
        lep_dxy[i]    = vec_lep_dxy.at(it->second);
        lep_tightId[i]     = vec_lep_tightId.at(it->second);
        lep_relIso03[i]    = vec_lep_relIso03.at(it->second);
        lep_relIso04[i]    = vec_lep_relIso04.at(it->second);
        lep_mcMatchId[i]   = vec_lep_mcMatchId.at(it->second);
        lep_lostHits[i]    = vec_lep_lostHits.at(it->second);
        lep_convVeto[i]    = vec_lep_convVeto.at(it->second);
        lep_tightCharge[i] = vec_lep_tightCharge.at(it->second);
        i++;
      }

      //JETS
      //before we start, check that no genGet is matched to multiple recoJets
      //vector<float> pTofMatchedGenJets;
      //for(unsigned int iJet = 0; iJet < cms2.pfjets_p4().size(); iJet++){
      //  if(cms2.pfjets_p4().at(iJet).pt() < 10.0) continue;
      //  if(fabs(cms2.pfjets_p4().at(iJet).eta()) > 5.2) continue;
      //	float matchedPt = cms2.pfjets_mc_p4().at(iJet).pt();
      //	if (matchedPt!=0) {
      //	  if ( find( pTofMatchedGenJets.begin(), pTofMatchedGenJets.end(), matchedPt  ) != pTofMatchedGenJets.end() ) {
      //	    cout<<" This genJetPt is identical to one that was already matched to another reco jet of higher pT. Need to find another match"<<endl;
      //	    cout<<"event"<<evt_event()<<" jet pT: "<<cms2.pfjets_p4().at(iJet).pt()<<" genJetPt: "<<matchedPt<<endl;
      //	  }
      //	  else {
      //	    pTofMatchedGenJets.push_back( matchedPt );
      //	  }
      //	}
      //}

      //check baseline selections
      vector<int> passJets; //index of jets that pass baseline selections
      for(unsigned int iJet = 0; iJet < cms2.pfjets_p4().size(); iJet++){
        if(cms2.pfjets_p4().at(iJet).pt() < 10.0) continue;
        if(fabs(cms2.pfjets_p4().at(iJet).eta()) > 5.2) continue;
        if(!isLoosePFJet(iJet)) continue;

        passJets.push_back(iJet);
      }

      //check overlapping with leptons
      //only want to remove the closest jet to a lepton, threshold deltaR < 0.4
      vector<int> removedJets; //index of jets to be removed because they overlap with a lepton
      for(int iLep = 0; iLep < nlep; iLep++){

        float minDR = 0.4;
        int minIndex = -1;
        for(unsigned int passIdx = 0; passIdx < passJets.size(); passIdx++){ //loop through jets that passed baseline selections

          int iJet = passJets.at(passIdx);

          if(cms2.pfjets_p4().at(iJet).pt() < 10.0) continue;
          if(fabs(cms2.pfjets_p4().at(iJet).eta()) > 5.2) continue;
          if(!isLoosePFJet(iJet)) continue;

          bool alreadyRemoved = false;
          for(unsigned int j=0; j<removedJets.size(); j++){
            if(iJet == removedJets.at(j)){
              alreadyRemoved = true;
              break;
            }
          }
          if(alreadyRemoved) continue;

          float thisDR = DeltaR(pfjets_p4().at(iJet).eta(), lep_eta[iLep], pfjets_p4().at(iJet).phi(), lep_phi[iLep]);
          if(thisDR < minDR){
            minDR = thisDR; 
            minIndex = iJet;
          }
        }
        removedJets.push_back(minIndex);
      }

      njet = 0;
      nJet40 = 0;
      nBJet40 = 0;
      ht = 0;
      LorentzVector sumJetp4 = LorentzVector(0,0,0,0);
      vector<LorentzVector> goodJets;
      vector<LorentzVector> hemJets;
      deltaPhiMin = 999;

      //now fill variables for jets that pass baseline selections and don't overlap with a lepton
      for(unsigned int passIdx = 0; passIdx < passJets.size(); passIdx++){

        int iJet = passJets.at(passIdx);

        //check against list of jets that overlap with a lepton
        bool isOverlapJet = false;
        for(unsigned int j=0; j<removedJets.size(); j++){
          if(iJet == removedJets.at(j)){
            isOverlapJet = true;
            break;
          }
        }
        if(isOverlapJet) continue;
        
	if (njet >= max_njet) {
          std::cout << "WARNING: attempted to fill more than " << max_njet << " jets" << std::endl;
	  break;
	}

        jet_pt[njet]   = cms2.pfjets_p4().at(iJet).pt();
        jet_eta[njet]  = cms2.pfjets_p4().at(iJet).eta();
        jet_phi[njet]  = cms2.pfjets_p4().at(iJet).phi();
        jet_mass[njet] = cms2.pfjets_mass().at(iJet);
        jet_btagCSV[njet] = cms2.pfjets_combinedSecondaryVertexBJetTag().at(iJet); 
        jet_mcPt[njet] = cms2.pfjets_mc_p4().at(iJet).pt();
        jet_mcFlavour[njet] = cms2.pfjets_partonFlavour().at(iJet);
        //jet_quarkGluonID
        jet_area[njet] = cms2.pfjets_area().at(iJet);
	      jet_rawPt[njet] = cms2.pfjets_p4().at(iJet).pt() * cms2.pfjets_undoJEC().at(iJet);

        if(isTightPFJet(iJet))  jet_id[njet] = 3;
        else if(isMediumPFJet(iJet)) jet_id[njet] = 2;
        else jet_id[njet] = 1; //required to be loose above

        jet_puId[njet] = loosePileupJetId(iJet) ? 1 : 0;

        if( (jet_pt[njet] > 40.0) && (fabs(jet_eta[njet]) < 2.5) ){ 
          goodJets.push_back(cms2.pfjets_p4().at(iJet));
          nJet40++;
          ht+= jet_pt[njet];
          if(jet_btagCSV[njet] >= 0.679) nBJet40++; //CSVM

          sumJetp4 -= cms2.pfjets_p4().at(iJet); 

          if(nJet40 <= 4){
            deltaPhiMin = min(deltaPhiMin, DeltaPhi(met_phi, jet_phi[njet]));
          }

        }

        njet++;
      }

      if(goodJets.size() > 1){

        //Hemispheres used in MT2 calculation
        hemJets = getHemJets(goodJets);  

        mt2 = HemMT2(met_pt, met_phi, hemJets.at(0), hemJets.at(1));
      
        pseudoJet1_pt   = hemJets.at(0).pt();
        pseudoJet1_eta  = hemJets.at(0).eta();
        pseudoJet1_phi  = hemJets.at(0).phi();
        pseudoJet1_mass = hemJets.at(0).mass();
        pseudoJet2_pt   = hemJets.at(1).pt();
        pseudoJet2_eta  = hemJets.at(1).eta();
        pseudoJet2_phi  = hemJets.at(1).phi();
        pseudoJet2_mass = hemJets.at(1).mass();

      }

      mht_pt  = sumJetp4.pt();
      mht_phi = sumJetp4.phi();

      TVector2 mhtVector = TVector2(mht_pt*cos(mht_phi), mht_pt*sin(mht_phi));
      TVector2 metVector = TVector2(met_pt*cos(met_phi), met_pt*sin(met_phi));
      diffMetMht = (mhtVector - metVector).Mod();

      
      //GEN MT2
      vector<LorentzVector> goodGenJets;
      for(unsigned int iGenJet=0; iGenJet < cms2.genjets_p4NoMuNoNu().size(); iGenJet++){
        if(cms2.genjets_p4NoMuNoNu().at(iGenJet).pt() < 40.0) continue;
        if(fabs(cms2.genjets_p4NoMuNoNu().at(iGenJet).eta()) > 2.5) continue;
        goodGenJets.push_back(cms2.genjets_p4NoMuNoNu().at(iGenJet));
      }
      if(goodGenJets.size() > 1){
        hemJets = getHemJets(goodGenJets);  
        mt2_gen = HemMT2(met_genPt, met_genPhi, hemJets.at(0), hemJets.at(1));
      }


      //TAUS
      ntau = 0;
      nTaus20 = 0;
      for(unsigned int iTau = 0; iTau < cms2.taus_pf_p4().size(); iTau++){
        if(cms2.taus_pf_p4().at(iTau).pt() < 20.0) continue; 
        if(fabs(cms2.taus_pf_p4().at(iTau).eta()) > 2.3) continue; 
	      if (!cms2.taus_pf_byLooseCombinedIsolationDeltaBetaCorr3Hits().at(iTau)) continue; // HPS3 hits taus
	      if (!cms2.taus_pf_againstElectronLoose().at(iTau)) continue; // loose electron rejection 
	      if (!cms2.taus_pf_againstMuonTight().at(iTau)) continue; // loose muon rejection 
        
	if (ntau >= max_ntau) {
          std::cout << "WARNING: attempted to fill more than " << max_ntau << " taus" << std::endl;
	  break;
	}

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
        if(tau_pt[ntau] > 20) nTaus20++;
        //tau_mcMatchId[ntau] = ; // Have to do this by hand unless we want to add tau_mc branches in CMS3 through the CandToGenAssMaker

        ntau++;
      }

      //PHOTONS
      ngamma = 0;
      nGammas20 = 0;
      for(unsigned int iGamma = 0; iGamma < cms2.photons_p4().size(); iGamma++){
        if(cms2.photons_p4().at(iGamma).pt() < 20.0) continue;
        if(fabs(cms2.photons_p4().at(iGamma).eta()) > 2.5) continue;
	      if (cms2.photons_photonID_loose().at(iGamma)==0) continue;

	if (ngamma >= max_ngamma) {
          std::cout << "WARNING: attempted to fill more than " << max_ngamma << " photons" << std::endl;
	  break;
	}

        gamma_pt[ngamma]   = cms2.photons_p4().at(iGamma).pt();
        gamma_eta[ngamma]  = cms2.photons_p4().at(iGamma).eta();
        gamma_phi[ngamma]  = cms2.photons_p4().at(iGamma).phi();
        gamma_mass[ngamma] = cms2.photons_mass().at(iGamma);
        gamma_sigmaIetaIeta[ngamma] = cms2.photons_full5x5_sigmaIEtaIEta().at(iGamma);
        gamma_chHadIso[ngamma] = photons_chargedHadronIso().at(iGamma);
        gamma_neuHadIso[ngamma] = photons_neutralHadronIso().at(iGamma);
        gamma_phIso[ngamma] = photons_photonIso().at(iGamma);
        gamma_r9[ngamma] =  photons_full5x5_r9().at(iGamma);
        gamma_hOverE[ngamma] =  photons_full5x5_hOverEtowBC().at(iGamma);
        gamma_idCutBased[ngamma] =  photons_photonID_tight().at(iGamma) ? 2 : 0; // Medium working point is not saved in miniAOD, should implement on our own if we want it
        if(gamma_pt[ngamma] > 20) nGammas20++;

        //gamma_mcMatchId[ngamma] = ;
        
        ngamma++;
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
      vector<int>  vec_isoTrack_mcMatchId;

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
        vec_isoTrack_mass.push_back  ( cms2.pfcands_mass().at(ipf)      );
        vec_isoTrack_absIso.push_back( absiso                           );
        vec_isoTrack_dz.push_back    ( cms2.pfcands_dz().at(ipf)        );
        vec_isoTrack_pdgId.push_back ( cms2.pfcands_particleId().at(ipf));
        vec_isoTrack_mcMatchId.push_back ( 0 );

        nisoTrack++;
      }  

      //now fill arrays from vectors, isotracks with largest pt first
       i = 0;
      for(std::map<float, int>::reverse_iterator it = pt_ordering.rbegin(); it!= pt_ordering.rend(); ++it){

	if (i >= max_nisoTrack) {
          std::cout << "WARNING: attempted to fill more than " << max_nisoTrack << " iso tracks" << std::endl;
	  break;
	}

        isoTrack_pt[i]     = vec_isoTrack_pt.at(it->second);
        isoTrack_eta[i]    = vec_isoTrack_eta.at(it->second);
        isoTrack_phi[i]    = vec_isoTrack_phi.at(it->second);
        isoTrack_mass[i]   = vec_isoTrack_mass.at(it->second);
        isoTrack_absIso[i] = vec_isoTrack_absIso.at(it->second);
        isoTrack_dz[i]     = vec_isoTrack_dz.at(it->second);
        isoTrack_pdgId[i]  = vec_isoTrack_pdgId.at(it->second);
        isoTrack_mcMatchId[i]  = vec_isoTrack_mcMatchId.at(it->second);
        i++;
      }
        


      FillBabyNtuple();

   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "Processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  CloseBabyNtuple();

  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}

void babyMaker::MakeBabyNtuple(const char *BabyFilename){

  //
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  rootdir->cd();
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("treeProducerSusyFullHad", "A Baby Ntuple");

  BabyTree_->Branch("run", &run );
  BabyTree_->Branch("lumi", &lumi );
  BabyTree_->Branch("evt", &evt );
  BabyTree_->Branch("isData", &isData );

  BabyTree_->Branch("nevents", &nevents );
  BabyTree_->Branch("scale1fb", &scale1fb);
  BabyTree_->Branch("crossSection", &crossSection );
  BabyTree_->Branch("puWeight", &puWeight );
  BabyTree_->Branch("nVert", &nVert );
  BabyTree_->Branch("nTrueInt", &nTrueInt );
  BabyTree_->Branch("rho", &rho );
  BabyTree_->Branch("rho25", &rho25 );
  BabyTree_->Branch("nJet40", &nJet40 );
  BabyTree_->Branch("nBJet40", &nBJet40 );
  BabyTree_->Branch("nMuons10", &nMuons10 );
  BabyTree_->Branch("nElectrons10", &nElectrons10 );
  BabyTree_->Branch("nTaus20", &nTaus20 );
  BabyTree_->Branch("nGammas20", &nGammas20 );
  BabyTree_->Branch("deltaPhiMin", &deltaPhiMin );
  BabyTree_->Branch("diffMetMht", &diffMetMht );
  BabyTree_->Branch("ht", &ht );
  BabyTree_->Branch("mt2", &mt2 );
  BabyTree_->Branch("mt2_gen", &mt2_gen );
  BabyTree_->Branch("pseudoJet1_pt", &pseudoJet1_pt );
  BabyTree_->Branch("pseudoJet1_eta", &pseudoJet1_eta );
  BabyTree_->Branch("pseudoJet1_phi", &pseudoJet1_phi );
  BabyTree_->Branch("pseudoJet1_mass", &pseudoJet1_mass );
  BabyTree_->Branch("pseudoJet2_pt", &pseudoJet2_pt );
  BabyTree_->Branch("pseudoJet2_eta", &pseudoJet2_eta );
  BabyTree_->Branch("pseudoJet2_phi", &pseudoJet2_phi );
  BabyTree_->Branch("pseudoJet2_mass", &pseudoJet2_mass );
  BabyTree_->Branch("mht_pt", &mht_pt );
  BabyTree_->Branch("mht_phi", &mht_phi );
  BabyTree_->Branch("met_pt", &met_pt );
  BabyTree_->Branch("met_phi", &met_phi );
  BabyTree_->Branch("met_rawPt",  &met_rawPt );
  BabyTree_->Branch("met_rawPhi", &met_rawPhi );
  BabyTree_->Branch("met_caloPt",  &met_caloPt );
  BabyTree_->Branch("met_caloPhi", &met_caloPhi );
  BabyTree_->Branch("met_genPt",  &met_genPt );
  BabyTree_->Branch("met_genPhi", &met_genPhi );
  BabyTree_->Branch("HLT_HT650", &HLT_HT650 );
  BabyTree_->Branch("HLT_MET150", &HLT_MET150 );
  BabyTree_->Branch("HLT_ht350met100", &HLT_ht350met100 );
  BabyTree_->Branch("nlep", &nlep, "nlep/I" );
  BabyTree_->Branch("lep_pt", lep_pt, "lep_pt[nlep]/F");
  BabyTree_->Branch("lep_eta", lep_eta, "lep_eta[nlep]/F" );
  BabyTree_->Branch("lep_phi", lep_phi, "lep_phi[nlep]/F" );
  BabyTree_->Branch("lep_mass", lep_mass, "lep_mass[nlep]/F" );
  BabyTree_->Branch("lep_charge", lep_charge, "lep_charge[nlep]/I" );
  BabyTree_->Branch("lep_pdgId", lep_pdgId, "lep_pdgId[nlep]/I" );
  BabyTree_->Branch("lep_dxy", lep_dxy, "lep_dxy[nlep]/F" );
  BabyTree_->Branch("lep_dz", lep_dz, "lep_dz[nlep]/F" );
  BabyTree_->Branch("lep_tightId", lep_tightId, "lep_tightId[nlep]/I" );
  BabyTree_->Branch("lep_relIso03", lep_relIso03, "lep_relIso03[nlep]/F" );
  BabyTree_->Branch("lep_relIso04", lep_relIso04, "lep_relIso04[nlep]/F" );
  BabyTree_->Branch("lep_mcMatchId", lep_mcMatchId, "lep_mcMatchId[nlep]/I" );
  BabyTree_->Branch("lep_lostHits", lep_lostHits, "lep_lostHits[nlep]/I" );
  BabyTree_->Branch("lep_convVeto", lep_convVeto, "lep_convVeto[nlep]/I" );
  BabyTree_->Branch("lep_tightCharge", lep_tightCharge, "lep_tightCharge[nlep]/I" );
  BabyTree_->Branch("nisoTrack", &nisoTrack, "nisoTrack/I" );
  BabyTree_->Branch("isoTrack_pt", isoTrack_pt, "isoTrack_pt[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_eta", isoTrack_eta, "isoTrack_eta[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_phi", isoTrack_phi, "isoTrack_phi[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_mass", isoTrack_mass, "isoTrack_mass[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_absIso", isoTrack_absIso, "isoTrack_absIso[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_dz", isoTrack_dz, "isoTrack_dz[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_pdgId", isoTrack_pdgId, "isoTrack_pdgId[nisoTrack]/I" );
  BabyTree_->Branch("isoTrack_mcMatchId", isoTrack_mcMatchId, "isoTrack_mcMatchId[nisoTrack]/I" );
  BabyTree_->Branch("ntau", &ntau, "ntau/I" );
  BabyTree_->Branch("tau_pt", tau_pt, "tau_pt[ntau]/F" );
  BabyTree_->Branch("tau_eta", tau_eta, "tau_eta[ntau]/F" );
  BabyTree_->Branch("tau_phi", tau_phi, "tau_phi[ntau]/F" );
  BabyTree_->Branch("tau_mass", tau_mass, "tau_mass[ntau]/F" );
  BabyTree_->Branch("tau_charge", tau_charge, "tau_charge[ntau]/I" );
  BabyTree_->Branch("tau_dxy", tau_dxy, "tau_dxy[ntau]/F" );
  BabyTree_->Branch("tau_dz", tau_dz, "tau_dz[ntau]/F" );
  BabyTree_->Branch("tau_isoMVA2", tau_isoMVA2, "tau_isoMVA2[ntau]/F" );
  BabyTree_->Branch("tau_idCI3hit", tau_idCI3hit, "tau_idCI3hit[ntau]/I" );
  BabyTree_->Branch("tau_isoCI3hit", tau_isoCI3hit, "tau_isoCI3hit[ntau]/F" );
  BabyTree_->Branch("tau_idMVA2", tau_idMVA2, "tau_idMVA2[ntau]/I" );
  BabyTree_->Branch("tau_mcMatchId", tau_mcMatchId, "tau_mcMatchId[ntau]/I" );
  BabyTree_->Branch("ngamma", &ngamma, "ngamma/I" );
  BabyTree_->Branch("gamma_pt", gamma_pt, "gamma_pt[ngamma]/F" );
  BabyTree_->Branch("gamma_eta", gamma_eta, "gamma_eta[ngamma]/F" );
  BabyTree_->Branch("gamma_phi", gamma_phi, "gamma_phi[ngamma]/F" );
  BabyTree_->Branch("gamma_mass", gamma_mass, "gamma_mass[ngamma]/F" );
  BabyTree_->Branch("gamma_mcMatchId", gamma_mcMatchId, "gamma_eta[ngamma]/I" );
  BabyTree_->Branch("gamma_chHadIso", gamma_chHadIso, "gamma_chHadIso[ngamma]/F" );
  BabyTree_->Branch("gamma_neuHadIso", gamma_neuHadIso, "gamma_neuHadIso[ngamma]/F" );
  BabyTree_->Branch("gamma_phIso", gamma_phIso, "gamma_phIso[ngamma]/F" );
  BabyTree_->Branch("gamma_sigmaIetaIeta", gamma_sigmaIetaIeta, "gamma_sigmaIetaIeta[ngamma]/F" );
  BabyTree_->Branch("gamma_r9", gamma_r9, "gamma_r9[ngamma]/F" );
  BabyTree_->Branch("gamma_hOverE", gamma_hOverE, "gamma_hOverE[ngamma]/F" );
  BabyTree_->Branch("gamma_idCutBased", gamma_idCutBased, "gamma_idCutBased[ngamma]/I" );
  BabyTree_->Branch("ngenPart", &ngenPart, "ngenPart/I" );
  BabyTree_->Branch("genPart_pt", genPart_pt, "genPart_pt[ngenPart]/F" );
  BabyTree_->Branch("genPart_eta", genPart_eta, "genPart_eta[ngenPart]/F" );
  BabyTree_->Branch("genPart_phi", genPart_phi, "genPart_phi[ngenPart]/F" );
  BabyTree_->Branch("genPart_mass", genPart_mass, "genPart_mass[ngenPart]/F" );
  BabyTree_->Branch("genPart_pdgId", genPart_pdgId, "genPart_pdgId[ngenPart]/I" );
  BabyTree_->Branch("genPart_charge", genPart_charge, "genPart_charge[ngenPart]/F" );
  BabyTree_->Branch("genPart_motherId", genPart_motherId, "genPart_motherId[ngenPart]/I" );
  BabyTree_->Branch("genPart_grandmaId", genPart_grandmaId, "genPart_grandmaId[ngenPart]/I" );
  BabyTree_->Branch("njet", &njet, "njet/I" );
  BabyTree_->Branch("jet_pt", jet_pt, "jet_pt[njet]/F" );
  BabyTree_->Branch("jet_eta", jet_eta, "jet_eta[njet]/F" );
  BabyTree_->Branch("jet_phi", jet_phi, "jet_phi[njet]/F" );
  BabyTree_->Branch("jet_mass", jet_mass, "jet_mass[njet]/F" );
  BabyTree_->Branch("jet_btagCSV", jet_btagCSV, "jet_btagCSV[njet]/F" );
  BabyTree_->Branch("jet_rawPt", jet_rawPt, "jet_rawPt[njet]/F" );
  BabyTree_->Branch("jet_mcPt", jet_mcPt, "jet_mcPt[njet]/F" );
  BabyTree_->Branch("jet_mcFlavour", jet_mcFlavour, "jet_mcFlavour[njet]/I" );
  BabyTree_->Branch("jet_quarkGluonID", jet_quarkGluonID, "jet_quarkGluonID[njet]/F" );
  BabyTree_->Branch("jet_area", jet_area, "jet_area[njet]/F" );
  BabyTree_->Branch("jet_id", jet_id, "jet_id[njet]/I" );
  BabyTree_->Branch("jet_puId", jet_puId, "jet_puId[njet]/I" );

  return;
}

void babyMaker::InitBabyNtuple () {

  run    = -999;
  lumi   = -999;
  evt    = -999;
  isData = -999;
  nevents = 0;
  scale1fb = 0;
  crossSection = -999.0;
  puWeight = -999.0;
  nVert = -999;
  nTrueInt = -999;
  rho = -999.0;
  rho25 = -999.0;
  nJet40 = -999;
  nBJet40 = -999;
  nMuons10 = -999;
  nElectrons10 = -999;
  nTaus20 = -999;
  nGammas20 = -999;
  deltaPhiMin = -999.0;
  diffMetMht = -999.0;
  ht = -999.0;
  mt2 = -999.0;
  mt2_gen = -999.0;
  pseudoJet1_pt = 0.0;
  pseudoJet1_eta = 0.0;
  pseudoJet1_phi = 0.0;
  pseudoJet1_mass = 0.0;
  pseudoJet2_pt = 0.0;
  pseudoJet2_eta = 0.0;
  pseudoJet2_phi = 0.0;
  pseudoJet2_mass = 0.0;
  mht_pt = -999.0;
  mht_phi = -999.0;
  met_pt = -999.0;
  met_phi = -999.0;
  met_rawPt = -999.0;
  met_rawPhi = -999.0;
  met_caloPt = -999.0;
  met_caloPhi = -999.0;
  met_genPt = -999.0;
  met_genPhi = -999.0;
  HLT_HT650 = -999;
  HLT_MET150 = -999;
  HLT_ht350met100 = -999;
  nlep = -999;
  nisoTrack = -999;
  ntau = -999;
  ngamma = -999;
  ngenPart = -999;
  njet = -999;

  
  return;
}

void babyMaker::FillBabyNtuple(){
  BabyTree_->Fill();
  return;
}

void babyMaker::CloseBabyNtuple(){
  BabyFile_->cd();
  BabyTree_->Write();
  BabyFile_->Close();
  return;
}
