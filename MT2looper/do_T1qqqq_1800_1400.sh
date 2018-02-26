#!/bin/bash

make -j 12 || return $?

OUTDIR=output/full2016
LOGDIR=logs/
mkdir -p ${OUTDIR}
INDIR=/nfs-6/userdata/dpgilber/
eval "nohup nice -n 10 ./runLooper ${INDIR} T1qqqq_1800_1400 ${OUTDIR} >& ${LOGDIR}/log_T1qqqq_1800_1400.txt &"
