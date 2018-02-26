#!/bin/bash

make -j 12 || return $?

OUTDIR=output/T1qqqq_full
LOGDIR=logs/
mkdir -p ${OUTDIR}
INDIR=/nfs-6/userdata/mt2/V00-08-18_mc_skim_base_mt2gt200_ZinvV6_JECs/
eval "nohup nice -n 10 ./runLooper ${INDIR} T1qqqq ${OUTDIR} >& ${LOGDIR}/log_T1qqqq_full.txt &"
