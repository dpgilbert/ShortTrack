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

# 0 = SR, 1 = CRQCD, 2 = CRSL, 3 = CRDY
#REGION=1

declare -a REGIONS=(1)
declare -a SELECTIONS=(00001)
declare -a Samples=(ttsl_fromT ttsl_fromTbar)

for SELECTION in ${SELECTIONS[@]}; do
    for REGION in ${REGIONS[@]}; do
	for SAMPLE in ${Samples[@]}; do
	    echo "./ShortTrackType.exe ${INDIR} ${SAMPLE} ${INDIR_UNSKIMMED} ${SELECTION} ${REGION}"
	    eval "./ShortTrackType.exe ${INDIR} ${SAMPLE} ${INDIR_UNSKIMMED} ${SELECTION} ${REGION}"
	done
    done
done

