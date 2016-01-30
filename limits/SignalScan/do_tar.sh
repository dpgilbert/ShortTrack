#!/bin/bash

#MODEL="T1tttt"
#MODEL="T1bbbb"
#MODEL="T1qqqq"
#MODEL="T2tt"
MODEL="T2bb"
INDIR="/home/users/jgran/limits_for_paper/MT2Analysis/scripts/cards_$MODEL"
CARD_DIR="cards_$MODEL"

if [ -d $CARD_DIR ]
then
  rm -r $CARD_DIR
  mkdir $CARD_DIR
else
  mkdir $CARD_DIR
fi

if [ -e log_tar.log ]
then
  rm log_tar.log
fi

pushd .
cd $INDIR
while read i
do
  echo $i
  tar -czvf cards_$i.tar.gz "datacard_"*"_$i.txt" >> log_tar.log 2>&1
done < $INDIR/points_$MODEL.txt
popd

pushd .
cd $CARD_DIR
cp $INDIR/cards_*.tar.gz .
popd
