#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-08-02_json_Cert_271036-274421"

#
# DATA
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_JetHT_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_JetHT_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_MET_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_MET_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_HTMHT_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_HTMHT_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_SingleElectron_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_SingleElectron_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_SingleMuon_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_SingleMuon_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_SinglePhoton_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_SinglePhoton_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_DoubleEG_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_DoubleEG_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_DoubleMuon_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_DoubleMuon_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2016B_MuonEG_MINIAOD_PromptReco-v2/merged/V08-00-04 ${TAG}_data_Run2016B_MuonEG_PromptReco

#
# TTBAR
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttdl_mg_lo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttsl_mg_lo_top
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttsl_mg_lo_tbar

#
# HIGH STATS TTBAR EXTENSION
#


#
# W+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v2/V08-00-01/ ${TAG}_wjets_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/V08-00-01/ ${TAG}_wjets_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/V08-00-01/ ${TAG}_wjets_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/V08-00-01/ ${TAG}_wjets_ht600to800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01/ ${TAG}_wjets_ht800to1200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/V08-00-01/ ${TAG}_wjets_ht1200to2500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01/ ${TAG}_wjets_ht2500toInf

#
# SINGLE TOP
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_amcatnlo_4f_schan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_powheg_5f_tWchan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_powheg_5f_tbarWchan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_powheg_4f_tbartchan_l

#
# DY+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_dyjetsll_ht100to200 
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_dyjetsll_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_dyjetsll_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_dyjetsll_ht600toInf ## miniaod v2..

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01/ ${TAG}_dyjetsll_incl

#
# GAMMA + JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/GJets_DR-0p4_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_gjets_ht40to100  
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/V08-00-01 ${TAG}_gjets_ht100to200 
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_gjets_ht200to400 
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_gjets_ht400to600 
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_gjets_ht600toInf  

### all miniaodv2
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_gjets_dr0p05_ht40to100  
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v4/V08-00-05/ ${TAG}_gjets_dr0p05_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_gjets_dr0p05_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_gjets_dr0p05_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_gjets_dr0p05_ht600toInf


#
# Z INVISIBLE
#

# missing 100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-200To400_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01/  ${TAG}_zinv_ht200to400
# missing 400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-600To800_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht600to800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht800to1200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht1200to2500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht2500toInf

#
# DIBOSON
#

#
# TRIBOSON
#

#
# TTV
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttw_lnu_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttw_qq_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttz_qq_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttg_amcatnlo

#
# QCD
#

# ./writeConfig.sh ${TAG}_qcd_ht200to300
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht300to500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht500to700
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht700to1000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht1000to1500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht1500to2000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht2000toInf

#
# QCD Extension
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_ht200to300_ext
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_ht300to500_ext
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_ht500to700_ext
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_ht700to1000_ext
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_ht1000to1500_ext
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01/ ${TAG}_qcd_ht1500to2000_ext
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v2/V08-00-01 ${TAG}_qcd_ht2000toInf_ext

#
# QCD PT bins
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt80to120
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt120to170
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt170to300
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt300to470
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt470to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt600to800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt800to1000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt1000to1400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt1400to1800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt1800to2400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt2400to3200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_pt3200toInf

#
# QCD PT bins extensions
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_pt470to600_ext1
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/V08-00-01 ${TAG}_qcd_pt600to800_ext1



#
# SIGNAL
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/SMS-T1bbbb_mGluino-1000_mLSP-900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_T1bbbb_1000_900
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/SMS-T1bbbb_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_T1bbbb_1500_100

#
# SIGNAL SCANS
#

### all miniaodv2
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2_fastsim/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_T1tttt
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2_fastsim/SMS-T2bb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/V08-00-05/ ${TAG}_T2bb



# --- write submit script ---
mkdir -p configs_${TAG}

mv condor_${TAG}*.cmd configs_${TAG}
echo "#!/bin/bash" > submitAll.sh
echo "voms-proxy-init -voms cms -valid 240:00" >> submitAll.sh
for file in configs_${TAG}/*.cmd
do 
    echo "condor_submit ${file}" >> submitAll.sh
done
chmod +x submitAll.sh
echo "[writeAllConfig] wrote submit script submitAll.sh"
