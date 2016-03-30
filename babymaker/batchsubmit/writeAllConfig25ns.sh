#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="76x_miniaodv2_test_v4"

#
# DATA
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/DoubleEG_Run2015C_25ns-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015C_DoubleEG
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/DoubleEG_Run2015D-16Dec2015-v2/CMS3_V07-06-03/ ${TAG}_data_Run2015D_DoubleEG
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/DoubleMuon_Run2015C_25ns-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015C_DoubleMuon
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/DoubleMuon_Run2015D-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015D_DoubleMuon
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/MuonEG_Run2015C_25ns-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015C_MuonEG
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/MuonEG_Run2015D-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015D_MuonEG
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/SingleElectron_Run2015C_25ns-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015C_SingleElectron
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/SingleElectron_Run2015D-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015D_SingleElectron
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/SingleMuon_Run2015C_25ns-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015C_SingleMuon
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DATA/SingleMuon_Run2015D-16Dec2015-v1/CMS3_V07-06-03/ ${TAG}_data_Run2015D_SingleMuon

#
# TTBAR
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_ttdl_mg_lo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_ttsl_mg_lo_top
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v2/V07-06-03_MC/ ${TAG}_ttsl_mg_lo_tbar
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_ttjets_mg_lo
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext4-v1/V07-06-03_MC/ ${TAG}_tt_powheg
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TT/TTTo2L2Nu_13TeV-powheg/v1/CMS3_V07-06-03 ${TAG}_tt2l2nu_powheg

#
# HIGH STATS TTBAR EXTENSION
#


#
# W+JETS
#

#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_BGenFilter_Wpt-40toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_bgen_wpt40toInf
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht1200to2500
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht2500toInf
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht600to800
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht600toInf
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_ht800to1200
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_wjets_mg_lo

#
# SINGLE TOP
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/T/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/CMS3_V07-06-03/ ${TAG}_singletop_powheg_5f_tWchan
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/T/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/CMS3_V07-06-03/ ${TAG}_singletop_powheg_5f_tbarWchan

#
# DY+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_ht600toInf
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjets_m50_mg_lo
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_m5to50_ht100to200
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_m5to50_ht200to400
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_m5to50_ht400to600
#./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_dyjetsll_m5to50_ht600toInf

#
# GAMMA + JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_gjets_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_gjets_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_gjets_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v3/V07-06-03_MC/ ${TAG}_gjets_ht40to100
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_gjets_ht600toInf


#
# Z INVISIBLE
#

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/ZJetsToNuNu_HT-100To200_13TeV-madgraph_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_zinv_ht100to200
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/ZJetsToNuNu_HT-200To400_13TeV-madgraph_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_zinv_ht200to400
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/ZJetsToNuNu_HT-400To600_13TeV-madgraph_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_zinv_ht400to600
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/ ${TAG}_zinv_ht600toInf

# #
# # DIBOSON
# #

# #
# # TRIBOSON
# #

# #
# # TTV
# #

./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTV/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/CMS3_V07-06-03 ${TAG}_ttg_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTV/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/CMS3_V07-06-03 ${TAG}_ttw_lnu_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTV/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/CMS3_V07-06-03 ${TAG}_ttw_qq_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTV/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/CMS3_V07-06-03 ${TAG}_ttz_llnunu_amcatnlo
./writeConfig.sh /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TTV/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/CMS3_V07-06-03 ${TAG}_ttz_qq_amcatnlo

# #
# # QCD
# #


# #
# # SIGNAL
# #


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
