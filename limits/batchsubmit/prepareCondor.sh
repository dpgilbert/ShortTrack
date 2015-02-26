#!/bin/bash

#INDIR="/home/users/jgran/condor_limits/test/2015LowLumi_GAMMA_cards"
#INDIR="/home/users/jgran/condor_limits/test/2015ExtendedNJets_UltraHighHT_GAMMA_cards"
#INDIR="/home/users/jgran/condor_limits/test/Cards_SevenJets_UltraHighHT_GAMMA"
INDIR="/home/users/jgran/condor_limits/test/Cards_ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"

#OUTDIR="condor_LowLumi_GAMMA"
#OUTDIR="condor_ExtendedNJets_UltraHighHT_GAMMA"
#OUTDIR="condor_SevenJets_UltraHighHT_GAMMA"
OUTDIR="condor_ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"

declare -a samples=("T1tttt_1500_100" "T1tttt_1200_800" "T1bbbb_1500_100" "T1bbbb_1000_900" "T1qqqq_1400_100" "T1qqqq_1000_800" "T2tt_850_100" "T2tt_650_325" "T2tt_500_325" "T2tt_425_325" "T2bb_900_100" "T2bb_600_580" "T2qq_1200_100" "T2qq_600_550")
#declare -a samples=("T1tttt_1500_100")

if [ ! -d "$OUTDIR" ]; then
  mkdir -p "$OUTDIR"
  mkdir -p "$OUTDIR/log"
  mkdir -p "$OUTDIR/job_input"
  mkdir -p "$OUTDIR/cfg"
fi

pushd .
cp combine "$OUTDIR"
cp combine "$OUTDIR/job_input"
cp libHiggsAnalysisCombinedLimit.so "$OUTDIR"
cp libHiggsAnalysisCombinedLimit.so "$OUTDIR/job_input"
cp do_one.sh "$OUTDIR"
cp make_rValues.C "$OUTDIR"
cp getGridPoints.C "$OUTDIR"
cp writeAllConfig.sh "$OUTDIR"
cp writeConfig.sh "$OUTDIR"
cp wrapper.sh "$OUTDIR"
cp submitAll.sh "$OUTDIR"
cd "$OUTDIR"

for i in "${samples[@]}"
do
  cp ${INDIR}/$i.root job_input
  MODEL=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[1]') #because awk
  MASS1=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[2]')
  MASS2=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[3]')
  nohup  ./do_one.sh $i $MODEL $MASS1 $MASS2 $INDIR > $i.log 2>&1 &
  sleep 8
done

cd job_input
tar -czvf input.tar.gz *

popd
