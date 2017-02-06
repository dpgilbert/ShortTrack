#!/bin/bash

MODEL=T1tttt
#MODEL=T1bbbb
#MODEL=T1qqqq
#MODEL=T2tt
#MODEL=T2bb
#MODEL=T2-4bd_scan

DATE=ETH_Feb2

INDIR=/hadoop/cms/store/user/${USER}/combine/limits/${MODEL}_${DATE}
CARDDIR="/home/users/mderdzinski/winter2017/clean_master/scripts/cards_${MODEL}_${DATE}"

rm r-values_${MODEL}.root

echo ".L make_rValues.C+" > temp
while read i
do
#  echo $i
 MASS1=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[2]')
 MASS2=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[3]')
#  MASS1=$(echo "$i"|awk -F- 'split($0,a,"_")&&$0=a[2]')
#  MASS2=$(echo "$i"|awk -F- 'split($0,a,"_")&&$0=a[3]')
#moved by one to include "_scan"
  # MASS1=$(echo "$i"|awk -F- 'split($0,a,"_")&&$0=a[3]')
  # MASS2=$(echo "$i"|awk -F- 'split($0,a,"_")&&$0=a[4]')
  if [ "$MASS2" == "" ]
  then
    MASS2="0"
  fi
#  echo $MASS1
#  echo $MASS2
#  root -b -q "make_rValues.C(\"$INDIR\",\"$MODEL\",$MASS1,$MASS2)"
  echo "make_rValues(\"$INDIR\",\"$MODEL\",$MASS1,$MASS2)" >> temp

done < $CARDDIR/points_$MODEL.txt
echo ".q" >> temp

cat temp | root -b  &> logMakePlotInput.txt 

rm temp

root -b -q "smooth.C(\"$MODEL\")"
root -b -q "make_contour.C(\"$MODEL\")"
