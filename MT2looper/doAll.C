{

  gSystem->Load("libMiniFWLite.so");
  gSystem->Load("libMT2CORE.so");
  gSystem->Load("libMT2Looper.so");

  TChain *ch = new TChain("treeProducerSusyFullHad"); 
 
  //CMS3 ntuples made from  
  // /hadoop/cms/store/user/dalfonso/MINIAOD/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_PU20bx25.1.root
  // /hadoop/cms/store/user/dalfonso/MINIAOD/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_PU20bx25.2.root 
  //ch->Add("/nfs-3/userdata/jgran/MT2Sync/ntuple1.root");
  //ch->Add("/nfs-3/userdata/jgran/MT2Sync/ntuple2.root");
  //  ch->Add("/home/users/gzevi/miniAOD/CMSSW_7_0_6/src/CMS2/NtupleMaker/test/MT2ntuple1000.root");
  ch->Add("sntMT2Baby.root");
  //ch->Add("/home/users/jgran/CMSSW_7_0_6_patch1/src/CMS2/NtupleMaker/ntuple_first1000.root");

  MT2Looper *looper = new MT2Looper();
  looper->loop(ch, "plots"); 
}
