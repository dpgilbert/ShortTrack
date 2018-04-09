#!/bin/bash

make all

INDIR=/nfs-6/userdata/dpgilber/MT2_ST_Friends
LOGDIR=logs

mkdir -p ${LOGDIR}

declare -a Samples=(ttdl)

for SAMPLE in ${Samples[@]}; do
    echo "nohup nice -n 10 ./ToyLooper.exe ${INDIR} ${SAMPLE} >& ${LOGDIR}/${SAMPLE}.log &"
    eval "nohup nice -n 10 ./ToyLooper.exe ${INDIR} ${SAMPLE} >& ${LOGDIR}/${SAMPLE}.log &"
done
