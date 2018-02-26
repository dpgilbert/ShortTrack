#!/bin/bash

make -j 8 || return $?

INDIR=/hadoop/cms/store/user/mmasciov/AutoTwopler/SMS-T2qq_mSq-1850to2300_mLSP-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-T2qq_mSq-1850to2300_mLSP-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpri/180223_223250/0000/
for ((i=1;i<=3;i++));
do
    nohup ./processBaby $i ${INDIR}ntuple_${i}.root >& logs/T2qq_central_${i}.txt &
done