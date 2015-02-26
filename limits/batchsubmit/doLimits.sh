#!/bin/bash

source setup.sh

#CARD_DIR="2015LowLumi_GAMMA_cards"
#GRID_DIR="Grid_LowLumi_GAMMA"

#CARD_DIR="2015ExtendedNJets_UltraHighHT_GAMMA_cards"
#GRID_DIR="Grid_ExtendedNJets_UltraHighHT_GAMMA"

#CARD_DIR="Cards_SevenJets_UltraHighHT_GAMMA"
#GRID_DIR="Grid_SevenJets_UltraHighHT_GAMMA"

CARD_DIR="Cards_ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"
GRID_DIR="Grid_ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"

declare -a samples=("T1tttt_1500_100" "T1tttt_1200_800" "T1bbbb_1500_100" "T1bbbb_1000_900" "T1qqqq_1400_100" "T1qqqq_1000_800" "T2tt_850_100" "T2tt_650_325" "T2tt_500_325" "T2tt_425_325" "T2bb_900_100" "T2bb_600_580" "T2qq_1200_100" "T2qq_600_550")
#declare -a samples=("T1tttt_1500_100")

#VERSION="LowLumi_GAMMA"
#VERSION="ExtendedNJets_UltraHighHT_GAMMA"
#VERSION="SevenJets_UltraHighHT_GAMMA"
VERSION="ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"

> limits_${VERSION}.txt

for i in "${samples[@]}"
do
  echo "Computing limit for " "${i}"
  combine -M HybridNew --frequentist ${CARD_DIR}/${i}.root --readHybridResult --grid=${GRID_DIR}/${i}_Grid.root --expectedFromGrid=0.5 -n _${i} > /dev/null 2>&1
  root -b -q "printLimit.C(\"${i}\", \"${VERSION}\")" > /dev/null 2>&1
done
