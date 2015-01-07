#include "sampleID.h"

#include <iostream>

using namespace std;

//_____________________________________________________
int sampleID (const string& name) {

// sample ID numbers according to:
//  https://mangano.web.cern.ch/mangano/public/MECCA/samples_50ns_miniaod.txt

  if (name.find("QCD") != string::npos) {
    if (name.find("QCD_Pt-15to3000") != string::npos) return 101;
    else if (name.find("QCD_Pt-5to10") != string::npos) return 102;
    else if (name.find("QCD_Pt-10to15") != string::npos) return 103;
    else if (name.find("QCD_Pt-15to30") != string::npos) return 104;
    else if (name.find("QCD_Pt-30to50") != string::npos) return 105;
    else if (name.find("QCD_Pt-50to80") != string::npos) return 106;
    else if (name.find("QCD_Pt-80to120") != string::npos) return 107;
    else if (name.find("QCD_Pt-120to170") != string::npos) return 108;
    else if (name.find("QCD_Pt-170to300") != string::npos) return 109;
    else if (name.find("QCD_Pt-300to470") != string::npos) return 110;
    else if (name.find("QCD_Pt-470to600") != string::npos) return 111;
    else if (name.find("QCD_Pt-600to800") != string::npos) return 112;
    else if (name.find("QCD_Pt-800to1000") != string::npos) return 113;
    else if (name.find("QCD_Pt-1000to1400") != string::npos) return 114;
    else if (name.find("QCD_Pt-1400to1800") != string::npos) return 115;
    else if (name.find("QCD_Pt-1800") != string::npos) return 116;
    else if (name.find("QCD_Pt-2400to3200") != string::npos) return 117;
    else if (name.find("QCD_Pt-3200") != string::npos) return 118;

    else if (name.find("QCD_HT-100To250") != string::npos) return 120;
    else if (name.find("QCD_HT_250To500") != string::npos) return 121;
    else if (name.find("QCD_HT-500To1000") != string::npos) return 122;
    else if (name.find("QCD_HT_1000") != string::npos) return 123;
  }

  else if (name.find("GJet") != string::npos) {
    if (name.find("GJet_Pt-15to3000") != string::npos) return 200;
    else if (name.find("GJets_HT-100to200") != string::npos) return 202;
    else if (name.find("GJets_HT-200to400") != string::npos) return 203;
    else if (name.find("GJets_HT-400to600") != string::npos) return 204;
    else if (name.find("GJets_HT-600toInf") != string::npos) return 205;
  }

  else if (name.find("TTJets") != string::npos) {
    if (name.find("TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola") != string::npos) return 300;
  }

  // 400 block: single top

  else if (name.find("WJets") != string::npos) {
    if (name.find("WJetsToLNu_HT-100to200") != string::npos) return 502;
    else if (name.find("WJetsToLNu_HT-200to400") != string::npos) return 503;
    else if (name.find("WJetsToLNu_HT-400to600") != string::npos) return 504;
    else if (name.find("WJetsToLNu_HT-600toInf") != string::npos) return 505;
  }

  else if (name.find("ZJets") != string::npos) {
    if (name.find("ZJetsToNuNu_HT-100to200") != string::npos) return 602;
    else if (name.find("ZJetsToNuNu_HT-200to400") != string::npos) return 603;
    else if (name.find("ZJetsToNuNu_HT-400to600") != string::npos) return 604;
    else if (name.find("ZJetsToNuNu_HT-600toInf") != string::npos) return 605;
  }

  else if (name.find("DYJets") != string::npos) {
    if (name.find("DYJetsToLL_M-50_HT-100to200") != string::npos) return 702;
    else if (name.find("DYJetsToLL_M-50_HT-200to400") != string::npos) return 703;
    else if (name.find("DYJetsToLL_M-50_HT-400to600") != string::npos) return 704;
    else if (name.find("DYJetsToLL_M-50_HT-600toInf") != string::npos) return 705;
  }

  // 800 block: diboson etc

  else if (name.find("SMS-") != string::npos) {
    if (name.find("SMS-T1tttt_2J_mGl-1500_mLSP-100") != string::npos) return 1001;
    else if (name.find("SMS-T1tttt_2J_mGl-1200_mLSP-800") != string::npos) return 1002;
    else if (name.find("SMS-T1qqqq_2J_mGl-1400_mLSP-100") != string::npos) return 1010;
    else if (name.find("SMS-T1bbbb_2J_mGl-1000_mLSP-900") != string::npos) return 1020;
    else if (name.find("SMS-T1bbbb_2J_mGl-1500_mLSP-100") != string::npos) return 1021;
  }

  cout << "sampleID: WARNING: sample not recognized!  " << name << endl;
  return -1;
}
