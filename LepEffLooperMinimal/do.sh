#!/bin/bash

make -j 8

INDIR=/nfs-6/userdata/mt2/V00-08-06_nojson_nolepjetcleaning_noiso/
OUTDIR=output/V00-08-06_nojson_nolepjetcleaning_pfreliso01_allgenleps

declare -a Samples=(ttsl_mg_lo )
#declare -a Samples=(ttall_mg_lo dyjetsll_ht)
#declare -a Samples=(wjets_ht)
#declare -a Samples=(ttall_msdecays wjets_ht T1tttt_1500_100 T1bbbb_1500_100)
#declare -a Samples=(T1tttt_1500_100)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
