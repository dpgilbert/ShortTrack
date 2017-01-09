#!/bin/bash

#
# parse command line options
#
OPTIONS=""
while getopts "bc:hm:n:rt:w" opt; do
    case "$opt" in
        b) OPTIONS+="-b "
           ;;
        h) OPTIONS+="-h "
           ;;
        j) OPTIONS+="-j "
           ;;
        r) OPTIONS+="-r "
           ;;
        w) OPTIONS+="-w "
           ;;
        c) OPTIONS+="-c ${OPTARG}"
           ;;
        m) OPTIONS+="-m ${OPTARG}"
           ;;
        n) OPTIONS+="-n ${OPTARG}"
           ;;
        t) OPTIONS+="-t ${OPTARG}"
           ;;
        esac
done

shift $((OPTIND-1))

#
# args
#
INDIR=${@: -3:1}
FILEID=${@: -2:1}
COPYDIR=${@: -1:1}

echo "[wrapper] INDIR    = " ${INDIR}
echo "[wrapper] FILEID     = " ${FILEID}
echo "[wrapper] COPYDIR   = " ${COPYDIR}
echo "[wrapper] OPTIONS   = " ${OPTIONS}


#
# set up environment
#
CMSSW_VERSION=CMSSW_8_0_5

###version using cvmfs install of CMSSW
echo "[wrapper] setting env"
export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh
OLDDIR=`pwd`
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
#cmsenv
eval `scramv1 runtime -sh`
cd $OLDDIR

# ### version using nfs install of CMSSW
# echo "[wrapper] setting env"
# export SCRAM_ARCH=slc6_amd64_gcc493
# source /nfs-7/cmssoft/cms.cern.ch/cmssw/cmsset_default.sh
# OLDDIR=`pwd`
# cd /nfs-7/cmssoft/cms.cern.ch/cmssw/slc6_amd64_gcc493/cms/cmssw-patch/$CMSSW_VERSION/src
# #cmsenv
# eval `scramv1 runtime -sh`
# cd $OLDDIR

echo

echo "[wrapper] printing env"
printenv
echo 

echo "[wrapper] hostname  = " `hostname`
echo "[wrapper] date      = " `date`
echo "[wrapper] linux timestamp = " `date +%s`
echo "[wrapper] checking input file with ls"

IFS=','
read -ra FILES <<< "${FILEID}"
for file in "${FILES[@]}";
do
    ls -alrth ${INDIR}/${file}.root    

    # catch exit code
    if [ $? -ne 0 ]; then
        echo "[wrapper] could not find input file, trying xrootd instead"
        FILESHORT=${INDIR#/hadoop/cms}/${file}.root
        xrdfs xrootd.t2.ucsd.edu ls ${FILESHORT}
        if [ $? -ne 0 ]; then
	    echo "[wrapper] could not find input file with xrootd either, exiting"
	    exit 1
        else
	    echo "[wrapper] found file with xrootd, will proceed"
	    FILE="root://xrootd.t2.ucsd.edu/"${FILESHORT}
        fi
    fi
done
unset IFS

#
# untar input sandbox
#

echo "[wrapper] extracting input sandbox"
tar -zxf input.tar.gz

#source job_input/setupenv.sh
#printenv

#cd job_input
echo "[wrapper] input contents are"
ls -a

echo "[wrapper] directory contents are"
ls

#
# run it
#
echo "[wrapper] running: ./runLooper ${INDIR} ${FILEID} . ${OPTIONS}"

./runLooper ${INDIR} ${FILEID} . ${OPTIONS}

#
# do something with output
#

echo "[wrapper] output is"
ls

#
# clean up
#


echo "[wrapper] copying files"
OUTPUT=($(ls | grep ${FILES[0]}))
echo "[wrapper] OUTPUT = " ${OUTPUT}

have_baby=0
for outfile in "${OUTPUT[@]}";
do
    if [[ ${outfile} =~ .*baby.* ]]
    then
        have_baby=1
    fi   
done

if [ ! -d "${COPYDIR}" ]; then
    echo "creating output directory " ${COPYDIR}
    mkdir -p ${COPYDIR}
    if [[ ${have_baby} -eq 1 ]]
    then
        echo "creating output directory ${COPYDIR}/smearbaby"
        mkdir -p ${COPYDIR}/smearbaby
    fi
fi

for outfile in "${OUTPUT[@]}";
do
    if [[ ${outfile} =~ .*baby.* ]]
    then
        gfal-copy -p -f -t 4200 --verbose file:`pwd`/${outfile} srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/smearbaby/${outfile}
    else
        gfal-copy -p -f -t 4200 --verbose file:`pwd`/${outfile} srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${outfile}        
    fi
done
               
echo "[wrapper] cleaning up"
for FILE in `find . -not -name "*stderr" -not -name "*stdout"`; do rm -rf $FILE; done
echo "[wrapper] cleaned up"
ls
