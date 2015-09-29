#!/bin/bash

INDIR=/home/users/olivito/mt2_74x_dev/MT2Analysis/MT2looper/output/V00-01-05_25ns_json_246908-256869_kfactors_skim_150pb_mt2gt200/
THISDIR=`pwd`

## to use data for lostlepton
LOSTLEPFILE=data_Run2015D
## to use MC for lostlepton
#LOSTLEPFILE=lostlep

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR
echo "hadd -f top.root ttsl_mg_lo.root ttdl_mg_lo.root singletop_powheg.root ttw.root ttz.root"
hadd -f top.root ttsl_mg_lo.root ttdl_mg_lo.root singletop_powheg.root ttw.root ttz.root

## make "lostlep" from sum of MC lostlep backgrounds (except QCD, to avoid spikes)
echo "hadd -f lostlep.root ttsl_mg_lo.root ttdl_mg_lo.root singletop_powheg.root ttw.root ttz.root wjets_ht.root"
hadd -f lostlep.root ttsl_mg_lo.root ttdl_mg_lo.root singletop_powheg.root ttw.root ttz.root wjets_ht.root
cd $THISDIR

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/top.root,5)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/top.root\",5)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/lostlep.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/lostlep.root\",6)"

# make the data driven background files
echo "root -b -q lostlepMaker.C+(${INDIR},${LOSTLEPFILE})"
root -b -q "lostlepMaker.C+(\"${INDIR}\",\"${LOSTLEPFILE}\")" > dataDrivenEstimates.log
echo "root -b -q ZinvMaker.C+(${INDIR})"
root -b -q "ZinvMaker.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
cd $INDIR
rm qcdplusgjet.root
echo "hadd qcdplusgjet.root gjet_ht.root qcd_pt.root" 
hadd qcdplusgjet.root gjet_ht.root qcd_pt.root  >> $THISDIR/dataDrivenEstimates.log
rm CRRLbkg.root
echo "hadd CRRLbkg.root ttall_mg_lo.root singletop_powheg.root" # should probably include QCD here 
hadd CRRLbkg.root ttall_mg_lo.root singletop_powheg.root  >> $THISDIR/dataDrivenEstimates.log
cd $THISDIR
echo "root -b -q purity.C+(${INDIR})"
root -b -q "purity.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
echo "root -b -q purityRL.C+(${INDIR})"
root -b -q "purityRL.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
echo "done"


