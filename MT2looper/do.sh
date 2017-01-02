#!/bin/bash

make -j12
if [[ $? != 0 ]]; then return $?; fi

OUTDIR=output/full2016
LOGDIR=logs/
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

#most recent MC
INDIR=/nfs-6/userdata/mt2/V00-08-10_JECandRenorm_nojson_skim_base_mt2gt200_ZinvV6/
declare -a Samples=(ww ttsl ttdl singletop qcd_ht ttw_mg_lo ttz_mg_lo ttg dyjetsll_ht gjets_dr0p05_ht wjets_ht100to200 wjets_ht1200to2500 wjets_ht200to400 wjets_ht2500toInf wjets_ht400to600 wjets_ht600to800 wjets_ht800to1200 zinv_ht)

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
     nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done

#most recent run2016H and re-reco
INDIR=/nfs-6/userdata/mt2/V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb_skim_base_mt2gt200_ZinvV6/
declare -a Samples=(data_Run2016B data_Run2016C data_Run2016D data_Run2016E data_Run2016F data_Run2016G data_Run2016H)

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done
