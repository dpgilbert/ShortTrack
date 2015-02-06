# setup:

# environment on UAF:
export SCRAM_ARCH=slc6_amd64_gcc481
source /cvmfs/cms.cern.ch/cmsset_default.sh

git clone git@github.com:cmstas/MT2Analysis.git
cd MT2Analysis
cmsrel CMSSW_7_0_6_patch1
cd CMSSW_7_0_6_patch1/src
cmsenv
cd ../..
git clone git@github.com:cmstas/CORE.git
git clone git@github.com:cmstas/Tools.git
git clone git@github.com:cmstas/Software.git
cd babymaker
make

# to run:
cd babymaker
root -b doAll.C

# Laptop setup to run on MT2babies
(this assumes you alread have root setup)

git clone git@github.com:cmstas/MT2Analysis.git
cd MT2Analysis/MT2looper
make

# to run, modify INDIR and Samples in do.sh, then do:
source do.sh
