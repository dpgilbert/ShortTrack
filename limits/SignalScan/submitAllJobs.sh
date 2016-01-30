#!/bin/bash

#MODEL="T1tttt"
#MODEL="T1bbbb"
#MODEL="T1qqqq"
#MODEL="T2tt"
MODEL="T2bb"
INDIR="/home/users/jgran/limits_for_paper/MT2Analysis/scripts/cards_$MODEL"

while read i
do
  echo $i
  ./submit.sh $i 
done < $INDIR/points_$MODEL.txt
