#include "sampleID.h"

#include <iostream>

using namespace std;

//_____________________________________________________
int sampleID (const string& name) {

// sample ID numbers according to:
//  https://mangano.web.cern.ch/mangano/public/MECCA/samples_50ns_miniaod.txt

  if (name.find("QCD") != string::npos) {
    if ((name.find("QCD_Pt-15to3000") != string::npos) || (name.find("QCD_Pt_15to3000") != string::npos)) return 101;
    else if ((name.find("QCD_Pt-5to10") != string::npos) || (name.find("QCD_Pt_5to10") != string::npos)) return 102;
    else if ((name.find("QCD_Pt-10to15") != string::npos) || (name.find("QCD_Pt_10to15") != string::npos)) return 103;
    else if ((name.find("QCD_Pt-15to30") != string::npos) || (name.find("QCD_Pt_15to30") != string::npos)) return 104;
    else if ((name.find("QCD_Pt-30to50") != string::npos) || (name.find("QCD_Pt_30to50") != string::npos)) return 105;
    else if ((name.find("QCD_Pt-50to80") != string::npos) || (name.find("QCD_Pt_50to80") != string::npos)) return 106;
    else if ((name.find("QCD_Pt-80to120") != string::npos) || (name.find("QCD_Pt_80to120") != string::npos)) return 107;
    else if ((name.find("QCD_Pt-120to170") != string::npos) || (name.find("QCD_Pt_120to170") != string::npos)) return 108;
    else if ((name.find("QCD_Pt-170to300") != string::npos) || (name.find("QCD_Pt_170to300") != string::npos)) return 109;
    else if ((name.find("QCD_Pt-300to470") != string::npos) || (name.find("QCD_Pt_300to470") != string::npos)) return 110;
    else if ((name.find("QCD_Pt-470to600") != string::npos) || (name.find("QCD_Pt_470to600") != string::npos)) return 111;
    else if ((name.find("QCD_Pt-600to800") != string::npos) || (name.find("QCD_Pt_600to800") != string::npos)) return 112;
    else if ((name.find("QCD_Pt-800to1000") != string::npos) || (name.find("QCD_Pt_800to1000") != string::npos)) return 113;
    else if ((name.find("QCD_Pt-1000to1400") != string::npos) || (name.find("QCD_Pt_1000to1400") != string::npos)) return 114;
    else if ((name.find("QCD_Pt-1400to1800") != string::npos) || (name.find("QCD_Pt_1400to1800") != string::npos)) return 115;
    else if ((name.find("QCD_Pt-1800") != string::npos) || (name.find("QCD_Pt_1800") != string::npos)) return 116;
    else if ((name.find("QCD_Pt-2400to3200") != string::npos) || (name.find("QCD_Pt_2400to3200") != string::npos)) return 117;
    else if ((name.find("QCD_Pt-3200") != string::npos) || (name.find("QCD_Pt_3200") != string::npos)) return 118;

    else if ((name.find("QCD_HT_250To500") != string::npos) && (name.find("ext1") != string::npos)) return 162;
    else if ((name.find("QCD_HT_500To1000") != string::npos) && (name.find("ext1") != string::npos)) return 163;
    else if ((name.find("QCD_HT_1000") != string::npos) && (name.find("ext1") != string::npos)) return 164;

    else if (name.find("QCD_HT-100To250") != string::npos) return 151;
    else if (name.find("QCD_HT_250To500") != string::npos) return 152;
    else if (name.find("QCD_HT-500To1000") != string::npos) return 153;
    else if (name.find("QCD_HT_1000") != string::npos) return 154;
  }

  else if (name.find("GJet") != string::npos) {
    if (name.find("GJet_Pt-15to3000") != string::npos) return 200;
    else if ((name.find("GJets_HT-100to200") != string::npos) || (name.find("GJets_HT-100To200") != string::npos)) return 202;
    else if ((name.find("GJets_HT-200to400") != string::npos) || (name.find("GJets_HT-200To400") != string::npos)) return 203;
    else if ((name.find("GJets_HT-400to600") != string::npos) || (name.find("GJets_HT-400To600") != string::npos)) return 204;
    else if ((name.find("GJets_HT-600toInf") != string::npos) || (name.find("GJets_HT-600ToInf") != string::npos)) return 205;
  }

  else if (name.find("TTJets") != string::npos) {
    if (name.find("TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola") != string::npos) return 300;
  }

  // 400 block: single top, ttV
  else if (name.find("TToLeptons_s-channel") != string::npos) return 400;
  else if (name.find("TToLeptons_t-channel") != string::npos) return 401;
  else if (name.find("T_tW-channel") != string::npos) return 402;
  else if (name.find("TBarToLeptons_s-channel") != string::npos) return 403;
  else if (name.find("TBarToLeptons_t-channel") != string::npos) return 404;
  else if (name.find("Tbar_tW-channel") != string::npos) return 405;
  else if (name.find("TTWJets") != string::npos) return 410;
  else if (name.find("TTZJets") != string::npos) return 412;
  else if (name.find("TTbarH") != string::npos) return 412;

  else if (name.find("WJets") != string::npos) {
    if ((name.find("WJetsToLNu_HT-100to200") != string::npos) || (name.find("WJetsToLNu_HT-100To200") != string::npos)) return 502;
    else if ((name.find("WJetsToLNu_HT-200to400") != string::npos) || (name.find("WJetsToLNu_HT-200To400") != string::npos)) return 503;
    else if ((name.find("WJetsToLNu_HT-400to600") != string::npos) || (name.find("WJetsToLNu_HT-400To600") != string::npos)) return 503;
    else if ((name.find("WJetsToLNu_HT-600toInf") != string::npos) || (name.find("WJetsToLNu_HT-600ToInf") != string::npos)) return 503;
  }

  else if (name.find("ZJets") != string::npos) {
    if ((name.find("ZJetsToNuNu_HT-100to200") != string::npos) || (name.find("ZJetsToNuNu_HT-100To200") != string::npos)) return 602;
    else if ((name.find("ZJetsToNuNu_HT-200to400") != string::npos) || (name.find("ZJetsToNuNu_HT-200To400") != string::npos)) return 603;
    else if ((name.find("ZJetsToNuNu_HT-400to600") != string::npos) || (name.find("ZJetsToNuNu_HT-400To600") != string::npos)) return 603;
    else if ((name.find("ZJetsToNuNu_HT-600toInf") != string::npos) || (name.find("ZJetsToNuNu_HT-600ToInf") != string::npos)) return 603;
  }

  else if (name.find("DYJets") != string::npos) {
    if ((name.find("DYJetsToLL_M-50_HT-100to200")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-100To200")  != string::npos)) return 702;
    else if ((name.find("DYJetsToLL_M-50_HT-200to400")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-200To400") != string::npos)) return 703;
    else if ((name.find("DYJetsToLL_M-50_HT-400to600")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-400To600") != string::npos)) return 704;
    else if ((name.find("DYJetsToLL_M-50_HT-600toInf")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-600ToInf") != string::npos)) return 705;
  }

  // 800 block: diboson etc

  else if (name.find("SMS-") != string::npos) {
    if (name.find("SMS-T1tttt_2J_mGl-1500_mLSP-100") != string::npos) return 1001;
    else if (name.find("SMS-T1tttt_2J_mGl-1200_mLSP-800") != string::npos) return 1002;
    else if (name.find("SMS-T1qqqq_2J_mGl-1400_mLSP-100") != string::npos) return 1010;
    else if (name.find("SMS-T1qqqq_2J_mGl-1000_mLSP-800") != string::npos) return 1011;
    else if (name.find("SMS-T1bbbb_2J_mGl-1000_mLSP-900") != string::npos) return 1020;
    else if (name.find("SMS-T1bbbb_2J_mGl-1500_mLSP-100") != string::npos) return 1021;

    else if (name.find("SMS-T2tt_2J_mStop-425_mLSP-325") != string::npos) return 1101;
    else if (name.find("SMS-T2tt_2J_mStop-500_mLSP-325") != string::npos) return 1102;
    else if (name.find("SMS-T2tt_2J_mStop-650_mLSP-325") != string::npos) return 1103;
    else if (name.find("SMS-T2tt_2J_mStop-850_mLSP-100") != string::npos) return 1104;
    else if (name.find("SMS-T2qq_2J_mStop-600_mLSP-550") != string::npos) return 1110;
    else if (name.find("SMS-T2qq_2J_mStop-1200_mLSP-100") != string::npos) return 1111;
    else if (name.find("SMS-T2bb_2J_mStop-600_mLSP-580") != string::npos) return 1120;
    else if (name.find("SMS-T2bb_2J_mStop-900_mLSP-100") != string::npos) return 1121;
  }

  cout << "sampleID: WARNING: sample not recognized!  " << name << endl;
  return -1;
}
