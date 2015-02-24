#!/bin/bash

#INDIR=/nfs-6/userdata/mt2/V00-00-09_lepisostudy2/
INDIR=/nfs-6/userdata/mt2/V00-00-09_lepisostudy3_skim_nj2_ht450_mt2gt200_Zinv/
OUTDIR=output/V00-00-09_lepisostudy3_SRsel_reco/

#declare -a Samples=(wjets_ht)
declare -a Samples=(ttall_msdecays wjets_ht T1tttt_1500_100 T1bbbb_1500_100)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\)
  nohup root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\) >& log_${SAMPLE}.txt &
done
