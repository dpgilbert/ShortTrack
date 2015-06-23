//compiles and executes purity.C
void runPurityRL(string input_dir = "/nfs-5/users/mderdzinski/spring2015/mt2/MT2Analysis/MT2looper/output/CRRLskims")
{
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/SR_cc.so");
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/sigSelections_cc.so");
  gROOT->ProcessLine(".L $CMSSW_BASE/../scripts/purityRL.C++");
  purityRL(input_dir); 
}
