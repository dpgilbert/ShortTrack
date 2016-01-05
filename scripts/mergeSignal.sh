#!/bin/bash

INDIR=/home/users/olivito/mt2_74x_dev/MT2Analysis/MT2looper/output/V00-01-10_25ns_fastsim_skim_base_mt2gt200_2p155fb_btagsfs_lepsfs_newfastsimsfs_nomonojetid/
THISDIR=`pwd`

if [ ! -d "$INDIR" ]; then
  echo "Input directory does not exist" 
fi

cd $INDIR
echo "hadd -f T1tttt.root T1tttt_0.root T1tttt_1.root T1tttt_2.root T1tttt_3.root T1tttt_4.root T1tttt_5.root T1tttt_6.root"
hadd -f T1tttt.root T1tttt_0.root T1tttt_1.root T1tttt_2.root T1tttt_3.root T1tttt_4.root T1tttt_5.root T1tttt_6.root 

echo "hadd -f T1bbbb.root T1bbbb_0.root T1bbbb_1.root T1bbbb_2.root T1bbbb_3.root T1bbbb_4.root T1bbbb_5.root T1bbbb_6.root"
hadd -f T1bbbb.root T1bbbb_0.root T1bbbb_1.root T1bbbb_2.root  T1bbbb_3.root T1bbbb_4.root T1bbbb_5.root T1bbbb_6.root

echo "hadd -f T1qqqq.root T1qqqq_0.root T1qqqq_1.root T1qqqq_2.root T1qqqq_3.root T1qqqq_4.root T1qqqq_5.root"
hadd -f T1qqqq.root T1qqqq_0.root T1qqqq_1.root T1qqqq_2.root T1qqqq_3.root T1qqqq_4.root T1qqqq_5.root

echo "hadd -f T2tt.root T2tt_0.root T2tt_1.root"
hadd -f T2tt.root T2tt_0.root T2tt_1.root

cd $THISDIR

#this script scales the HI and LOW boundary histograms by 1/numSamples since we don't want these hadd'ed
echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T1tttt.root,7)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T1tttt.root\",7)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T1bbbb.root,7)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T1bbbb.root\",7)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T1qqqq.root,6)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T1qqqq.root\",6)"

echo "root -b -q rescaleBoundaryHists.C+(${INDIR}/T2tt.root,2)"
root -b -q "rescaleBoundaryHists.C+(\"${INDIR}/T2tt.root\",2)"

echo "done"


