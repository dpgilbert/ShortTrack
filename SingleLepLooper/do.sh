#!/bin/bash

make -j 8

#INDIR=/nfs-6/userdata/mt2/V00-08-09_json_271036-280385_NoL1T_24p6fb
#INDIR=/nfs-6/userdata/mt2/V00-08-09_json_271036-280385_NoL1T_24p6fb/extmerge
#INDIR=/nfs-6/userdata/mt2/V00-08-08_nojson/extmerge
#OUTDIR=output/V00-08-09_json_ichep_12p9fb_susregions_mtlt100

INDIR=/nfs-6/userdata/mt2/V00-08-10_JECandRenorm/skim_1l_6j_met50
#INDIR=/nfs-6/userdata/mt2/V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb/skim_1l_6j_met50
OUTDIR=output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_skim_1l_6j_met50_nobtagsfs_ttbarisrweight

INDIR=/nfs-6/userdata/mt2/V00-08-16_skim_1l7j/extmerge/
#INDIR=/nfs-6/userdata/mt2/V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb/skim_1l_6j_met50
OUTDIR=output/V00-08-16_36p8fb_btagsf

#declare -a Samples=(data_Run2016B_SingleElectron data_Run2016B_SingleMuon data_Run2016C_SingleElectron data_Run2016C_SingleMuon data_Run2016D_SingleElectron data_Run2016D_SingleMuon data_Run2016E_SingleElectron data_Run2016E_SingleMuon data_Run2016F_SingleElectron data_Run2016F_SingleMuon data_Run2016G_SingleElectron data_Run2016G_SingleMuon data_Run2016H_SingleElectron data_Run2016H_SingleMuon )
declare -a Samples=(data_Run2016 )
#declare -a Samples=( wjets_incl ttsl_mg_lo singletop )
#declare -a Samples=( wjets_ht ttsl ttdl singletop )
#declare -a Samples=(  singletop )
#declare -a Samples=(  ttsl_mg_lo_tbar ttsl_mg_lo_top ttdl_mg_lo  wjets_ht100to200 wjets_ht200to400 wjets_ht400to600 wjets_ht600to800 wjets_ht800to1200 wjets_ht1200to2500 wjets_ht2500toInf )
#declare -a Samples=( wjets_incl ttsl_mg_lo )
#declare -a Samples=(T1tttt_1800_1 T1tttt_1000_700)


mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup nice -n 19 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
