#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-01-04_50ns"

#
# DATA
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_JetHT_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_JetHT_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_MET_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_MET_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_HTMHT_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_HTMHT_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SingleMuon_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_SingleMuon_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_DoubleMuon_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_DoubleMuon_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_DoubleEG_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_DoubleEG_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_MuonEG_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_MuonEG_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SingleElectron_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_SingleElectron_PromptReco
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SinglePhoton_MINIAOD_PromptReco-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_SinglePhoton_PromptReco

./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_JetHT_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_JetHT_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_MET_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_MET_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_HTMHT_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_HTMHT_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SingleMuon_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_SingleMuon_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_DoubleMuon_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_DoubleMuon_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_DoubleEG_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_DoubleEG_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_MuonEG_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_MuonEG_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SingleElectron_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_SingleElectron_17Jul2015
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SinglePhoton_MINIAOD_17Jul2015-v1/merged/V07-04-08/ ${TAG}_data_Run2015B_SinglePhoton_17Jul2015

#
# TTBAR
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ ${TAG}_ttall_mg_lo

#
# ALTERNATIVE TTBAR
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ ${TAG}_ttall_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/TT_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v4/V07-04-03 ${TAG}_ttall_powheg_p8

#
# W+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03_mergeV2/ ${TAG}_wjets_amcatnlo


#
# SINGLE TOP
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ ${TAG}_singletop_powheg_4f_ttchan_l
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ ${TAG}_singletop_powheg_4f_tbartchan_l
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/ ${TAG}_singletop_powheg_5f_tbarWchan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-07/ ${TAG}_singletop_powheg_5f_tWchan


#
# ALTERNATIVE SINGLE TOP
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ ${TAG}_singletop_amcatnlo_4f_tchan_l
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/ ${TAG}_singletop_amcatnlo_5f_tchan_l


#
# DY+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/ ${TAG}_zjets_amcatnlo


#
# GAMMA + JETS
#


#
# Z INVISIBLE
#

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

./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/  ${TAG}_qcd_pt80to120
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/  ${TAG}_qcd_pt120to170
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/  ${TAG}_qcd_pt170to300
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/  ${TAG}_qcd_pt300to470
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/  ${TAG}_qcd_pt470to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/  ${TAG}_qcd_pt600to800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/  ${TAG}_qcd_pt800to1000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/  ${TAG}_qcd_pt1000to1400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/  ${TAG}_qcd_pt1400to1800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/  ${TAG}_qcd_pt1800to2400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/  ${TAG}_qcd_pt2400to3200
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/  ${TAG}_qcd_pt3200

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
