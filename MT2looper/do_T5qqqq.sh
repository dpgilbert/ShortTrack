#!/bin/bash

make -j 12 || return $?

OUTDIR=output/full2016
LOGDIR=logs/

INDIR=/nfs-6/userdata/dpgilber/ShortTrackFromMario/
eval "nohup nice -n 10 ./runLooper ${INDIR} T5qqqqLLChiChi_2016_baby ${OUTDIR} >& ${LOGDIR}/log_T5qqqq.txt &"
