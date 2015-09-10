#!/bin/bash

function run () {
    echo root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\)
    nohup nice -n 19 root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\) >& log_merge_${TAG}_$1.txt &
}

TAG=V00-01-04_50ns

HADOOPDIR=/hadoop/cms/store/user/${USER}/mt2babies/
OUTPUTDIR=/nfs-6/userdata/mt2/$TAG/

mkdir -p $OUTPUTDIR
chmod -R a+wrx $OUTPUTDIR

#
# DATA
#

run data_Run2015B_JetHT_PromptReco
run data_Run2015B_MET_PromptReco
run data_Run2015B_HTMHT_PromptReco
run data_Run2015B_SingleMuon_PromptReco
run data_Run2015B_SingleElectron_PromptReco
run data_Run2015B_SinglePhoton_PromptReco
run data_Run2015B_DoubleMuon_PromptReco
run data_Run2015B_DoubleEG_PromptReco
run data_Run2015B_MuonEG_PromptReco

run data_Run2015B_JetHT_17Jul2015
run data_Run2015B_MET_17Jul2015
run data_Run2015B_HTMHT_17Jul2015
run data_Run2015B_SingleMuon_17Jul2015
run data_Run2015B_SingleElectron_17Jul2015
run data_Run2015B_SinglePhoton_17Jul2015
run data_Run2015B_DoubleMuon_17Jul2015
run data_Run2015B_DoubleEG_17Jul2015
run data_Run2015B_MuonEG_17Jul2015

#
# TTBAR
#

run ttall_mg_lo

#
# ALTERNATIVE TTBAR
#

run ttall_amcatnlo
run ttall_powheg_p8

#
# W+JETS
#

run wjets_amcatnlo

#
# SINGLE TOP
#

run singletop_powheg_4f_ttchan_l
run singletop_powheg_4f_tbartchan_l
run singletop_powheg_5f_tbarWchan
run singletop_powheg_5f_tWchan

#
# ALTERNATIVE SINGLE TOP
#

run singletop_amcatnlo_4f_tchan_l
run singletop_amcatnlo_5f_tchan_l

#
# DY+JETS
#

run zjets_amcatnlo


#
# GAMMA + JETS
#

#
# Z INVISIBLE
#

#
# DIBOSON
#

#
# TRIBOSON
#

#
# TTV
#

#
# QCD
#

run qcd_pt80to120
##run qcd_pt120to170
run qcd_pt170to300
run qcd_pt300to470
run qcd_pt470to600
run qcd_pt600to800
run qcd_pt800to1000
run qcd_pt1000to1400
run qcd_pt1400to1800
run qcd_pt1800to2400
run qcd_pt2400to3200
##run qcd_pt3200

#
# SIGNAL
#





