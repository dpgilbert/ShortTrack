#!/bin/bash

#INDIR=/nfs-6/userdata/mt2/V00-00-05/
#INDIR=/home/users/gzevi/MT2/MT2Analysis/babymaker/batchsubmit/merged/skim_gammaOrTwoLep
INDIR=/nfs-6/userdata/gzevi/mt2/ZllStudiesBabiesV00-00-05/skim_gammaOrTwoLep
OUTDIR=output/test/

declare -a Samples=(dyjetsll_ht gjet_ht)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1bbbb_1500_100 T1bbbb_1000_900)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\)
  nohup root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\) >& log_${SAMPLE}.txt &
done