#!/bin/bash

INDIR=/nfs-6/userdata/mt2/V00-00-12_root6
OUTDIR=output/test_root6/

#declare -a Samples=(ttall_msdecays wjets_ht zinv_ht dyjetsll_ht qcd_pt)
declare -a Samples=(T1tttt_1500_100)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1bbbb_1500_100 T1bbbb_1000_900)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done