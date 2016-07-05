#!/bin/bash

make -j 8

INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-08-02_nojson
# INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-01-09_25ns
OUTDIR=output/V00-08-02_nojson_3p99fb_RA2clean

LOGDIR=logs

declare -a Samples=(qcd_ht ttsl ttdl singletop ttw ttz ttg dyjetsll_ht wjets_ht gjets_dr0p05_ht data_Run2016B)
# declare -a Samples=(zinv_ht)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done
