//compiles and executes purity.C
void runPurity()
{
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/SR.cc++");
  gROOT->ProcessLine(".L $CMSSW_BASE/../MT2CORE/sigSelections.cc++");
  gROOT->ProcessLine(".x $CMSSW_BASE/../scripts/purity.C++");
}
