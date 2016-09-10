#!/bin/bash

make -j12

INDIR=/nfs-6/userdata/mt2/RebalanceAndSmear_V00-08-08
# INDIR=/nfs-7/userdata/bemarsh/JRTbabies/v4
OUTDIR=output/mt2babies_noPUID_withBTAG/

declare -a Samples=(qcd_pt80to120)
# declare -a Samples=(qcd_pt15to30 qcd_pt30to50 qcd_pt50to80 qcd_pt80to120 qcd_pt120to170 qcd_pt170to300)
# declare -a Samples=(qcd_pt300to470 qcd_pt470to600 qcd_pt600to800 qcd_pt800to1000 qcd_pt1000to1400 qcd_pt1400to1800 qcd_pt1800to2400 qcd_pt2400to3200 qcd_pt3200toInf)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR}
  nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& log_${SAMPLE}.txt &
done
