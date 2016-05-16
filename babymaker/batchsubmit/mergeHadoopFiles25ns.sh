#!/bin/bash

function run () {
    echo root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\)
    nohup nice -n 19 root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\) >& ${LOGDIR}/log_merge_${TAG}_$1.txt &
}

TAG=V00-08-00

HADOOPDIR=/hadoop/cms/store/user/${USER}/mt2babies/
OUTPUTDIR=/nfs-6/userdata/mt2/$TAG/
LOGDIR=mergeLogs

mkdir -p $OUTPUTDIR
mkdir -p $LOGDIR
chmod -R a+wrx $OUTPUTDIR

#
# DATA
#

# run data_Run2016B_JetHT
# run data_Run2016B_MET
# run data_Run2016B_HTMHT
# run data_Run2016B_SingleElectron
# run data_Run2016B_SingleMuon
# run data_Run2016B_SinglePhoton
# run data_Run2016B_DoubleEG
# run data_Run2016B_DoubleMuon
# run data_Run2016B_MuonEG

#
# TTBAR
#

# #run ttdl_mg_lo
# run ttsl_mg_lo_top
# run ttsl_mg_lo_tbar
# #run ttjets_mg_lo
# #run tt_powheg
# #run tt2l2nu_powheg

#
# HIGH STATS TTBAR EXTENSION
#


#
# W+JETS
#

# run wjets_ht100to200
# run wjets_ht200to400
# run wjets_ht400to600
# #run wjets_ht600to800
# #run wjets_ht600toInf
# run wjets_ht800to1200
# run wjets_ht1200to2500
run wjets_ht2500toInf
# #run wjets_mg_lo

#
# SINGLE TOP
#

#run singletop_amcatnlo_4f_schan
#run singletop_powheg_5f_tWchan
# run singletop_powheg_5f_tbarWchan

#
# DY+JETS
#

# run dyjetsll_ht100to200
# run dyjetsll_ht200to400
# run dyjetsll_ht400to600
# run dyjetsll_ht600toInf
# # run dyjets_m50_ht200to400
# # run dyjets_m50_ht400to600
# # run dyjets_m50_mg_lo
# # run dyjets_m5to50_ht100to200
# # run dyjets_m5to50_ht200to400
# # run dyjets_m5to50_ht400to600
# # run dyjets_m5to50_ht600toInf

#
# GAMMA + JETS
#

# run gjets_ht100to200
# run gjets_ht200to400
# run gjets_ht400to600
# run gjets_ht40to100
# run gjets_ht600toInf


#
# Z INVISIBLE
#

# run zinv_ht100to200
# run zinv_ht200to400
# run zinv_ht400to600
# run zinv_ht600toInf
# run zinv_ht600to800
# run zinv_ht800to1200
# run zinv_ht1200to2500
# run zinv_ht2500toInf

# #
# # DIBOSON
# #

# #
# # TRIBOSON
# #

# #
# # TTV
# #

# run ttg_amcatnlo
# run ttw_lnu_amcatnlo
# run ttw_qq_amcatnlo
# run ttz_llnunu_amcatnlo
# run ttz_qq_amcatnlo

# #
# # QCD
# #

# run qcd_pt120to170
# run qcd_pt170to300
# run qcd_pt300to470
# run qcd_pt470to600
# run qcd_pt600to800
# run qcd_pt800to1000
# run qcd_pt1000to1400
# run qcd_pt1400to1800
# run qcd_pt1800to2400
# run qcd_pt2400to3200
# run qcd_pt3200toInf

# # run qcd_ht200to300
# run qcd_ht300to500
# # run qcd_ht500to700
# run qcd_ht700to1000
# run qcd_ht1000to1500
# run qcd_ht1500to2000
# run qcd_ht2000toInf

# #
# # SIGNAL
# #

# run T1tttt_1500_100
# run T1tttt_1200_800
# run T1bbbb_1500_100
# run T1bbbb_1000_900
# run T1qqqq_1400_100
# run T1qqqq_1000_800
