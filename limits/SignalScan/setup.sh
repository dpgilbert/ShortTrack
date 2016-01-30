#!/bin/bash

#copy input files
cp /nfs-7/userdata/jgran/mt2_limit_input/job_input.tar.gz .
cp ../../babymaker/data/xsec_susy_13tev.root .

#checkout PlotsSMS package
git clone git@github.com:CMS-SUS-XPAG/PlotsSMS.git

#put modified sms.py and model config files into PlotsSMS area
cp sms.py PlotsSMS/python
mkdir -p PlotsSMS/config/mt2
cp *_mt2.cfg PlotsSMS/config/mt2

#set environment
source /nfs-7/cmssoft/cms.cern.ch/cmssw/cmsset_default.sh > /dev/null 2>&1
export SCRAM_ARCH=slc6_amd64_gcc491 > /dev/null
pushd /nfs-7/cmssoft/cms.cern.ch/cmssw/slc6_amd64_gcc491/cms/cmssw-patch/CMSSW_7_4_1_patch1 > /dev/null
eval `scramv1 runtime -sh`
popd > /dev/null
