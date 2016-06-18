#!/bin/bash

make -j 8

INDIR=/nfs-6/userdata/mt2/rphi_skims
OUTDIR=output/test/

#declare -a Samples=(qcd_ht ttsl ttdl dyjetsll_incl wjets_ht zinv_ht data_Run2016B)
declare -a Samples=(qcd_ht data_Run2016B)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
