#!/bin/bash

make -j 12 || return $?

OUTDIR=output/trialST_unskimmed
LOGDIR=logs/
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# unskimmed
INDIR=/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/MT2/V00-08-18_mc/output/

declare -a Samples=(ttsl_mg_lo_ext1 ttdl_mg_lo_ext1 singletop ttw_mg_lo ttz_mg_lo ttg tth dyjetsll_ht wjets_ht100to200_ext1 wjets_ht1200to2500 wjets_ht200to400_ext1 wjets_ht2500toInf_ext1 wjets_ht400to600_ext1 wjets_ht600to800_ext1 wjets_ht800to1200_ext1 zinv_ht qcd_ht1000to1500_ext1 qcd_ht100to200 qcd_ht_1500to2000_ext1 qcd_ht1500to2000_ext1 qcd_ht2000toInf_ext1 qcd_ht200to300_ext1 qcd_ht300to500_ext1 qcd_ht500t700_ext1 qcd_ht700to1000_ext1 gjets_dr0p05)

for SAMPLE in ${Samples[@]}; do
    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done
