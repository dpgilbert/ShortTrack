#!/bin/bash

function run () {
    echo root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\)
    nohup nice -n 19 root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\) >& ${LOGDIR}/log_merge_${TAG}_$1.txt &
}

TAG=V00-08-08_nojson

HADOOPDIR=/hadoop/cms/store/user/${USER}/mt2babies/
OUTPUTDIR=/nfs-6/userdata/mt2/$TAG/
LOGDIR=mergeLogs

mkdir -p $OUTPUTDIR
mkdir -p $LOGDIR
chmod -R a+wrx $OUTPUTDIR

#
# DATA
#

# run data_Run2016B_JetHT_PromptReco
# run data_Run2016B_MET_PromptReco
# run data_Run2016B_HTMHT_PromptReco
# run data_Run2016B_SingleElectron_PromptReco
# run data_Run2016B_SingleMuon_PromptReco
# run data_Run2016B_SinglePhoton_PromptReco
# run data_Run2016B_DoubleEG_PromptReco
# run data_Run2016B_DoubleMuon_PromptReco
# run data_Run2016B_MuonEG_PromptReco

# run data_Run2016C_JetHT_PromptReco
# run data_Run2016C_MET_PromptReco
# run data_Run2016C_HTMHT_PromptReco
# run data_Run2016C_SingleElectron_PromptReco
# run data_Run2016C_SingleMuon_PromptReco
# run data_Run2016C_SinglePhoton_PromptReco
# run data_Run2016C_DoubleEG_PromptReco
# run data_Run2016C_DoubleMuon_PromptReco
# run data_Run2016C_MuonEG_PromptReco

# run data_Run2016D_JetHT_PromptReco
# run data_Run2016D_MET_PromptReco
# run data_Run2016D_HTMHT_PromptReco
# run data_Run2016D_SingleElectron_PromptReco
# run data_Run2016D_SingleMuon_PromptReco
# run data_Run2016D_SinglePhoton_PromptReco
# run data_Run2016D_DoubleEG_PromptReco
# run data_Run2016D_DoubleMuon_PromptReco
# run data_Run2016D_MuonEG_PromptReco

# run data_Run2016E_JetHT_PromptReco
# run data_Run2016E_MET_PromptReco
# run data_Run2016E_HTMHT_PromptReco
# run data_Run2016E_SingleElectron_PromptReco
# run data_Run2016E_SingleMuon_PromptReco
# run data_Run2016E_SinglePhoton_PromptReco
# run data_Run2016E_DoubleEG_PromptReco
# run data_Run2016E_DoubleMuon_PromptReco
# run data_Run2016E_MuonEG_PromptReco

# run data_Run2016F_JetHT_PromptReco
# run data_Run2016F_MET_PromptReco
# run data_Run2016F_HTMHT_PromptReco
# run data_Run2016F_SingleElectron_PromptReco
# run data_Run2016F_SingleMuon_PromptReco
# run data_Run2016F_SinglePhoton_PromptReco
# run data_Run2016F_DoubleEG_PromptReco
# run data_Run2016F_DoubleMuon_PromptReco
# run data_Run2016F_MuonEG_PromptReco

#
# TTBAR
#

# run ttdl_mg_lo
# run ttsl_mg_lo_top
# run ttsl_mg_lo_tbar

#
# HIGH STATS TTBAR EXTENSION
#

# run ttsl_mg_lo_top_ext1
# run ttsl_mg_lo_tbar_ext1
# run ttdl_mg_lo_ext1

#
# W+JETS
#

# run wjets_ht100to200
# run wjets_ht200to400
# run wjets_ht400to600
# run wjets_ht600to800
# run wjets_ht800to1200
# run wjets_ht1200to2500
# run wjets_ht2500toInf

# run wjets_ht100to200_ext1
# run wjets_ht200to400_ext1
# run wjets_ht400to600_ext1
# run wjets_ht600to800_ext1
# run wjets_ht800to1200_ext1
# run wjets_ht1200to2500_ext1
# run wjets_ht2500toInf_ext1

# # run wjets_incl

#
# SINGLE TOP
#

# run singletop_amcatnlo_4f_schan
# run singletop_powheg_5f_tWchan_l
# run singletop_powheg_5f_tbarWchan_l
# run singletop_powheg_4f_ttchan_l
# run singletop_powheg_4f_tbartchan_l

#
# DY+JETS
#

# run dyjetsll_ht100to200
# run dyjetsll_ht200to400
# run dyjetsll_ht400to600
# run dyjetsll_ht600to800
# run dyjetsll_ht800to1200
# run dyjetsll_ht1200to2500
# run dyjetsll_ht2500toInf
# #run dyjetsll_ht600toInf

# run dyjetsll_ht100to200_ext1
# run dyjetsll_ht200to400_ext1
# run dyjetsll_ht400to600_ext1
# #run dyjetsll_ht600toInf_ext1

# #run dyjetsll_incl


#
# GAMMA + JETS
#

# run gjets_dr0p4_ht40to100
# run gjets_dr0p4_ht100to200
# run gjets_dr0p4_ht200to400
# run gjets_dr0p4_ht400to600
# run gjets_dr0p4_ht600toInf

# run gjets_dr0p05_ht40to100
# run gjets_dr0p05_ht100to200
# run gjets_dr0p05_ht200to400
# run gjets_dr0p05_ht400to600
# run gjets_dr0p05_ht600toInf

#
# Z INVISIBLE
#

# run zinv_ht100to200
# run zinv_ht200to400
# run zinv_ht400to600
# run zinv_ht600to800
# run zinv_ht800to1200
# run zinv_ht1200to2500
# run zinv_ht2500toInf

# run zinv_ht100to200_ext1
# run zinv_ht200to400_ext1
# run zinv_ht400to600_ext1
# # run zinv_ht600to800_ext1
# # run zinv_ht800to1200_ext1
# run zinv_ht1200to2500_ext1
# # run zinv_ht2500toInf_ext1

#
# DIBOSON
#

#
# TRIBOSON
#

#
# TTV, TT+X
#

# run ttg_amcatnlo
# #run ttw_lnu_amcatnlo
# #run ttw_qq_amcatnlo
# #run ttz_nunu_amcatnlo
# #run ttz_qq_amcatnlo

# run ttw_mg_lo
# run ttz_mg_lo

# run tttt
# run ttbb
# run ttbb_ext1


# run tth_bb
# run tth_nonbb

#
# QCD
#

# run qcd_pt80to120
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

# run qcd_pt470to600_ext1
# run qcd_pt600to800_ext1

# # run qcd_ht200to300
# run qcd_ht300to500
# run qcd_ht500to700
# run qcd_ht700to1000
# run qcd_ht1000to1500
# run qcd_ht1500to2000
# run qcd_ht2000toInf

# # run qcd_ht200to300_ext1
# run qcd_ht300to500_ext1
# run qcd_ht500to700_ext1
# run qcd_ht700to1000_ext1
# run qcd_ht1000to1500_ext1
# run qcd_ht1500to2000_ext1
# run qcd_ht2000toInf_ext1

#
# SIGNAL
#

# run T1tttt_1500_100
# run T1tttt_1200_800
# run T1bbbb_1500_100
# run T1bbbb_1000_900
# # run T1qqqq_1400_100
# # run T1qqqq_1000_800

#
# SIGNAL SCANS
#

# run T1tttt
# run T1bbbb
# run T1qqqq
# # run T5qqqqVV
# run T2bb
# run T2tt_mStop-400to1200
# run T2tt_mStop-150to250
# run T2tt_mStop-250to350
# run T2tt_mStop-350to400
# run T2qq
