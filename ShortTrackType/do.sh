#!/bin/bash

make all

INDIR=/nfs-6/userdata/dpgilber/MT2_ST_Friends_incl_skim
OUTDIR=output
LOGDIR=logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(ttsl_fromT ttsl_fromTbar wjets_incl zinv_zpt100to200 zinv_zpt200toInf qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000_ext1 qcd_ht2000toInf ttdl dyjetstoll_incl_ext1)

for SAMPLE in ${Samples[@]}; do
    echo "nohup nice -n 10 ./ShortTrackType.exe ${INDIR} ${SAMPLE}_incl_skim ${OUTDIR} >& ${LOGDIR}/${SAMPLE}.log &"
    eval "nohup nice -n 10 ./ShortTrackType.exe ${INDIR} ${SAMPLE}_incl_skim ${OUTDIR} >& ${LOGDIR}/${SAMPLE}.log &"
done