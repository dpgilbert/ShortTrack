#!/bin/bash

THISDIR=`pwd`
cd ..
tar -czf batchsubmit/job_input/package.tar.gz *.so LinkDef*.pcm jetCorrections jsons btagsf lepsf data processBaby skim_macro.C skim_rphi_macro.C
cd $THISDIR
cp wrapper_auto.sh sweeproot.sh sweeproot_macro.C merge_script.sh merge_macro.C job_input/
