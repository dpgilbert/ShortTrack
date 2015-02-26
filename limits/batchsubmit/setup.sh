#!/bin/bash

export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc481
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.18/bin/thisroot.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export PATH=$PATH:.

pushd .
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_1_5/src/
eval `scramv1 runtime -sh`
popd
