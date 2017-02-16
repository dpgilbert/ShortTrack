#include "sampleID.h"

#include <iostream>

using namespace std;

//_____________________________________________________
int sampleID (const string& name) {

// sample ID numbers according to:
//  https://mangano.web.cern.ch/mangano/public/MECCA/samples_50ns_miniaod.txt

  if ((name.find("data_Run201") != string::npos) || (name.find("run2_data") != string::npos)) {
    if (name.find("JetHT") != string::npos) return 1;
    else if (name.find("HTMHT") != string::npos) return 2;
    else if (name.find("MET") != string::npos) return 3;
    else if (name.find("DoubleEG") != string::npos) return 4;
    else if (name.find("DoubleMuon") != string::npos) return 5;
    else if (name.find("MuonEG") != string::npos) return 6;
    else if (name.find("SinglePhoton") != string::npos) return 7;
    else if (name.find("SingleMuon") != string::npos) return 8;
    else if (name.find("SingleElectron") != string::npos) return 9;
  }
  
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

    else if (name.find("QCD_HT300To500_GenJets5") != string::npos) return 163;
    else if (name.find("QCD_HT500to700_GenJets5") != string::npos) return 164;
    else if (name.find("QCD_HT700to1000_GenJets5") != string::npos) return 165;
    else if (name.find("QCD_HT1000to1500_GenJets5") != string::npos) return 166;
    else if (name.find("QCD_HT1500to2000_GenJets5") != string::npos) return 167;
    else if (name.find("QCD_HT2000toInf_GenJets5") != string::npos) return 168;

    else if (name.find("QCD_HT-100to200") != string::npos) return 151;
    else if (name.find("QCD_HT_200to300") != string::npos) return 152;
    else if (name.find("QCD_HT-300to500") != string::npos) return 153;
    else if (name.find("QCD_HT-500to700") != string::npos) return 154;
    else if (name.find("QCD_HT-700to1000") != string::npos) return 155;
    else if (name.find("QCD_HT-1000to1500") != string::npos) return 156;
    else if (name.find("QCD_HT-1500to2000") != string::npos) return 157;
    else if (name.find("QCD_HT_2000toInf") != string::npos) return 158;

    else if (name.find("QCD_HT100to200") != string::npos) return 151;
    else if (name.find("QCD_HT200to300") != string::npos) return 152;
    else if (name.find("QCD_HT300to500") != string::npos) return 153;
    else if (name.find("QCD_HT500to700") != string::npos) return 154;
    else if (name.find("QCD_HT700to1000") != string::npos) return 155;
    else if (name.find("QCD_HT1000to1500") != string::npos) return 156;
    else if (name.find("QCD_HT1500to2000") != string::npos) return 157;
    else if (name.find("QCD_HT2000toInf") != string::npos) return 158;

  }

  else if (name.find("GJet") != string::npos) {
    if (name.find("GJet_Pt-15to3000") != string::npos) return 200;
    else if (name.find("GJets_DR-0p4_HT-40To100") != string::npos || name.find("GJets_HT-40To100") != string::npos) return 201;
    else if (name.find("GJets_DR-0p4_HT-100To200") != string::npos || name.find("GJets_HT-100To200") != string::npos) return 202;
    else if (name.find("GJets_DR-0p4_HT-200To400") != string::npos || name.find("GJets_HT-200To400") != string::npos) return 203;
    else if (name.find("GJets_DR-0p4_HT-400To600") != string::npos || name.find("GJets_HT-400To600") != string::npos) return 204;
    else if (name.find("GJets_DR-0p4_HT-600ToInf") != string::npos || name.find("GJets_HT-600ToInf") != string::npos) return 205;
  }

  else if (name.find("TTJets") != string::npos) {
    if (name.find("TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8") != string::npos) return 300;
    else if (name.find("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8") != string::npos) return 301;
    else if (name.find("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8") != string::npos) return 302;
    else if (name.find("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8") != string::npos) return 303;
    else if (name.find("TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8") != string::npos) return 306;
    
    else if (name.find("TTJets_HT-600to800") != string::npos) return 311;
    else if (name.find("TTJets_HT-800to1200") != string::npos) return 312;
    else if (name.find("TTJets_HT-1200to2500") != string::npos) return 313;
    else if (name.find("TTJets_HT-2500toInf") != string::npos) return 314;
  }

  // 400 block: single top, ttV
  else if (name.find("TToLeptons_s-channel") != string::npos) return 400;
  else if (name.find("TToLeptons_t-channel") != string::npos) return 401;
  else if (name.find("T_tW-channel") != string::npos) return 402;
  else if (name.find("TBarToLeptons_s-channel") != string::npos) return 403;
  else if (name.find("TBarToLeptons_t-channel") != string::npos) return 404;
  else if (name.find("Tbar_tW-channel") != string::npos) return 405;
  // amcatnlo
  else if (name.find("TTWToLNu") != string::npos) return 410; 
  else if (name.find("TTWToQQ") != string::npos) return 411; 
  else if (name.find("TTZToLLNuNu") != string::npos) return 412; 
  else if (name.find("TTZToQQ") != string::npos) return 413; 
  else if (name.find("TTGJets") != string::npos) return 414;
  // powheg
  else if (name.find("ttHTobb") != string::npos) return 415;
  else if (name.find("ttHToNonbb") != string::npos) return 416;
  // LO madgraph
  else if ((name.find("TTWJets") != string::npos) || (name.find("ttWJets") != string::npos)) return 450;
  else if ((name.find("TTZJets") != string::npos) || (name.find("ttZJets") != string::npos)) return 452;

  else if (name.find("WJets") != string::npos) {
    if ((name.find("WJetsToLNu_HT-100to200") != string::npos) || (name.find("WJetsToLNu_HT-100To200") != string::npos)) return 502;
    else if ((name.find("WJetsToLNu_HT-200to400") != string::npos) || (name.find("WJetsToLNu_HT-200To400") != string::npos)) return 503;
    else if ((name.find("WJetsToLNu_HT-400to600") != string::npos) || (name.find("WJetsToLNu_HT-400To600") != string::npos)) return 504;
    else if ((name.find("WJetsToLNu_HT-600toInf") != string::npos) || (name.find("WJetsToLNu_HT-600ToInf") != string::npos)) return 505;
    else if ((name.find("WJetsToLNu_HT-600to800") != string::npos) || (name.find("WJetsToLNu_HT-600To800") != string::npos)) return 505;
    else if ((name.find("WJetsToLNu_HT-800to1200") != string::npos) || (name.find("WJetsToLNu_HT-800To1200") != string::npos)) return 506;
    else if ((name.find("WJetsToLNu_HT-1200to2500") != string::npos) || (name.find("WJetsToLNu_HT-1200To2500") != string::npos)) return 507;
    else if ((name.find("WJetsToLNu_HT-2500toInf") != string::npos) || (name.find("WJetsToLNu_HT-2500ToInf") != string::npos)) return 508;
  }

  else if (name.find("ZJets") != string::npos) {
    if ((name.find("ZJetsToNuNu_HT-100to200") != string::npos) || (name.find("ZJetsToNuNu_HT-100To200") != string::npos)) return 602;
    else if ((name.find("ZJetsToNuNu_HT-200to400") != string::npos) || (name.find("ZJetsToNuNu_HT-200To400") != string::npos)) return 603;
    else if ((name.find("ZJetsToNuNu_HT-400to600") != string::npos) || (name.find("ZJetsToNuNu_HT-400To600") != string::npos)) return 604;
    else if ((name.find("ZJetsToNuNu_HT-600to800") != string::npos) || (name.find("ZJetsToNuNu_HT-600To800") != string::npos)) return 605;
    else if ((name.find("ZJetsToNuNu_HT-800to1200") != string::npos) || (name.find("ZJetsToNuNu_HT-800To1200") != string::npos)) return 606;
    else if ((name.find("ZJetsToNuNu_HT-1200to2500") != string::npos) || (name.find("ZJetsToNuNu_HT-1200To2500") != string::npos)) return 607;
    else if ((name.find("ZJetsToNuNu_HT-2500toInf") != string::npos) || (name.find("ZJetsToNuNu_HT-2500ToInf") != string::npos)) return 608;
  }

  else if (name.find("DYJets") != string::npos) {
    if (name.find("DYJetsToLL_M-50_TuneCUETP8M1") != string::npos) return 701;
    else if ((name.find("DYJetsToLL_M-50_HT-100to200")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-100To200") != string::npos)) return 702;
    else if ((name.find("DYJetsToLL_M-50_HT-200to400")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-200To400") != string::npos)) return 703;
    else if ((name.find("DYJetsToLL_M-50_HT-400to600")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-400To600") != string::npos)) return 704;
    else if ((name.find("DYJetsToLL_M-50_HT-600toInf")  != string::npos) || (name.find("DYJetsToLL_M-50_HT-600ToInf") != string::npos)) return 705;
  }

  // 800 block: diboson etc
  else if (name.find("VVTo2L2Nu") != string::npos) return 801;
  else if (name.find("WWTo2L2Nu") != string::npos) return 802;
  else if (name.find("WWToLNuQQ") != string::npos) return 803;
  else if (name.find("WZTo1L3Nu") != string::npos) return 804;
  else if (name.find("WZTo1L1Nu2Q") != string::npos) return 805;
  else if (name.find("WZTo2L2Q") != string::npos) return 806;
  else if (name.find("WZTo3LNu") != string::npos) return 807;
  else if (name.find("ZZTo2Q2Nu") != string::npos) return 808;
  else if (name.find("ZZTo2L2Q") != string::npos) return 809;
  else if (name.find("ZZTo2L2Nu") != string::npos) return 810;
  else if (name.find("ZZTo4L") != string::npos) return 811;
  
  // 900 block: triboson
  else if (name.find("WWW") != string::npos) return 901;
  else if (name.find("WWZ") != string::npos) return 902;
  else if (name.find("WZZ") != string::npos) return 903;
  else if (name.find("ZZZ") != string::npos) return 904;
  
  else if (name.find("SMS-") != string::npos) {
    if (name.find("SMS-T1tttt") != string::npos) return 1001;
    else if (name.find("SMS-T1qqqq") != string::npos) return 1010;
    else if (name.find("SMS-T1bbbb") != string::npos) return 1020;
    else if (name.find("SMS-T1ttbb") != string::npos) return 1030;
    else if (name.find("SMS-T5qqqqVV") != string::npos) return 1040;

    else if (name.find("SMS-T2tt") != string::npos) return 1101;
    else if (name.find("SMS-T2qq") != string::npos) return 1110;
    else if (name.find("SMS-T6qqWW") != string::npos) return 1115;
    else if (name.find("SMS-T2bb") != string::npos) return 1120;
    else if (name.find("SMS-T2cc") != string::npos) return 1130;
    else if (name.find("SMS-T2bW") != string::npos) return 1140;
    else if (name.find("SMS-T2bt") != string::npos) return 1150;
  }

  cout << "sampleID: WARNING: sample not recognized!  " << name << endl;
  return -1;
}
