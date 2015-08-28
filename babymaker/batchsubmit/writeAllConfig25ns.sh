#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-01-03_25ns"

#
# DATA
#

./writeConfig.sh /hadoop/cms/store/user/cgeorge/dataTuple/Run2015C_JetHT_MINIAOD_PromptReco-v1/V07-04-08/ ${TAG}_data_Run2015C_JetHT_PromptReco
./writeConfig.sh /hadoop/cms/store/user/jgran/dataTuple/Run2015C_MET_MINIAOD_PromptReco-v1/V07-04-08/ ${TAG}_data_Run2015C_MET_PromptReco
./writeConfig.sh /hadoop/cms/store/user/cgeorge/dataTuple/Run2015C_HTMHT_MINIAOD_PromptReco-v1/V07-04-08/ ${TAG}_data_Run2015C_HTMHT_PromptReco

# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_JetHT_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_JetHT_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_MET_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_MET_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_HTMHT_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_HTMHT_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_SingleMuon_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_SingleMuon_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_DoubleMuon_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_DoubleMuon_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_DoubleEG_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_DoubleEG_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_MuonEG_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_MuonEG_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_SingleElectron_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_SingleElectron_PromptReco
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015C_SinglePhoton_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015C_SinglePhoton_PromptReco

#
# TTBAR
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/ ${TAG}_ttall_mg_lo

#
# ALTERNATIVE TTBAR
#

## add also madgraph sample split by lepton decay

#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_ttall_amcatnlo
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/TT_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/ ${TAG}_ttall_powheg_p8

#
# W+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_wjets_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_wjets_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_wjets_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/V07-04-03/ ${TAG}_wjets_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_wjets_ht600toInf


#
# SINGLE TOP
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_singletop_powheg_4f_ttchan_l
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_singletop_powheg_4f_tbartchan_l
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_singletop_powheg_5f_tWchan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_singletop_powheg_5f_tbarWchan


#
# DY+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/V07-04-03/ ${TAG}_zjets_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/CMS3_V07-04-07/ ${TAG}_dyjetsll_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-07 ${TAG}_dyjetsll_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-07 ${TAG}_dyjetsll_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-07 ${TAG}_dyjetsll_ht600toInf

#
# GAMMA + JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-07/ ${TAG}_gjet_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-07 ${TAG}_gjet_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_gjet_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_gjet_ht600toInf

#
# Z INVISIBLE
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ZJetsToNuNu_HT-100To200_13TeV-madgraph_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-07/ ${TAG}_zinv_ht100To200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ZJetsToNuNu_HT-200To400_13TeV-madgraph_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-07/ ${TAG}_zinv_ht200To400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ZJetsToNuNu_HT-400To600_13TeV-madgraph_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-07/ ${TAG}_zinv_ht400To600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-07/ ${TAG}_zinv_ht600ToInf

#
# DIBOSON
#

#
# TRIBOSON
#

#
# TTV
#


#
# QCD
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt120to170
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/ ${TAG}_qcd_pt170to300
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt300to470
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/ ${TAG}_qcd_pt470to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/V07-04-07/ ${TAG}_qcd_pt600to800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/ ${TAG}_qcd_pt800to1000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt1000to1400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt1400to1800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt1800to2400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt2400to3200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/ ${TAG}_qcd_pt3200toInf

#
# SIGNAL
#



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
