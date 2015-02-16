#!/bin/bash

make -j12

INDIR=/nfs-6/userdata/mt2/V00-00-08_skim_nj2_ht450_mt2gt200_Zinv/
OUTDIR=output/test/

declare -a Samples=(ttall_msdecays wjets_ht zinv_ht dyjetsll_ht qcd_ht gjet_ht)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800 T2tt_850_100 T2tt_500_325 T2bb_900_100 T2bb_600_580 T2qq_1200_100 T2qq_600_550)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\)
  nohup root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\) >& log_${SAMPLE}.txt &
done
