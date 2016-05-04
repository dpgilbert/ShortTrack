#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="80x_miniaodv1_test_v1"

#
# DATA
#



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
# need 600 to 800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01/ ${TAG}_wjets_ht800to1200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/V08-00-01/ ${TAG}_wjets_ht1200to2500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01/ ${TAG}_wjets_ht2500toInf

#
# SINGLE TOP
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_amcatnlo_4f_schan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_powheg_5f_tWchan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_singletop_powheg_5f_tbarWchan

#
# DY+JETS
#


#
# GAMMA + JETS
#


#
# Z INVISIBLE
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-600To800_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht600to800
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht800to1200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht1200to2500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_zinv_ht2500toInf

# #
# # DIBOSON
# #

# #
# # TRIBOSON
# #

# #
# # TTV
# #

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttw_lnu_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttw_qq_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttz_qq_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_ttg_amcatnlo

# #
# # QCD
# #

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht300to500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht700to1000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht1000to1500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht1500to2000
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_qcd_ht2000toInf

# #
# # SIGNAL
# #
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/SMS-T1bbbb_mGluino-1000_mLSP-900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_T1bbbb_1000_900
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/SMS-T1bbbb_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/V08-00-01 ${TAG}_T1bbbb_1500_100

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
