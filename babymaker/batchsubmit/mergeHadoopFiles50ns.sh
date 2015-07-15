#!/bin/bash

function run () {
    echo root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\)
    nohup nice -n 19 root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\) >& log_merge_${TAG}_$1.txt &
}

TAG=V00-01-00_50ns

HADOOPDIR=/hadoop/cms/store/user/${USER}/mt2babies/
OUTPUTDIR=/nfs-6/userdata/mt2/$TAG/

mkdir -p $OUTPUTDIR
chmod -R a+wrx $OUTPUTDIR

#
# DATA
#

run data_Run2015B_JetHT
run data_Run2015B_MET
run data_Run2015B_HTMHT
run data_Run2015B_SingleMuon
run data_Run2015B_DoubleMuon
run data_Run2015B_DoubleEG
run data_Run2015B_MuonEG

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

#
# SIGNAL
#





