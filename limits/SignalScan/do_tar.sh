#!/bin/bash

MODEL="T1tttt"
#MODEL="T1bbbb"
#MODEL="T1qqqq"
#MODEL="T2tt"
#MODEL="T2bb"
#MODEL="T2-4bd_scan" #Need to remove points beyond 400 by hand from points_$MODEL

DATE="ETH_Feb2"

INDIR="/home/users/mderdzinski/winter2017/clean_master/scripts/cards_${MODEL}_${DATE}"
CARD_DIR="cards_${MODEL}_${DATE}"

if [ -d $CARD_DIR ]
then
  rm -r $CARD_DIR
  mkdir  $CARD_DIR
else
  mkdir  $CARD_DIR
fi

pushd .
cd $INDIR

if [ -e log_tar.log ]
then
  rm log_tar.log
fi

while read i
do
  echo $i
  tar -czvf cards_$i.tar.gz "datacard_"*"${i}.txt" >> log_tar.log 2>&1
  #tar -czvf cards_$i.tar.gz "datacard_"*"${i}_combined.txt" >> log_tar.log 2>&1  #for cards already combined
done < $INDIR/points_$MODEL.txt
popd

pushd .
cd $CARD_DIR
cp $INDIR/cards_*.tar.gz .
popd
