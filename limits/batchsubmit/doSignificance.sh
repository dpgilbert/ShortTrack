#!/bin/bash

source setup.sh

#CARD_DIR="2015ExtendedNJets_UltraHighHT_GAMMA_cards"
#TOY_DIR="Toys_ExtendedNJets_UltraHighHT_GAMMA"

#CARD_DIR="2015LowLumi_GAMMA_cards"
#TOY_DIR="Toys_LowLumi_GAMMA"

#CARD_DIR="Cards_SevenJets_UltraHighHT_GAMMA"
#TOY_DIR="Toys_SevenJets_UltraHighHT_GAMMA"

CARD_DIR="Cards_ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"
TOY_DIR="Toys_ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA"

declare -a samples=("T1tttt_1500_100" "T1tttt_1200_800" "T1bbbb_1500_100" "T1bbbb_1000_900" "T1qqqq_1400_100" "T1qqqq_1000_800" "T2tt_850_100" "T2tt_650_325" "T2tt_500_325" "T2tt_425_325" "T2bb_900_100" "T2bb_600_580" "T2qq_1200_100" "T2qq_600_550")
#declare -a samples=("T1tttt_1500_100")

for i in "${samples[@]}"
do
  echo "Significance ${i}"
  combine -M HybridNew --frequentist ${CARD_DIR}/${i}.root --significance --readHybridResult --toysFile=${TOY_DIR}/${i}_Toys.root --expectedFromGrid=0.5
done
