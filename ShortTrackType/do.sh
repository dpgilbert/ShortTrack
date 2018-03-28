#!/bin/bash

make all

INDIR=/nfs-6/userdata/dpgilber/MT2_ST_Friends_incl_skim
INDIR_UNSKIMMED=/nfs-6/userdata/dpgilber/MT2_ST_Friends
#OUTDIR=output_incl
#OUTDIR=output_HT
#OUTDIR=output_HTMET
#OUTDIR=output_HTMETDphiDiff
OUTDIR=output_HTMETDphiDiffMT2
# bitwise: <doMT2><doDphiDiff><doHTMET><doHT>
# if any bits are high, do Nj > 0
#SELECTION=0000 # Inclusive
#SELECTION=0001 # HT
#SELECTION=0011 # HTMET
#SELECTION=0111 # HTMETDphiDiff
SELECTION=1111 # HTMETDphiDiffMT2
LOGDIR=logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(ttsl_fromT ttsl_fromTbar wjets_incl zinv_zpt100to200 zinv_zpt200toInf qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000_ext1 qcd_ht2000toInf ttdl dyjetstoll_incl_ext1)
#declare -a Samples=(ttdl)

for SAMPLE in ${Samples[@]}; do
    echo "nohup nice -n 10 ./ShortTrackType.exe ${INDIR} ${SAMPLE} ${OUTDIR} ${INDIR_UNSKIMMED} ${SELECTION} >& ${LOGDIR}/${SAMPLE}.log &"
    eval "nohup nice -n 10 ./ShortTrackType.exe ${INDIR} ${SAMPLE} ${OUTDIR} ${INDIR_UNSKIMMED} ${SELECTION} >& ${LOGDIR}/${SAMPLE}.log &"
done