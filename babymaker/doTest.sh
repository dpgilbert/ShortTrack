#!/bin/bash

make -j 8 || return $?

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

#./processBaby test_ttbar_80X /nfs-7/userdata/bemarsh/scratch/TTJets_SingleLeptFromT_25ns_80X_All.root

#./processBaby T2tt1 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_1.root
#./processBaby T2tt2 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_2.root
#./processBaby T2tt3 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_3.root
#./processBaby T2tt4 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_4.root
#./processBaby T2tt5 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_5.root
#./processBaby T2tt6 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_6.root
#./processBaby T2tt7 /hadoop/cms/store/user/namin/AutoTwopler/SMS-T2tt_mStop1600-Spring16Fast-v3/crab_SMS-T2tt_mStop1600-Spring16Fast-v3/180131_173119/0000/ntuple_7.root

#INDIR=/hadoop/cms/store/user/mmasciov/AutoTwopler/SMS-T2tt_mStop-1200to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-T2tt_mStop-1200to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16F/180220_040634/0000/

#for ((i=2;i<=28;i++));
#do
#    if [ $i != 5 && $i != 14 ]
#    then
#	nohup ./processBaby 1200to1500_$i ${INDIR}ntuple_${i}.root >& logs/1200to1500_${i}.txt &
#    fi
#done

#INDIR=/hadoop/cms/store/user/mmasciov/AutoTwopler/SMS-T2tt_mStop-1550to2300_mLSP-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-T2tt_mStop-1550to2300_mLSP-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSp/180220_040531/0000/

#for ((i=1;i<=7;i++));
#do
#    nohup ./processBaby 1550to2300_$i ${INDIR}ntuple_${i}.root >& logs/1550to2300_${i}.txt &
#done

INDIR=/hadoop/cms/store/user/mmasciov/AutoTwopler/SMS-T2bb_mSb1650to2300-Spring16Fast-v3/crab_SMS-T2bb_mSb1650to2300-Spring16Fast-v3/180220_235631/0000/
for ((i=1;i<=94;i++));
do
    if [ $i != 5 ]
    then
	nohup ./processBaby 1650to2300_$i ${INDIR}ntuple_${i}.root >& logs/1650to2300_${i}.txt &
    fi
done