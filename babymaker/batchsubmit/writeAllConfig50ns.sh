#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-01-02_50ns"

#
# DATA
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_JetHT_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_JetHT
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_MET_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_MET
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_HTMHT_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_HTMHT
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_SingleMuon_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_SingleMuon
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_DoubleMuon_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_DoubleMuon
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_DoubleEG_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_DoubleEG
./writeConfig.sh /hadoop/cms/store/group/snt/run2_data/Run2015B_MuonEG_MINIAOD_PromptReco-v1/merged/V07-04-04/ ${TAG}_data_Run2015B_MuonEG

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

### need to sort through/name these

# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ 
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ 
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/ 
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/ 
# ./writeConfig.sh /hadoop/cms/store/group/snt/run2_50ns/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/V07-04-03/ 


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
