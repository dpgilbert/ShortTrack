#!/bin/bash

make -j 12 || return $?

OUTDIR=output/prep2017
LOGDIR=logs/
mkdir -p $OUTDIR
mkdir -p $LOGDIR

INDIR=/nfs-6/userdata/dpgilber/ShortTrackFromMario
eval "nohup nice -n 10 ./runLooper ${INDIR} T5qqqqLLChiChi_2016_friends ${OUTDIR} >& ${LOGDIR}/log_T5qqqq.txt &"
