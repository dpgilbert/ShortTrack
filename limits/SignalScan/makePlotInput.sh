#!/bin/bash

#MODEL=T1tttt
#MODEL=T1bbbb
#MODEL=T1qqqq
#MODEL=T2tt
MODEL=T2bb
INDIR=/hadoop/cms/store/user/jgran/combine/limits_for_paper/$MODEL
CARDDIR=/home/users/jgran/limits_for_paper/MT2Analysis/scripts/cards_$MODEL

while read i
do
  echo $i
  MASS1=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[2]')
  MASS2=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[3]')
  if [ "$MASS2" == "" ]
  then
    MASS2="0"
  fi
  echo $MASS1
  echo $MASS2
  root -b -q "make_rValues.C(\"$INDIR\",\"$MODEL\",$MASS1,$MASS2)"
done < $CARDDIR/points_$MODEL.txt

root -b -q "smooth.C(\"$MODEL\")" #FIXME
root -b -q "make_contour.C(\"$MODEL\")"
