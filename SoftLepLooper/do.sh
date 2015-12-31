#!/bin/bash

make -j12

INDIR=/nfs-6/userdata/mt2/V00-01-07_25ns_miniaodv2_softLepSR
OUTDIR=output/softLepSR/

# INDIR=/nfs-6/userdata/mt2/V00-01-07_25ns_miniaodv2_softLepCR
# OUTDIR=output/softLepCR/

#declare -a Samples=(data_Run2015C data_Run2015D ttsl_mg_lo ttdl_mg_lo wjets_ht zinv_ht singletop qcd_ht ttw ttz ttg tth dyjetsll_ht gjet_ht)
declare -a Samples=(wjets_ht zinv_ht singletop ttdl ttsl ttg tth ttw ttz dyjetsll)
#declare -a Samples=(ttdl)

mkdir -p ${OUTDIR}
mkdir -p logs

for SAMPLE in ${Samples[@]};
  do echo nice -n 19 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup nice -n 19 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& logs/${SAMPLE}.log &
done

