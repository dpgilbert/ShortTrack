#!/bin/bash

make -j 8 || return $?

INDIR=/hadoop/cms/store/user/dpgilber/AutoTwopler/ZJetsToNuNu_Zpt-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_ZJetsToNuNu_Zpt-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_/180312_171816/0000/merged
for ((i=1;i<=15;i++));
#for ((i=16;i<=30;i++));
do
    nohup ./processBaby zinv_Zpt100to200_$i ${INDIR}/merged_ntuple_${i}.root >& logs/zinv_Zpt100to200_${i}.txt &
done