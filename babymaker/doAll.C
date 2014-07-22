{

  gROOT->ProcessLine(".L ../MT2CORE/libMT2CORE.so");
  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("Events"); 
 
  //CMS3 ntuples made from  
  // /hadoop/cms/store/user/dalfonso/MINIAOD/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_PU20bx25.1.root
  // /hadoop/cms/store/user/dalfonso/MINIAOD/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_PU20bx25.2.root 
  //ch->Add("/nfs-3/userdata/jgran/MT2Sync/ntuple1.root");
  //ch->Add("/nfs-3/userdata/jgran/MT2Sync/ntuple2.root");
  //ch->Add("/home/users/gzevi/miniAOD/CMSSW_7_0_6/src/CMS2/NtupleMaker/test/MT2ntuple1000.root");
  ch->Add("/home/users/jgran/CMSSW_7_0_6_patch1/src/CMS2/NtupleMaker/ntuple_first1000.root");

  babyMaker *looper = new babyMaker();
  looper->ScanChain(ch, "sntMT2Baby"); 
}
