#!/bin/bash

VERSION=$1
SAMPLE=$2
NJOBS=$3
TOYSPERJOB=$4
ITERPERJOB=$5
GRIDPOINT=$6
SEEDBEGIN=$7

while  ! voms-proxy-info -exist
do echo "No Proxy found issuing \"voms-proxy-init -voms cms\""
   voms-proxy-init -hours 168 -voms cms 
done

UNIVERSE="grid"
EXE="wrapper.sh"
INPUT="wrapper.sh, job_input/input.tar.gz"
SITE="T2_US_UCSD,T2_US_Nebraska,T2_US_Wisconsin,T2_US_MIT,T2_US_FLORIDA"
SUBMITLOGDIR="${PWD}/submit_logs"
JOBLOGDIR="${PWD}/job_logs"
PROXY=$(voms-proxy-info -path)
USERNAME=$(whoami)

LOGDIR=${PWD}/submit_logs
OUTDIR=${PWD}/job_logs  
LOG="${LOGDIR}/condor_`date "+%m_%d_%Y"`.log"
OUT="${OUTDIR}/1e.\$(Cluster).\$(Process).out"
ERR="${OUTDIR}/1e.\$(Cluster).\$(Process).err"

if [ ! -d "${LOGDIR}" ]; then
    echo "[writeConfig] creating log directory " ${LOGDIR}
    mkdir -p ${LOGDIR}
fi

if [ ! -d "${OUTDIR}" ]; then
    echo "[writeConfig] creating job output log directory " ${OUT}
    mkdir -p ${OUT}
fi

OUTPUTDIR=/hadoop/cms/store/user/${USERNAME}/combine/limits/${VERSION}/${SAMPLE}
echo "[writeConfig] running on dataset ${DATADIR}"
echo "[writeConfig] copying output to ${COPYDIR}"

if [ ! -d "${OUTPUTDIR}" ]; then
    echo "[writeConfig] creating job output directory " ${COPYDIR}
    mkdir -p ${OUTPUTDIR}
fi

Grid_Resource="condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu"
echo "
universe=${UNIVERSE}
Grid_Resource=${Grid_Resource}
when_to_transfer_output = ON_EXIT
#the actual executable to run is not transfered by its name.
#In fact, some sites may do weird things like renaming it and such.
transfer_input_files=${INPUT}
transfer_output_files = /dev/null
+DESIRED_Sites=\"${SITE}\"
+Owner = undefined
log=${LOG}
output=${OUT}
error =${ERR}
notification=Never
x509userproxy=${PROXY}
" > condor_${SAMPLE}_${SEEDBEGIN}.cmd

    #
    # now set the rest of the arguments 
    # for each job
    # 

    for i in $(seq 1 ${NJOBS}); do 
        echo "
        executable=${EXE}
        transfer_executable=True
        arguments=${SAMPLE}.root $((${SEEDBEGIN}+$i)) ${TOYSPERJOB} ${ITERPERJOB} ${GRIDPOINT} ${OUTPUTDIR}
        queue
        " >> condor_${SAMPLE}_${SEEDBEGIN}.cmd
    done

echo "[writeConfig] wrote condor_${SAMPLE}_${SEEDBEGIN}.cmd"
