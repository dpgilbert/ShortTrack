#!/bin/bash

make -j12
if [[ $? != 0 ]]; then return $?; fi

INDIR=/nfs-6/userdata/mt2/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4/
OUTDIR=output/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4/
LOGDIR=logs/

#declare -a Samples=(data_Run2016B)
#declare -a Samples=(ttsl ttdl wjets_ht singletop qcd_ht qcd_pt ttw ttz ttg tth dyjetsll_ht gjets_dr0p05_ht data_Run2016B)
#declare -a Samples=(data_Run2015C data_Run2015D ttsl_mg_lo ttdl_mg_lo wjets_ht zinv_ht singletop qcd_ht ttw ttz ttg tth dyjetsll_ht gjet_ht)
#declare -a Samples=(gjet_ht ttsl ttdl ttz ttw wjets_ht zinv_ht singletop qcd_ht ttg data_Run2016B dyjetsll_ht)
# declare -a Samples=(gjets_ht ttsl ttdl ttz ttw wjets_ht zinv_ht singletop qcd_ht ttg data_Run2016B dyjetsll_incl)
declare -a Samples=(2015qcd_ht)
#declare -a Samples=(2015qcd_ht 2015gjets_ht 2015dyjetsll_ht 2015zinv_ht data_Run2016B)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800 T2tt_850_100 T2tt_650_325 T2tt_500_325 T2tt_425_325 T2bb_900_100 T2bb_600_580 T2qq_1200_100 T2qq_600_550)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800)
#declare -a Samples=(T1bbbb)

# 2015 samples for Zinvisible estimate
#INDIR=/nfs-6/userdata/mt2/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4/
#declare -a Samples=(2015qcd_ht 2015gjets_ht 2015dyjetsll_ht 2015zinv_ht)

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
     nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &
done
