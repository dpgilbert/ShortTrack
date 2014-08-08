{

  //  gSystem->Load("libMiniFWLite.so");
  gSystem->Load("libMT2CORE.so");
  gSystem->Load("libMT2Looper.so");

  TChain *ch = new TChain("treeProducerSusyFullHad"); 
 
  ch->Add("/nfs-6/userdata/mt2/V00-00-01/ttall_msdecays.root");

  MT2Looper *looper = new MT2Looper();
  looper->loop(ch, "plots"); 
}
