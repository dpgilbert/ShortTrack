#!/bin/bash

#INDIR=/home/users/jgran/limits_for_paper/MT2Analysis/MT2looper/output/V00-01-09_25ns_skim_base_mt2gt200_ZinvV3_2p2fb/
#INDIR=/home/users/olivito/mt2_74x_dev/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_skim_base_1p26fb_mt2gt200_crqcd/
#INDIR=/home/users/gzevi/MT2/MT2Analysis80X/MT2Analysis/MT2looper/output/Bennett_V00-08-02_json_Cert_271036-274421_skim_base_mt2gt200_ZinvV4/
# INDIR=/home/users/gzevi/MT2/MT2Analysis80X/MT2Analysis/MT2looper/output/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4
INDIR=/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_12p9fb
THISDIR=`pwd`

## to use data for lostlepton
LOSTLEPFILE=data_Run2016
## to use MC for lostlepton
#LOSTLEPFILE=lostlep

GJETFILE=data_Run2016
#GJETFILE=qcdplusgjet

RLFILE=data_Run2016
#RLFILE=removedlep

QCDFILE=data_Run2016
#QCDFILE=qcd_ht

QCDESTIMATE=./inputs/qcdEstimateData_2016_ICHEP
QCDMONOJET=./inputs/qcdEstimateMonojet_2016_ICHEP

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR

echo "hadd -f data_Run2016.root data_Run2016B.root data_Run2016C.root data_Run2016D.root"
hadd -f data_Run2016.root data_Run2016B.root data_Run2016C.root data_Run2016D.root > dataDrivenEstimates.log

## (temporarily replaced tth with ttg, because of sample availability)
echo "hadd -f top.root ttsl.root ttdl.root singletop.root ttw.root ttz.root ttg.root"
hadd -f top.root ttsl.root ttdl.root singletop.root ttw.root ttz.root ttg.root >> dataDrivenEstimates.log

## make "lostlep" from sum of MC lostlep backgrounds (except QCD, to avoid spikes)
echo "hadd -f lostlep.root ttsl.root ttdl.root singletop.root ttw.root ttz.root ttg.root wjets_ht.root" ## (temporarily replaced tth with ttg, because of sample availability)
hadd -f lostlep.root ttsl.root ttdl.root singletop.root ttw.root ttz.root ttg.root wjets_ht.root >> dataDrivenEstimates.log
cd $THISDIR

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/data_Run2016.root,3)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/data_Run2016.root\",3)" >> dataDrivenEstimates.log

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/top.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/top.root\",6)" >> dataDrivenEstimates.log

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/lostlep.root,7)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/lostlep.root\",7)" >> dataDrivenEstimates.log

# make the data driven background files
echo "root -b -q lostlepMaker.C+(${INDIR},${LOSTLEPFILE})"
root -b -q "lostlepMaker.C+(\"${INDIR}\",\"${LOSTLEPFILE}\")" >> dataDrivenEstimates.log
echo "root -b -q ZinvMaker.C+(${INDIR})"
root -b -q "ZinvMaker.C+(\"${INDIR}\")" >> dataDrivenEstimates.log
cd $INDIR
echo "hadd qcdplusgjet.root 2015gjets_ht.root 2015qcd_ht.root"
hadd -f qcdplusgjet.root 2015gjets_ht.root 2015qcd_ht.root  >> $THISDIR/dataDrivenEstimates.log
echo "hadd CRRLbkg.root ttsl.root ttdl.root singletop.root" # should probably include QCD here
hadd -f CRRLbkg.root ttsl.root ttdl.root singletop.root  >> $THISDIR/dataDrivenEstimates.log
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
echo "root -b -q qcdRphiMaker.C+(${INDIR},${QCDFILE},${QCDESTIMATE},${QCDMONOJET})"
root -b -q "qcdRphiMaker.C+(\"${INDIR}\",\"${QCDFILE}\",\"${QCDESTIMATE}\",\"${QCDMONOJET}\")" >> dataDrivenEstimates.log
echo "done"


