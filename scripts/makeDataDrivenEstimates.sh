#!/bin/bash

INDIR=/home/users/gzevi/MT2/MT2Analysis/MT2looper/output/V00-00-11skim/
THISDIR=`pwd`


echo "root -b -q lostlepMaker.C(${INDIR})"
root -b -q "lostlepMaker.C(\"${INDIR}\")" > dataDrivenEstimates.log
echo "root -b -q ZinvMaker.C(${INDIR})"
root -b -q "ZinvMaker.C(\"${INDIR}\")" >> dataDrivenEstimates.log
cd $INDIR
rm qcdplusgjet.root
echo "hadd qcdplusgjet.root gjet_ht.root qcd_pt.root" 
hadd qcdplusgjet.root gjet_ht.root qcd_pt.root  >> $THISDIR/dataDrivenEstimates.log
cd $THISDIR
echo "root -b -q runPurity.C(${INDIR})"
root -b -q "runPurity.C(\"${INDIR}\")" >> dataDrivenEstimates.log
echo "done"


