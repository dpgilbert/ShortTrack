#!/bin/bash

make -j12

#INDIR=/nfs-6/userdata/mt2/Spring15_QCD_MG_RebalanceAndSmear_25nsV2_MC_JEC
#OUTDIR=output/MC_controlRegions_579pb
#declare -a Samples=(qcd_ht2000toInf qcd_ht1500to2000 qcd_ht1000to1500 qcd_ht300to500 qcd_ht700to1000 qcd_ht500to700)

INDIR=/nfs-6/userdata/mt2/RebalanceAndSmear_V00-08-12
# REBALDIR=../RebalanceLooper/output/mt2babies_withPUID_withBTAG_forEWK_allEvents
OUTDIR=output/RebalanceAndSmear_V00-08-12_noRS
LOGDIR=logs_noRS

# INDIR=..
# REBALDIR=../RebalanceLooper/output/test
# OUTDIR=output/test

declare -a Samples=(qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf)
# declare -a Samples=(ttsl ttdl wjets_ht100to200 wjets_ht200to400 wjets_ht400to600 wjets_ht600to800 wjets_ht800to1200 wjets_ht1200to2500 wjets_ht2500toInf )
# declare -a Samples=(zinv_ht100to200 zinv_ht200to400 zinv_ht400to600 zinv_ht600to800 zinv_ht800to1200 zinv_ht1200to2500 zinv_ht2500toInf )
# declare -a Samples=(qcd_pt15to30 qcd_pt30to50 qcd_pt50to80 qcd_pt80to120 qcd_pt120to170 qcd_pt170to300)
# declare -a Samples=(qcd_pt300to470 qcd_pt470to600 qcd_pt600to800 qcd_pt800to1000 qcd_pt1000to1400 qcd_pt1400to1800 qcd_pt1800to2400 qcd_pt2400to3200 qcd_pt3200toInf)

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done
