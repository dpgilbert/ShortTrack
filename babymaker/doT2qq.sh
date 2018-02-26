#!/bin/bash

make -j 8 || return $?

INDIR=/hadoop/cms/store/user/mmasciov/AutoTwopler/SMS-T2qq_mSq1850to2300-Spring16Fast-v3/crab_SMS-T2qq_mSq1850to2300-Spring16Fast-v3/180222_002648/0000/
for ((i=1;i<=38;i++));
do
    if [ $i = 18 ] || [ $i = 25 ] || [ $i = 30 ]
    then
	nohup ./processBaby $i ${INDIR}ntuple_${i}.root >& logs/T2qq_${i}.txt &
    fi
done