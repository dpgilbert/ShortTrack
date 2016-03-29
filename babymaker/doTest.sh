#!/bin/bash

make -j 8

#./processBaby data_json_test /home/users/jgran/temp/74x_sync/CMSSW_7_4_3/src/CMS3/NtupleMaker/ntuple_data.root
#./processBaby data_test  /nfs-7/userdata/jgran/74x_sync/ntuple_data.root
#./processBaby data_json_fixrho  /home/users/jgran/temp/electronID/CMSSW_7_4_1_patch1/src/CMS3/NtupleMaker/ntuple_rereco.root
#./processBaby qcd_CMS3-07-04-03_test  /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/merged_ntuple_31.root
#./processBaby qcd_test_noJEC_fixjetID ./ntuple_QCD.root
#./processBaby qcd_test_noJEC_fixjetID_fixgamma /home/users/gzevi/ntupling/CMSSW_7_4_1_patch1/src/CMS3/NtupleMaker/ntupleQCD_Pt_1000to1400_10000.root

##### Run2 datasets for sync - note that these do NOT have the postprocessing vars
#./processBaby test_ttbar_1k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_ttbar_25ns_1k.root
#./processBaby test_ttbar_10k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_ttbar_25ns_10k.root
#./processBaby test_data_JetHT_1k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_data_Run2015D_JetHT_PromptReco-v4_1k.root
#./processBaby test_data_JetHT_10k /nfs-7/userdata/olivito/mt2/run2_25ns_MiniAODv2/ntuple_data_Run2015D_JetHT_PromptReco-v4_10k.root

#./processBaby test_T2tt /hadoop/cms/store/group/snt/run2_fastsim_v1/SMS-T2tt_mStop-600-950_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9-v1/V07-04-12_miniaodv1_FS/merged_ntuple_1.root

#./processBaby test_ttbar_76X /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/TT/TTTo2L2Nu_13TeV-powheg/v1/CMS3_V07-06-03/merged_ntuple_1.root
#./processBaby test_ttbar_76X /home/users/gzevi/ntupling/CMSSW_7_6_3/src/CMS3/NtupleMaker/test/TTJets_SingleLeptFromT_25ns_76X_1000.root 
./processBaby test_wjets_76X /hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/V07-06-03_MC/merged_ntuple_1.root

