#! /bin/bash

#INPATH=/nfs-6/userdata/mt2/V00-08-08_nojson
#OUTPATH=/nfs-6/userdata/mt2/V00-08-08_nojson/extmerge

INPATH=/nfs-6/userdata/mt2/V00-08-10_JECandRenorm/
OUTPATH=/nfs-6/userdata/mt2/V00-08-10_JECandRenorm/extmerge

#declare -a Samples=(qcd_ht200to300 qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf)
#declare -a Samples=( qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf)

#declare -a Samples=(  ttsl_mg_lo_tbar ttdl_mg_lo wjets_ht200to400 wjets_ht800to1200 wjets_ht1200to2500 dyjetsll_ht600toInf zinv_ht1200to2500 qcd_ht300to500 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf )
#declare -a Samples=(  ttsl_mg_lo_tbar ttdl_mg_lo wjets_ht200to400 wjets_ht800to1200 wjets_ht1200to2500  )
#declare -a Samples=( ttsl_mg_lo_tbar wjets_ht200to400 wjets_ht800to1200 wjets_ht1200to2500 dyjetsll_ht600toInf  )
#declare -a Samples=( zinv_ht1200to2500 qcd_ht300to500 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf ttdl_mg_lo )
#declare -a Samples=( ttdl_mg_lo  )
#declare -a Samples=(  dyjetsll_ht600toInf zinv_ht1200to2500 qcd_ht300to500 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf )
#declare -a Samples=(  qcd_ht300to500 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf )
declare -a Samples=( zinv_ht100to200 zinv_ht200to400 zinv_ht400to600  zinv_ht600to800 zinv_ht800to1200 zinv_ht_1200to2500 zinv_ht2500toInf)

mkdir -p mergeLogs

for SAMPLE in ${Samples[@]}
  do nohup nice -n 19 root -b -q mergeFixScale1fb.C+\(\"$INPATH\",\"$SAMPLE\",\"$OUTPATH\"\) >& mergeLogs/log_extmerge_$SAMPLE.txt &
#  do nohup nice -n 19 root -b -q mergeFixScale1fb.C+\(\"$INPATH\",\"$SAMPLE\",\"$OUTPATH\",347700\) >& mergeLogs/log_extmerge_$SAMPLE.txt &
done
