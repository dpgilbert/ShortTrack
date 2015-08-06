#!/bin/bash

make -j 8

INDIR=/nfs-6/userdata/mt2/V00-01-02_50ns_goldenjsonv2_reminiaod_met3p0_skim_trig
OUTDIR=output/test/

declare -a Samples=(data_Run2015B zjets_amcatnlo ttall_mg_lo)
#declare -a Samples=(zjets_amcatnlo ttall_mg_lo)
#declare -a Samples=(data_Run2015B)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
