#!/bin/bash

make -j 8

INDIR=/nfs-6/userdata/mt2/V00-01-03_50ns_jecv4_fixhbhe_t1met_v2_skim_trig
OUTDIR=output/test/

declare -a Samples=(data_Run2015B wjets_amcatnlo ttall_mg_lo )
#declare -a Samples=(singletop_powheg)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
