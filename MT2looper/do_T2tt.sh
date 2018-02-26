#!/bin/bash

make -j 12 || return $?

OUTDIR=output/full2016
LOGDIR=logs/

INDIR=/nfs-6/userdata/dpgilber/T2tt-Spring16Fast
eval "nohup nice -n 10 ./runLooper ${INDIR} T2tt_baby_full ${OUTDIR} >& ${LOGDIR}/log_T2tt_baby_full.txt &"
