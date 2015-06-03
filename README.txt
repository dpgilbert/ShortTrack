# setup:

# environment on UAF:
export SCRAM_ARCH=slc6_amd64_gcc491
source /nfs-7/cmssoft/cms.cern.ch/cmssw/cmsset_default.sh

git clone git@github.com:cmstas/MT2Analysis.git
cd MT2Analysis
cmsrel CMSSW_7_4_1_patch1
cd CMSSW_7_4_1_patch1/src
cmsenv
cd ../..
git clone git@github.com:cmstas/CORE.git
cd CORE
git checkout cmssw74x
cd ..
git clone git@github.com:cmstas/Tools.git
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
