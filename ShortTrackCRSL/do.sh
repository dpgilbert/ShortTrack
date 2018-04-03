#!/bin/bash

make all

INDIR=/nfs-6/userdata/dpgilber/MT2_ST_Friends
LOGDIR=logs

mkdir -p ${LOGDIR}

# bitwise: <doNt><doEta><doNj><doLep><doMT2><doDphiDiff><doHTMET><doHT>
# if any bits are high, do Nj > 0
#SELECTION=000000 # Inclusive
#SELECTION=000001 # HT
#SELECTION=000011 # HTMET, same behavior as 00010 = MET
#SELECTION=000111 # HTMETDphiDiff
#SELECTION=001111 # HTMETDphiDiffMT2
#SELECTION=011111 # HTMETDphiDiffMT2Lep
#SELECTION=100000 # Nj > 1
#declare -a SELECTIONS=(0) # inclusive
#declare -a SELECTIONS=(00000 00001 00011 00111 01111 11111)
#declare -a SELECTIONS=(00000 00001 00011 00100 01000 10000 11111)
#declare -a SELECTIONS=(01000) # MT2
#declare -a SELECTIONS=(010000) # Leptons
#declare -a SELECTIONS=(10001) # Leptons and HT
#declare -a SELECTIONS=(110000) # Leptons and Nj > 1
#declare -a SELECTIONS=(010000 010001 110000)
#declare -a SELECTIONS=(010000 010001)
#declare -a SELECTIONS=( 110000 110001)
#declare -a SELECTIONS=(1010001) # HTLepEta
#declare -a SELECTIONS=(0 0010001 1 0010011) # HTMETLep HTLep HT
#declare -a SELECTIONS=(0 00010001) # HTLep
declare -a SELECTIONS=(0 00010001 10001 1) # HTLep, Lep, HT
declare -a Samples=(ttsl_fromT ttsl_fromTbar wjets_incl zinv_zpt100to200 zinv_zpt200toInf qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000_ext1 qcd_ht2000toInf dyjetstoll_incl_ext1 ttdl)
#declare -a Samples=(ttdl)
#declare -a Samples=(wjets_incl)

for SELECTION in ${SELECTIONS[@]}; do
    for SAMPLE in ${Samples[@]}; do
	echo "nohup nice -n 10 ./ShortTrackCRSL.exe ${INDIR} ${SAMPLE} ${SELECTION} >& ${LOGDIR}/${SAMPLE}_${SELECTION}.log &"
	eval "nohup nice -n 10 ./ShortTrackCRSL.exe ${INDIR} ${SAMPLE} ${SELECTION} >& ${LOGDIR}/${SAMPLE}_${SELECTION}.log &"
    done
done
