#!/bin/bash

MODEL=$1
DATE=$2
INDIR=$3
COMBINED=$4

OUTPUTDIR="/hadoop/cms/store/user/$USER/combine/limits/${MODEL}_${DATE}"

rm -r $OUTPUTDIR/*

while read i
do
  echo $i
  ./submit.sh $i ${DATE} ${MODEL} ${COMBINED}
done < $INDIR/points_$MODEL.txt
