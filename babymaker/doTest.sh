#!/bin/bash

#./processBaby data_json_test /home/users/jgran/temp/74x_sync/CMSSW_7_4_3/src/CMS3/NtupleMaker/ntuple_data.root
#./processBaby data_test  /nfs-7/userdata/jgran/74x_sync/ntuple_data.root
#./processBaby data_json_fixrho  /home/users/jgran/temp/electronID/CMSSW_7_4_1_patch1/src/CMS3/NtupleMaker/ntuple_rereco.root
#./processBaby qcd_CMS3-07-04-03_test  /hadoop/cms/store/group/snt/run2_25ns/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/merged_ntuple_31.root
#./processBaby qcd_test_noJEC_fixjetID ./ntuple_QCD.root
#./processBaby qcd_test_noJEC_fixjetID_fixgamma /home/users/gzevi/ntupling/CMSSW_7_4_1_patch1/src/CMS3/NtupleMaker/ntupleQCD_Pt_1000to1400_10000.root

##### Run2 datasets for sync - note that these do NOT have the postprocessing vars
./processBaby sync_ttbar_1k /nfs-7/userdata/olivito/mt2/run2/ntuple_ttbar_50ns_1k.root
#./processBaby sync_ttbar_10k /nfs-7/userdata/olivito/mt2/run2/ntuple_ttbar_50ns_10k.root
#./processBaby sync_data_JetHT_1k /nfs-7/userdata/olivito/mt2/run2/ntuple_data_Run2015B_JetHT_1k.root
#./processBaby sync_data_JetHT_10k /nfs-7/userdata/olivito/mt2/run2/ntuple_data_Run2015B_JetHT_10k.root
