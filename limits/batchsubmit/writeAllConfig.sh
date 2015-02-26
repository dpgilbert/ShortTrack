#!/bin/bash

VERSION="LowLumi_GAMMA"
#VERSION="ExtendedNJets_UltraHighHT_GAMMA"
#VERSION="SevenJets_UltraHighHT_GAMMA"
#VERSION="ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"

declare -a samples=("T1tttt_1500_100" "T1tttt_1200_800" "T1bbbb_1500_100" "T1bbbb_1000_900" "T1qqqq_1400_100" "T1qqqq_1000_800" "T2tt_850_100" "T2tt_650_325" "T2tt_500_325" "T2tt_425_325" "T2bb_900_100" "T2bb_600_580" "T2qq_1200_100" "T2qq_600_550")
#declare -a samples=("T1bbbb_1500_100" "T1qqqq_1400_100")
#declare -a samples=("T1tttt_1500_100")

OFF1=0 
for i in "${samples[@]}"
do
  readarray -t points < gridPoints_$i.txt
  OFF2=0
  for j in "${points[@]}"
  do
    seedBegin=$(((${OFF1}*10000)+(${OFF2}*100)))
    echo ${seed}
    ./writeConfig.sh ${VERSION} $i 2 250 5 $j ${seedBegin}
    let OFF2+=1
  done
  let OFF1+=1
done
