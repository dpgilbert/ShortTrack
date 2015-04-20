//compiles and executes purity.C
void runPurity()
{
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/SR_cc.so");
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/sigSelections_cc.so");
  gROOT->ProcessLine(".x $CMSSW_BASE/../scripts/purity.C++");
}
