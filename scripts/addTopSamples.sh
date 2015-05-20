#!/bin/bash

INDIR=/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

if [ -e "$INDIR/top.root" ]; then
  echo "top.root already exists, will delete it and make a new one." 
  rm $INDIR/top.root
fi

pushd .
cd $INDIR
hadd top.root ttall_msdecays.root tth.root ttzjets.root ttwjets.root singletop.root
popd

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
root -b -q "rescaleBoundaryHists.C(\"${INDIR}\",5)";
