#!/bin/bash

make all

INDIR=/nfs-6/userdata/dpgilber/MT2_ST_Friends_incl_skim
INDIR_UNSKIMMED=/nfs-6/userdata/dpgilber/MT2_ST_Friends
# bitwise: <doLep><doMT2><doDphiDiff><doHTMET><doHT>
# if any bits are high, do Nj > 0
#SELECTION=00000 # Inclusive
#SELECTION=00001 # HT
#SELECTION=00011 # HTMET
#SELECTION=00111 # HTMETDphiDiff
#SELECTION=01111 # HTMETDphiDiffMT2
#SELECTION=11111 # HTMETDphiDiffMT2Lep
LOGDIR=logs

# 0 = SR, 1 = CRQCD, 2 = CRSL, 3 = CRDY
#REGION=1

mkdir -p ${LOGDIR}

#declare -a REGIONS=(0 1 2 3)
#declare -a REGIONS=(0 1)
declare -a REGIONS=(2 3)
declare -a SELECTIONS=(00000 00001 00011 00111 01111 11111)
declare -a Samples=(ttsl_fromT ttsl_fromTbar wjets_incl zinv_zpt100to200 zinv_zpt200toInf qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000_ext1 qcd_ht2000toInf ttdl dyjetstoll_incl_ext1)
#declare -a Samples=(ttdl)

for SELECTION in ${SELECTIONS[@]}; do
    for REGION in ${REGIONS[@]}; do
	for SAMPLE in ${Samples[@]}; do
	    echo "nohup nice -n 10 ./ShortTrackType_mu.exe ${INDIR} ${SAMPLE} ${INDIR_UNSKIMMED} ${SELECTION} ${REGION} >& ${LOGDIR}/${SAMPLE}.log &"
	    eval "nohup nice -n 10 ./ShortTrackType_mu.exe ${INDIR} ${SAMPLE} ${INDIR_UNSKIMMED} ${SELECTION} ${REGION} >& ${LOGDIR}/${SAMPLE}.log &"
	done
    done
done
