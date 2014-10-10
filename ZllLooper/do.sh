#!/bin/bash

#INDIR=/nfs-6/userdata/mt2/V00-00-05/
#INDIR=/home/users/gzevi/MT2/MT2Analysis/babymaker/batchsubmit/merged/skim_gammaOrTwoLep
#INDIR=/nfs-6/userdata/gzevi/mt2/ZllStudiesBabiesV00-00-05/skim_gammaOrTwoLep
#INDIR=/nfs-6/userdata/gzevi/mt2/ZllStudiesBabiesV00-00-05/
#INDIR=/nfs-6/userdata/gzevi/mt2/V00-00-04GenJets/
INDIR=/nfs-6/userdata/gzevi/mt2/V00-00-04GenJetsGammaIDNew2/
OUTDIR=output/test/

declare -a Samples=(dyjetsll_ht)
#declare -a Samples=(dyjetsll_ht zinv_ht)
#declare -a Samples=(dyjetsll_ht600)
#declare -a Samples=(gjet_ht100 gjet_ht200 gjet_ht400 gjet_ht600 )
#declare -a Samples=(dyjetsll_ht100 dyjetsll_ht200 dyjetsll_ht400 dyjetsll_ht600 )
#declare -a Samples=(dyjetsll_ht100 dyjetsll_ht200 dyjetsll_ht400 dyjetsll_ht600 zinv_ht100 zinv_ht200 zinv_ht400 zinv_ht600)
#declare -a Samples=(zinv_ht100 zinv_ht200 zinv_ht400 zinv_ht600)
#declare -a Samples=(zinv_ht100)
#declare -a Samples=(dyjetsll_ht100 dyjetsll_ht200 dyjetsll_ht400 dyjetsll_ht600 gjet_ht100 gjet_ht200 gjet_ht400 gjet_ht600 )
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1bbbb_1500_100 T1bbbb_1000_900)

# Running over the GammaID samples
#INDIR=/home/users/gzevi/MT2/MT2Analysis/babymaker/batchsubmit/merged/
#OUTDIR=output/test/newGammaID/
#declare -a Samples=(gjet_ht)


mkdir -p ${OUTDIR}

for SAMPLE in ${Samples[@]};
  do echo root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\)
  nohup root -b -q -l doAll.C\(\"${INDIR}\",\"${SAMPLE}\",\"${OUTDIR}\"\) >& log_${SAMPLE}.txt &
done