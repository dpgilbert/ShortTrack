#!/bin/bash

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

OUTPUTDIR=/hadoop/cms/store/user/${USERNAME}/combine/limits/${VERSION}/${SAMPLE}

Grid_Resource="condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu"

#Run condor_q to get list of running jobs
condor_q $USER > temp_status.txt
sed -i '1,4d' temp_status.txt
sed -i '$d' temp_status.txt
sed -i '$d' temp_status.txt

rm heldList.txt 2>/dev/null

#Read condor_q output to fill lists.  
while read line
do
  if [ `echo $line | awk '{ print $6 }'` == "H" ]
  then
    echo `echo $line | awk '{ print $1 }'` >> heldList.txt
  fi
done < temp_status.txt

rm temp_status.txt

if [ -e heldList.txt ]
then
  while read line
  do
    ARGS=`condor_q $line -l | grep Args`
    ARGS=`echo $ARGS | sed 's/Args = "//g'`
    ARGS=`echo $ARGS | sed 's/"//g'`
    ARG1=`echo $ARGS | awk '{print $1}'` 
    ARG2=`echo $ARGS | awk '{print $2}'` 
    ARG3=`echo $ARGS | awk '{print $3}'` 
    ARG4=`echo $ARGS | awk '{print $4}'` 
    ARG5=`echo $ARGS | awk '{print $5}'` 
    ARG6=`echo $ARGS | awk '{print $6}'` 
    SAMPLE=`echo $ARG1 | sed 's/.root//g'`
    echo "
    universe=${UNIVERSE}
    Grid_Resource=${Grid_Resource}
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
    executable=${EXE}
    transfer_executable=True
    arguments=$ARG1 $ARG2 $ARG3 $ARG4 $ARG5 $ARG6
    queue
    " > condor_${SAMPLE}_${ARG2}.cmd
    condor_submit condor_${SAMPLE}_${ARG2}.cmd
  done < heldList.txt
fi

Delete held jobs
if [ -e heldList.txt ]
then
  while read line
  do
    condor_rm $line
  done < heldList.txt
fi
