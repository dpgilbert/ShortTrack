#!/bin/bash

make -j 8 || return $?

INDIR=/hadoop/cms/store/user/dpgilber/AutoTwopler/ZJetsToNuNu_Zpt-200toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_ZJetsToNuNu_Zpt-200toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_/180312_171920/0000/merged
for ((i=1;i<=14;i++));
do
    nohup ./processBaby zinv_zpt200toInf_$i ${INDIR}/merged_ntuple_${i}.root >& logs/zinv_zpt200toInf_${i}.txt &
done