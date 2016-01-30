#!/bin/bash

SAMPLE=$1

while  ! voms-proxy-info -exist
do echo "No Proxy found issuing \"voms-proxy-init -voms cms\""
   voms-proxy-init -hours 168 -voms cms 
done

MODEL=$(echo "$SAMPLE"|awk -F- 'split($1,a,"_")&&$0=a[1]')

INPUT="job_input.tar.gz, cards_$MODEL/cards_$SAMPLE.tar.gz"
SITE="T2_US_UCSD"
PROXY=$(voms-proxy-info -path)
SUBMITLOGDIR="${PWD}/submit_logs_$MODEL"
JOBLOGDIR="/data/tmp/jgran/job_logs/limits_for_paper_$MODEL"
JOBCFGDIR="${PWD}/job_cfg_$MODEL"
LOG="${SUBMITLOGDIR}/condor_submit.log"
OUT="${JOBLOGDIR}/1e.\$(Cluster).\$(Process).out"
ERR="${JOBLOGDIR}/1e.\$(Cluster).\$(Process).err"

OUTPUTDIR=/hadoop/cms/store/user/$USER/combine/limits_for_paper/$MODEL

if [ ! -d "${SUBMITLOGDIR}" ]; then
    mkdir -p ${SUBMITLOGDIR}
fi

if [ ! -d "${JOBLOGDIR}" ]; then
    mkdir -p ${JOBLOGDIR}
fi

if [ ! -d "${JOBCFGDIR}" ]; then
    mkdir -p ${JOBCFGDIR}
fi

echo "
universe=grid
Grid_Resource=condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu
when_to_transfer_output = ON_EXIT
transfer_input_files=${INPUT}
transfer_output_files = /dev/null
+DESIRED_Sites=\"${SITE}\"
+Owner = undefined
log=${LOG}
output=${OUT}
error =${ERR}
notification=Never
x509userproxy=${PROXY}
executable=condorExecutable.sh
transfer_executable=True
arguments=$SAMPLE $OUTPUTDIR 
queue
" > ${JOBCFGDIR}/condor_$SAMPLE.cmd

condor_submit ${JOBCFGDIR}/condor_$SAMPLE.cmd
