#!/bin/bash

MODEL="T1tttt"
#MODEL="T1bbbb"
#MODEL="T1qqqq"
#MODEL="T2tt"
#MODEL="T2bb"
#MODEL="T2-4bd_scan"

DATE="ETH_Feb2"

INDIR="/home/users/mderdzinski/winter2017/clean_master/scripts/cards_${MODEL}_${DATE}"

OUTPUTDIR="/hadoop/cms/store/user/$USER/combine/limits/${MODEL}_${DATE}"

rm $OUTPUTDIR/*

while read i
do
  echo $i
  ./submit.sh $i ${DATE} ${MODEL}
done < $INDIR/points_$MODEL.txt
