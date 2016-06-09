#!/bin/bash

make -j 8

INDIR=/nfs-6/userdata/mt2/V00-08-01_json_Cert_271036-274240
OUTDIR=output/V00-08-01_json_Cert_271036-274240_emulooper_csv

declare -a Samples=(data_Run2016B_MuonEG ttdl_mg_lo dyjetsll_incl)
#declare -a Samples=(zjets_amcatnlo ttall_mg_lo)
#declare -a Samples=(data_Run2015B)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
