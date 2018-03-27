#!/bin/bash

make -j 12 all || return $?

OUTDIR=output/prep2017
#OUTDIR=output/debug
LOGDIR=logs/
mkdir -p $OUTDIR
mkdir -p $LOGDIR

#Signal MC
#INDIR=/nfs-6/userdata/dpgilber/ShortTrackFromMario
#eval "nohup nice -n 10 ./runLooper ${INDIR} T5qqqqLLChiChi_2016_friends ${OUTDIR} >& ${LOGDIR}/log_T5qqqq.txt &"

#Background MC
INDIR=/nfs-6/userdata/dpgilber/MT2_ST_Friends
#declare -a Samples=(qcd_ht2000toInf)
#declare -a Samples=(qcd_ht1000to1500  qcd_ht1500to2000_ext1 qcd_ht2000toInf qcd_ht300to500 qcd_ht700to1000 qcd_ht500to700 ttsl_fromT ttsl_fromTbar ttdl wjets_incl zinv_zpt100to200 zinv_zpt200toInf)
declare -a Samples=(dyjetstoll_incl_ext1)
for SAMPLE in ${Samples[@]}; do
    echo "nohup nice -n 1 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
    eval "nohup nice -n 1 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done