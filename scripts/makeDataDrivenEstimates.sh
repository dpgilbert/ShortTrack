#!/bin/bash

INDIR=/home/users/olivito/MT2Analysis/MT2looper/output/V00-00-12_binsHI/
THISDIR=`pwd`

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR
echo "hadd -f top.root ttall_msdecays.root tth.root ttzjets.root ttwjets.root singletop.root"
hadd -f top.root ttall_msdecays.root tth.root ttzjets.root ttwjets.root singletop.root

## make "lostlep" from sum of MC lostlep backgrounds (except QCD, to avoid spikes)
echo "hadd -f lostlep.root ttall_msdecays.root tth.root ttzjets.root ttwjets.root singletop.root wjets_ht.root"
hadd -f lostlep.root ttall_msdecays.root tth.root ttzjets.root ttwjets.root singletop.root wjets_ht.root
cd $THISDIR

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/top.root,5)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/top.root\",5)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/lostlep.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/lostlep.root\",6)"

# make the data driven background files
echo "root -b -q lostlepMaker.C+(${INDIR})"
root -b -q "lostlepMaker.C+(\"${INDIR}\")" > dataDrivenEstimates.log
echo "root -b -q ZinvMaker.C+(${INDIR})"
root -b -q "ZinvMaker.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
cd $INDIR
rm qcdplusgjet.root
echo "hadd qcdplusgjet.root gjet_ht.root qcd_pt.root" 
hadd qcdplusgjet.root gjet_ht.root qcd_pt.root  >> $THISDIR/dataDrivenEstimates.log
cd $THISDIR
echo "root -b -q runPurity.C(${INDIR})"
root -b -q "runPurity.C(\"${INDIR}\")" >> dataDrivenEstimates.log
echo "done"


