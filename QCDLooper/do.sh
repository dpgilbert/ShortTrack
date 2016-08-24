#!/bin/bash

make -j 8

# INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-08-04_MiniAODv2_nojson
INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-08-08_nojson
# INDIR=/nfs-6/userdata/mt2/rphi_skim_V00-01-09_25ns
OUTDIR=output/V00-08-08_nojson_12p9fb

LOGDIR=logs

# declare -a Samples=(qcd_ht ttsl ttdl singletop ttw ttz ttg dyjetsll_ht wjets_ht gjets_dr0p05_ht)
declare -a Samples=(qcd_ht)  
# declare -a Samples=(data_Run2016)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done
