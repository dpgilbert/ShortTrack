#!/bin/bash

INDIR=/home/users/olivito/mt2_74x_dev/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_Summer15_25nsV6_skim_base_2p1fb_mt2gt200/
#INDIR=/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-05_25ns_json_246908-256869_skimV3_3fb/
THISDIR=`pwd`

## to use data for lostlepton
LOSTLEPFILE=data_Run2015CD
## to use MC for lostlepton
#LOSTLEPFILE=lostlep

GJETFILE=data_Run2015CD
#GJETFILE=qcdplusgjet

RLFILE=data_Run2015CD
#RLFILE=removedlep

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR
echo "hadd -f data_Run2015CD.root data_Run2015C.root data_Run2015D.root"
hadd -f data_Run2015CD.root data_Run2015C.root data_Run2015D.root

echo "hadd -f top.root ttsl_mg_lo.root ttdl_mg_lo.root singletop.root ttw.root ttz.root tth.root"
hadd -f top.root ttsl_mg_lo.root ttdl_mg_lo.root singletop.root ttw.root ttz.root tth.root

## make "lostlep" from sum of MC lostlep backgrounds (except QCD, to avoid spikes)
echo "hadd -f lostlep.root ttsl_mg_lo.root ttdl_mg_lo.root singletop.root ttw.root ttz.root tth.root wjets_ht.root"
hadd -f lostlep.root ttsl_mg_lo.root ttdl_mg_lo.root singletop.root ttw.root ttz.root tth.root wjets_ht.root
cd $THISDIR

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/data_Run2015CD.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/data_Run2015CD.root\",2)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/top.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/top.root\",6)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/lostlep.root,7)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/lostlep.root\",7)"

# make the data driven background files
echo "root -b -q lostlepMaker.C+(${INDIR},${LOSTLEPFILE})"
root -b -q "lostlepMaker.C+(\"${INDIR}\",\"${LOSTLEPFILE}\")" > dataDrivenEstimates.log
echo "root -b -q ZinvMaker.C+(${INDIR})"
root -b -q "ZinvMaker.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
cd $INDIR
echo "hadd qcdplusgjet.root gjet_ht.root qcd_ht.root"
hadd -f qcdplusgjet.root gjet_ht.root qcd_ht.root  >> $THISDIR/dataDrivenEstimates.log
echo "hadd CRRLbkg.root ttsl_mg_lo.root ttdl_mg_lo.root singletop.root" # should probably include QCD here
hadd -f CRRLbkg.root ttsl_mg_lo.root ttdl_mg_lo.root singletop.root  >> $THISDIR/dataDrivenEstimates.log
hadd -f removedlep.root wjets_ht.root CRRLbkg.root >> $THISDIR/dataDrivenEstimates.log
cd $THISDIR
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/qcdplusgjet.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/qcdplusgjet.root\",2)" >> dataDrivenEstimates.log
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/CRRLbkg.root,3)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/CRRLbkg.root\",3)" >> dataDrivenEstimates.log
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/removedlep.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/removedlep.root\",2)" >> dataDrivenEstimates.log
echo "root -b -q purity.C+(${INDIR})"
root -b -q "purity.C+(\"${INDIR}\",\"${GJETFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q purityRL.C+(${INDIR}, ${RLFILE})"
root -b -q "purityRL.C+(\"${INDIR}\",\"${RLFILE}\")" >> dataDrivenEstimates.log
echo "done"


