#!/bin/bash

INDIR=/home/users/olivito/mt2_80x/MT2Analysis/MT2looper/output/V00-08-08_nojson_fastsim_skim_base_mt2gt200_ZinvV6_12p9fb_newbtagsfs_newlepsfs_0lcor
THISDIR=`pwd`

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR

echo "hadd -f T1tttt.root T1tttt_0.root T1tttt_1.root T1tttt_2.root T1tttt_3.root T1tttt_4.root T1tttt_5.root "
hadd -f T1tttt.root T1tttt_0.root T1tttt_1.root T1tttt_2.root T1tttt_3.root T1tttt_4.root T1tttt_5.root  

echo "hadd -f T1bbbb.root T1bbbb_0.root T1bbbb_1.root T1bbbb_2.root "
hadd -f T1bbbb.root T1bbbb_0.root T1bbbb_1.root T1bbbb_2.root  

echo "hadd -f T1qqqq.root T1qqqq_0.root T1qqqq_1.root T1qqqq_2.root "
hadd -f T1qqqq.root T1qqqq_0.root T1qqqq_1.root T1qqqq_2.root 

echo "hadd -f T2tt.root T2tt_0.root T2tt_1.root T2tt_2.root T2tt_3.root"
hadd -f T2tt.root T2tt_0.root T2tt_1.root T2tt_2.root T2tt_3.root

echo "hadd -f T2bb.root T2bb_0.root T2bb_1.root"
hadd -f T2bb.root T2bb_0.root T2bb_1.root

echo "hadd -f T2qq.root T2qq_0.root T2qq_1.root T2qq_2.root"
hadd -f T2qq.root T2qq_0.root T2qq_1.root T2qq_2.root

# echo "hadd -f T2cc.root T2cc_0.root T2cc_1.root"
# hadd -f T2cc.root T2cc_0.root T2cc_1.root

cd $THISDIR

### this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T1tttt.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T1tttt.root\",6)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T1bbbb.root,3)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T1bbbb.root\",3)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T1qqqq.root,3)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T1qqqq.root\",3)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T2tt.root,4)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T2tt.root\",4)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T2bb.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T2bb.root\",2)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T2qq.root,3)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T2qq.root\",3)"

# echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T2cc.root,2)"
# root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T2cc.root\",2)"

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


