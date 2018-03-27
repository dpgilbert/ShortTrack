#!/bin/bash

INDIR=../output/prep2017
declare -a Samples=(total qcd_ht1000to1500  qcd_ht1500to2000_ext1 qcd_ht2000toInf qcd_ht300to500 qcd_ht700to1000 qcd_ht500to700 ttsl_fromT ttsl_fromTbar ttdl wjets_incl zinv_zpt100to200 zinv_zpt200toInf dyjetstoll_incl_ext1)
for SAMPLE in ${Samples[@]}; do
    OUTDIR=pngs_${SAMPLE}
    mkdir -p ${OUTDIR}
    echo "nohup python plotter.py ${INDIR}/${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &"
    eval "nohup python plotter.py ${INDIR}/${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &"
done