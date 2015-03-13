#!/bin/bash

#VERSION=LowLumi_GAMMA
#VERSION=ExtendedNJets_UltraHighHT_GAMMA
#VERSION=SevenJets_UltraHighHT_GAMMA
VERSION=ExtendedNJets_UltraHighHT_9JetsHTIntegrated_GAMMA
INDIR=/hadoop/cms/store/user/$USER/combine/limits/${VERSION}
OUTDIR=Grid_${VERSION}

if [ ! -d "$OUTDIR" ]; then
  mkdir -p "$OUTDIR"
fi

for i in `ls ${INDIR}`
do
  echo $i
  hadd ${OUTDIR}/${i}_Grid.root ${INDIR}/$i/*.root
done
