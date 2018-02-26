#!/bin/bash

make -j 8 || return $?

INDIR=/hadoop/cms/store/user/mmasciov/AutoTwopler/SMS-T2bb_mSbot-1650to2300_mLSP-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-T2bb_mSbot-1650to2300_mLSP-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSp/180223_072806/0000/
for ((i=1;i<=5;i++));
do
    if [ $i = 4 ]
	then
	nohup ./processBaby $i ${INDIR}ntuple_${i}.root >& logs/T2bb_${i}.txt &
    fi
done