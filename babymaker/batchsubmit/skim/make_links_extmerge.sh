#! /bin/bash

INPATH=/nfs-6/userdata/mt2/V00-08-08_nojson
OUTPATH=/nfs-6/userdata/mt2/V00-08-08_nojson/extmerge

#### list of samples that have been merged and should be skipped
declare -a Samples=(  ttsl_mg_lo_tbar ttdl_mg_lo wjets_ht200to400 wjets_ht800to1200 wjets_ht1200to2500 dyjetsll_ht600toInf zinv_ht1200to2500 qcd_ht300to500 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf )

for i in $( /bin/ls $INPATH/*.root ); do
    pass=1
    for VETO in ${Samples[@]}; do
	if [[ $i == *"${VETO}"* ]]
	then
	    pass=0
	fi
    done
    if [[ $pass == 1 ]]
    then
	echo "ln -s -t $OUTPATH $i"
	ln -s -t $OUTPATH $i
    fi
done
