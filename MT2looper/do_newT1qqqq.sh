#!/bin/bash

make -j 12 || return $?

OUTDIR=output/sanity
LOGDIR=logs/
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

#signal
INDIR=/home/users/olivito/mt2_80x_mc/MT2Analysis/babymaker/skimmed_T1qqqq
declare -a Samples=(T1qqqq)

for SAMPLE in ${Samples[@]}; do
    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

