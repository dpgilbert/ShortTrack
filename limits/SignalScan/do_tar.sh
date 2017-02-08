#!/bin/bash

MODEL=$1
DATE=$2
INDIR=$3
COMBINED=$4

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
  if [ $COMBINED == 1 ]
  then
      tar -czvf cards_$i.tar.gz "datacard_"*"${i}_combined.txt" >> log_tar.log 2>&1  #for cards already combined
  else
      tar -czvf cards_$i.tar.gz "datacard_"*"${i}.txt" >> log_tar.log 2>&1
  fi
done < $INDIR/points_$MODEL.txt
popd

pushd .
cd $CARD_DIR
cp $INDIR/cards_*.tar.gz .
popd
