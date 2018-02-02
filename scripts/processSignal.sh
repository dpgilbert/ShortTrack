#!/bin/bash

#INDIR=/home/users/olivito/mt2_80x/MT2Analysis/MT2looper/output/V00-08-08_nojson_fastsim_skim_base_mt2gt200_ZinvV6_12p9fb_newbtagsfs_newlepsfs_0lcor
INDIR=/home/users/dpgilber/MT2AnalysisFinal16/MT2looper/output/full2016
THISDIR=`pwd`

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

### make signal contamination inputs for ETH

echo "root -b -q sigContamMaker.C+(${INDIR},T1tttt)"
root -b -q "sigContamMaker.C+(\"${INDIR}\",\"T1tttt\")"

echo "root -b -q sigContamMaker.C+(${INDIR},T2tt)"
root -b -q "sigContamMaker.C+(\"${INDIR}\",\"T2tt\")"

### make total signal inputs for ETH

echo "root -b -q convertSNTtoETH.C+(${INDIR},T1tttt_sigcontam)"
root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T1tttt_sigcontam\")"

echo "root -b -q convertSNTtoETH.C+(${INDIR},T1bbbb)"
root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T1bbbb\")"

echo "root -b -q convertSNTtoETH.C+(${INDIR},T1qqqq)"
root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T1qqqq\")"

echo "root -b -q convertSNTtoETH.C+(${INDIR},T2tt_sigcontam)"
root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T2tt_sigcontam\")"

echo "root -b -q convertSNTtoETH.C+(${INDIR},T2bb)"
root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T2bb\")"

echo "root -b -q convertSNTtoETH.C+(${INDIR},T2qq)"
root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T2qq\")"

# echo "root -b -q convertSNTtoETH.C+(${INDIR},T2cc)"
# root -b -q "convertSNTtoETH.C+(\"${INDIR}\",\"T2cc\")"


echo "done"


