#!/bin/bash

#INDIR=/nfs-6/userdata/mt2/V00-00-05/
#INDIR=/home/users/gzevi/MT2/MT2Analysis/babymaker/batchsubmit/merged/skim_gammaOrTwoLep
#INDIR=/nfs-6/userdata/gzevi/mt2/ZllStudiesBabiesV00-00-05/skim_gammaOrTwoLep
#INDIR=/nfs-6/userdata/gzevi/mt2/ZllStudiesBabiesV00-00-05/
#INDIR=/nfs-6/userdata/gzevi/V00-00-04GenJets/
#INDIR=/nfs-6/userdata/gzevi/mt2/V00-00-05GammaID/

OUTDIR=output/test/NoGammaID/

#declare -a Samples=(QCD)
#declare -a Samples=(QCD_PT30 QCD_PT50 QCD_PT80 QCD_PT120 QCD_PT170 QCD_PT300 QCD_PT300 QCD_PT470 QCD_PT600 QCD_PT800 QCD_PT1000 QCD_PT1400 QCD_PT1800)
#declare -a Samples=(dyjetsll_ht gjet_ht)
#declare -a Samples=(gjet_ht)
#declare -a Samples=(gjet_ht100 gjet_ht200 gjet_ht400 gjet_ht600 )
#declare -a Samples=(dyjetsll_ht100 dyjetsll_ht200 dyjetsll_ht400 dyjetsll_ht600 )
#declare -a Samples=(dyjetsll_ht100 dyjetsll_ht200 dyjetsll_ht400 dyjetsll_ht600 gjet_ht100 gjet_ht200 gjet_ht400 gjet_ht600 )
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1bbbb_1500_100 T1bbbb_1000_900)

# Running over the GammaID samples
#INDIR=/home/users/gzevi/MT2/MT2Analysis/babymaker/batchsubmit/merged/NoGammaID
#declare -a Samples=(gjet_ht)

#run over QCD samples
#INDIR=/nfs-6/userdata/gzevi/mt2/V00-00-05GammaID/
#declare -a Samples=(QCD)
#declare -a Samples=(QCD_PT30 QCD_PT50 QCD_PT80 QCD_PT120 QCD_PT170 QCD_PT300 QCD_PT300 QCD_PT470 QCD_PT600 QCD_PT800 QCD_PT1000 QCD_PT1400 QCD_PT1800)

#run over gjets sample with stat 1111 gen particles test file
#INDIR=/nfs-6/userdata/gzevi/mt2/V00-00-06packedGenNoGammaID
#declare -a Samples=(gjet_ht100to200)

#run over qcd sample with stat 1111 gen particles test file
#INDIR=/hadoop/cms/store/user/gzevi/mt2babies/V00-00-06packedGenNoGammaID_qcd_pt170to300
#declare -a Samples=(qcd_pt170to300)

#run over full gjets and qcd samples with stat1111 gen particles
INDIR=/nfs-6/userdata/gzevi/mt2/V00-00-06packedGenNoGammaID
declare -a Samples=(gjet qcd)
#declare -a Samples=(qcd_pt30 qcd_pt50 qcd_pt80 qcd_pt120 qcd_pt170 qcd_pt300 qcd_pt300 qcd_pt470 qcd_pt600 qcd_pt800 qcd_pt1000 qcd_pt1400 qcd_pt1800)

mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\)
  nohup root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\) >& logs/log_${SAMPLE}.log &
done