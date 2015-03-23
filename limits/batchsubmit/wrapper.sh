#!/bin/bash

echo "host: " 
hostname

HOME=`pwd`

#This stuff to get it to run
export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc481
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.18/bin/thisroot.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export PATH=$PATH:.

pwd
pushd .
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_1_5/src/
pwd
eval `scramv1 runtime -sh`
popd
pwd

tar -xzvf input.tar.gz > /dev/null

ls -lrth

CARD=$1
SEED=$2
NTOYS=$3
NITERS=$4
POINT=$5
COPYDIR=$6

echo "Start Time is `date`"

./combine -M GenerateOnly ${CARD} -t -1 --expectSignal 0 --saveToys -s 12345
./combine -M HybridNew --frequentist ${CARD} --saveToys --fullBToys --saveHybridResult --singlePoint ${POINT} -T ${NTOYS} -i ${NITERS} -s ${SEED} --clsAcc 0 -v -1 --toysFile higgsCombineTest.GenerateOnly.mH120.12345.root -t -1

echo "Finish Time is `date`"

ls -lrth

rm ./rstats*
rm ./T*.root
rm ./combine
rm ./libHiggsAnalysisCombinedLimit.so
rm ./input.tar.gz

ls -lrth

lcg-cp -b -D srmv2 --vo cms -t 2400 --verbose file:`pwd`/higgsCombineTest.HybridNew.mH120.${SEED}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/higgsCombineTest.HybridNew.mH120.${SEED}.root
