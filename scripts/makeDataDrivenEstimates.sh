#!/bin/bash

INDIR=/home/users/jgran/limits_for_paper/MT2Analysis/MT2looper/output/V00-01-09_25ns_skim_base_mt2gt200_ZinvV3_2p2fb/
#INDIR=/home/users/olivito/mt2_74x_dev/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_skim_base_1p26fb_mt2gt200_crqcd/
#INDIR=/home/users/gzevi/MT2/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_Summer15_25nsV6_2p1fb_skim_base_mt2gt200_ZinvV3/
THISDIR=`pwd`

## to use data for lostlepton
LOSTLEPFILE=data_Run2015CD
#LOSTLEPFILE=data_Run2015D
## to use MC for lostlepton
#LOSTLEPFILE=lostlep

GJETFILE=data_Run2015CD
#GJETFILE=data_Run2015D
#GJETFILE=qcdplusgjet

RLFILE=data_Run2015CD
#RLFILE=data_Run2015D
#RLFILE=removedlep

QCDFILE=data_Run2015CD
#QCDFILE=data_Run2015D
#QCDFILE=qcd_ht

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
echo "root -b -q DataRZGammaRatioMaker.C+(${INDIR})"
root -b -q "DataRZGammaRatioMaker.C+(\"${INDIR}\",\"${GJETFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q qcdRphiMaker.C+(${INDIR},${QCDFILE})"
root -b -q "qcdRphiMaker.C+(\"${INDIR}\",\"${QCDFILE}\")" >> dataDrivenEstimates.log
echo "done"


