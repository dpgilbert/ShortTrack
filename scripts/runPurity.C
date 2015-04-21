//compiles and executes purity.C
void runPurity(string input_dir = "/home/users/gzevi/MT2/MT2Analysis/MT2looper/output/V00-00-11skim/")
{
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/SR_cc.so");
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/sigSelections_cc.so");
  gROOT->ProcessLine(".L $CMSSW_BASE/../scripts/purity.C++");
  purity(input_dir); 
}
