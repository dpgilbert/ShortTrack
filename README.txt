# setup:

# environment on UAF:
export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh

git clone git@github.com:cmstas/MT2Analysis.git
cd MT2Analysis
git checkout cmssw80x
cmsrel CMSSW_8_0_5
cd CMSSW_8_0_5/src
cmsenv
cd ../..
git clone git@github.com:cmstas/CORE.git
cd babymaker
make -j 8

# to run:
cd babymaker
source doTest.sh

# Laptop setup to run on MT2babies
(this assumes you already have root setup)

git clone git@github.com:cmstas/MT2Analysis.git
cd MT2Analysis/MT2looper
make

# to run, modify INDIR and Samples in do.sh, then do:
source do.sh
