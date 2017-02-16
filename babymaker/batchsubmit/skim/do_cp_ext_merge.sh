#!/bin/bash

# for samples where we don't merge with extensions. remove skim,rphi from the names
function do_cp ()
{
    for oldname in `/bin/ls $INDIR/$1*.root`;
    do
	oldname1=$(basename ${oldname})
	newname1=${oldname1/skim_/}
	newname=${newname1/rphi_/}
	echo "cp ${oldname} ${OUTPUTDIR}/${newname}"
	cp ${oldname} ${OUTPUTDIR}/${newname}
    done
}

# for samples where we do merge with extensions
function do_extmerge ()
{   
    echo root -b -q mergeFixScale1fb.C+\(\"$INDIR\",\"$1\"\,\"$OUTPUTDIR\"\) 
    nohup nice -n 19 root -b -q mergeFixScale1fb.C+\(\"$INDIR\",\"$1\"\,\"$OUTPUTDIR\"\) >& $LOGDIR/log_mergeskim_$1.txt &
}

TAG=V00-08-16

INDIR=/hadoop/cms/store/user/${USER}/AutoTwopler_babies/merged/MT2/${TAG}/skim/
OUTPUTDIR=/nfs-6/userdata/mt2/${TAG}_skim_base_mt2gt200_ZinvV6_JECs/

#INDIR=/hadoop/cms/store/user/${USER}/AutoTwopler_babies/merged/MT2/${TAG}/skim_rphi/
#OUTPUTDIR=/nfs-6/userdata/mt2/${TAG}_skim_rphi/

LOGDIR=mergeLogs

mkdir -p $OUTPUTDIR
mkdir -p $LOGDIR
chmod -R a+wrx $OUTPUTDIR


#
# DATA
#

do_cp data_Run2016B_JetHT_23Sep2016
do_cp data_Run2016B_MET_23Sep2016
do_cp data_Run2016B_HTMHT_23Sep2016
do_cp data_Run2016B_SingleElectron_23Sep2016
do_cp data_Run2016B_SingleMuon_23Sep2016
do_cp data_Run2016B_SinglePhoton_23Sep2016
do_cp data_Run2016B_DoubleEG_23Sep2016
do_cp data_Run2016B_DoubleMuon_23Sep2016
do_cp data_Run2016B_MuonEG_23Sep2016

do_cp data_Run2016C_JetHT_23Sep2016
do_cp data_Run2016C_MET_23Sep2016
do_cp data_Run2016C_HTMHT_23Sep2016
do_cp data_Run2016C_SingleElectron_23Sep2016
do_cp data_Run2016C_SingleMuon_23Sep2016
do_cp data_Run2016C_SinglePhoton_23Sep2016
do_cp data_Run2016C_DoubleEG_23Sep2016
do_cp data_Run2016C_DoubleMuon_23Sep2016
do_cp data_Run2016C_MuonEG_23Sep2016

do_cp data_Run2016D_JetHT_23Sep2016
do_cp data_Run2016D_MET_23Sep2016
do_cp data_Run2016D_HTMHT_23Sep2016
do_cp data_Run2016D_SingleElectron_23Sep2016
do_cp data_Run2016D_SingleMuon_23Sep2016
do_cp data_Run2016D_SinglePhoton_23Sep2016
do_cp data_Run2016D_DoubleEG_23Sep2016
do_cp data_Run2016D_DoubleMuon_23Sep2016
do_cp data_Run2016D_MuonEG_23Sep2016

do_cp data_Run2016E_JetHT_23Sep2016
do_cp data_Run2016E_MET_23Sep2016
do_cp data_Run2016E_HTMHT_23Sep2016
do_cp data_Run2016E_SingleElectron_23Sep2016
do_cp data_Run2016E_SingleMuon_23Sep2016
do_cp data_Run2016E_SinglePhoton_23Sep2016
do_cp data_Run2016E_DoubleEG_23Sep2016
do_cp data_Run2016E_DoubleMuon_23Sep2016
do_cp data_Run2016E_MuonEG_23Sep2016

do_cp data_Run2016F_JetHT_23Sep2016
do_cp data_Run2016F_MET_23Sep2016
do_cp data_Run2016F_HTMHT_23Sep2016
do_cp data_Run2016F_SingleElectron_23Sep2016
do_cp data_Run2016F_SingleMuon_23Sep2016
do_cp data_Run2016F_SinglePhoton_23Sep2016
do_cp data_Run2016F_DoubleEG_23Sep2016
do_cp data_Run2016F_DoubleMuon_23Sep2016
do_cp data_Run2016F_MuonEG_23Sep2016

do_cp data_Run2016G_JetHT_23Sep2016
do_cp data_Run2016G_MET_23Sep2016
do_cp data_Run2016G_HTMHT_23Sep2016
do_cp data_Run2016G_SingleElectron_23Sep2016
do_cp data_Run2016G_SingleMuon_23Sep2016
do_cp data_Run2016G_SinglePhoton_23Sep2016
do_cp data_Run2016G_DoubleEG_23Sep2016
do_cp data_Run2016G_DoubleMuon_23Sep2016
do_cp data_Run2016G_MuonEG_23Sep2016

do_cp data_Run2016H_JetHT_PromptReco-v2
do_cp data_Run2016H_MET_PromptReco-v2
do_cp data_Run2016H_HTMHT_PromptReco-v2
do_cp data_Run2016H_SingleElectron_PromptReco-v2
do_cp data_Run2016H_SingleMuon_PromptReco-v2
do_cp data_Run2016H_SinglePhoton_PromptReco-v2
do_cp data_Run2016H_DoubleEG_PromptReco-v2
do_cp data_Run2016H_DoubleMuon_PromptReco-v2
do_cp data_Run2016H_MuonEG_PromptReco-v2

do_cp data_Run2016H_JetHT_PromptReco-v3
do_cp data_Run2016H_MET_PromptReco-v3
do_cp data_Run2016H_HTMHT_PromptReco-v3
do_cp data_Run2016H_SingleElectron_PromptReco-v3
do_cp data_Run2016H_SingleMuon_PromptReco-v3
do_cp data_Run2016H_SinglePhoton_PromptReco-v3
do_cp data_Run2016H_DoubleEG_PromptReco-v3
do_cp data_Run2016H_DoubleMuon_PromptReco-v3
do_cp data_Run2016H_MuonEG_PromptReco-v3


# #
# # TTBAR
# #

do_extmerge ttsl_mg_lo_top
do_extmerge ttsl_mg_lo_tbar
do_extmerge ttdl_mg_lo

#do_cp ttsl_mg_lo_top_ext1
#do_cp ttsl_mg_lo_tbar_ext1
#do_cp ttdl_mg_lo_ext1

# #
# # W+JETS
# #

do_extmerge wjets_ht100to200
do_extmerge wjets_ht200to400
do_extmerge wjets_ht400to600
do_extmerge wjets_ht600to800
do_cp wjets_ht800to1200
do_cp wjets_ht1200to2500
do_extmerge wjets_ht2500toInf

# # # do_cp wjets_incl

# # #
# # # SINGLE TOP
# # #

do_cp singletop_amcatnlo_4f_schan
do_cp singletop_powheg_5f_tWchan_l
do_cp singletop_powheg_5f_tbarWchan_l
#do_cp singletop_powheg_4f_ttchan
do_cp singletop_powheg_4f_tbartchan
do_cp twz_ll_mg_lo

# #
# # DY+JETS
# #

do_cp dyjetsll_ht100to200
do_extmerge dyjetsll_ht200to400
do_extmerge dyjetsll_ht400to600
do_cp dyjetsll_ht600to800
do_cp dyjetsll_ht800to1200
do_cp dyjetsll_ht1200to2500
do_cp dyjetsll_ht2500toInf

# do_cp dyjetsll_incl


# #
# # GAMMA + JETS
# #

do_cp gjets_dr0p4_ht40to100
do_cp gjets_dr0p4_ht100to200
do_cp gjets_dr0p4_ht200to400
do_cp gjets_dr0p4_ht400to600
do_cp gjets_dr0p4_ht600toInf

do_cp gjets_dr0p05_ht40to100
do_cp gjets_dr0p05_ht100to200
do_cp gjets_dr0p05_ht200to400
do_cp gjets_dr0p05_ht400to600
do_extmerge gjets_dr0p05_ht600toInf


#
# Z INVISIBLE
#

do_cp zinv_ht100to200
do_cp zinv_ht200to400
do_cp zinv_ht400to600
do_cp zinv_ht600to800
do_cp zinv_ht800to1200
do_cp zinv_ht1200to2500
do_cp zinv_ht2500toInf

# #
# # DIBOSON
# #

do_cp ww_2l2nu_powheg
# do_cp ww_lnuqq_powheg
# do_cp wz_lnqq_amcnlo
# do_cp wz_1l3n_amcnlo
# do_cp wz_3lnu_powheg
# do_cp zz_2l2n_powheg
# do_cp zz_2l2q_powheg
# do_cp zz_2q2n_amcnlo
# do_cp zz_4l_powheg

# #
# # TRIBOSON
# #

# do_cp www_incl_amcnlo
# do_cp wwz_incl_amcnlo
# do_cp wzz_incl_amcnlo
# do_cp zzz_incl_amcnlo

# #
# # TTV, TT+X
# #

do_cp ttg_amcatnlo

do_cp ttw_mg_lo
do_cp ttz_mg_lo

do_cp tttt
# do_cp ttbb

do_cp tth_bb
do_cp tth_nonbb

#
# QCD
#

# do_cp qcd_pt80to120
# do_cp qcd_pt120to170
# do_cp qcd_pt170to300
# do_cp qcd_pt300to470
# do_cp qcd_pt470to600
# do_cp qcd_pt600to800
# do_cp qcd_pt800to1000
# do_cp qcd_pt1000to1400
# do_cp qcd_pt1400to1800
# do_cp qcd_pt1800to2400
# do_cp qcd_pt2400to3200
# do_cp qcd_pt3200toInf

# # do_extmerge qcd_ht200to300
do_extmerge qcd_ht300to500
do_extmerge qcd_ht500to700
do_extmerge qcd_ht700to1000
do_extmerge qcd_ht1000to1500
do_extmerge qcd_ht1500to2000
do_extmerge qcd_ht2000toInf

#
# SIGNAL
#

# do_cp T1tttt_1500_100
# do_cp T1tttt_1200_800
# do_cp T1bbbb_1500_100
# do_cp T1bbbb_1000_900
# # do_cp T1qqqq_1400_100
# # do_cp T1qqqq_1000_800

#
# SIGNAL SCANS
#

do_cp T1tttt
do_cp T1bbbb
do_cp T1qqqq
# do_cp T5qqqqVV
do_cp T2bb
do_cp T2tt_mStop-400to1200
do_cp T2tt_mStop-150to250
do_cp T2tt_mStop-250to350
do_cp T2tt_mStop-350to400
do_cp T2qq
do_cp T2cc
