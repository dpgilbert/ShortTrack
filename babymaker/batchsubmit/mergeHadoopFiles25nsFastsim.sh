#!/bin/bash

function run () {
    echo root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\)
    nohup nice -n 19 root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\) >& log_merge_${TAG}_$1.txt &
}

TAG=V00-01-08_25ns_miniaodv2

HADOOPDIR=/hadoop/cms/store/user/${USER}/mt2babies/
OUTPUTDIR=/nfs-6/userdata/mt2/$TAG/

mkdir -p $OUTPUTDIR
chmod -R a+wrx $OUTPUTDIR

#
# T1bbbb
#

run T1bbbb_mGlu-1025-1050_mLSP-800to975-400to800-1000to1025
run T1bbbb_mGlu-1050-1075_mLSP-850to975-850to925-1000to1050
run T1bbbb_mGlu-1250-1275_mLSP-400to950-1175to1200-1050to1175
run T1bbbb_mGluino-1000-1025_mLSP-1to975-1000
run T1bbbb_mGluino-1075_mLSP-950to975
run T1bbbb_mGluino-1100_mLSP-1to1075
run T1bbbb_mGluino-1125-1150_mLSP-900to1100-1000to1075
run T1bbbb_mGluino-1150_mLSP-400to975-1100to1125
run T1bbbb_mGluino-1175_mLSP-950to1150

run T1bbbb_mGluino-1200_mLSP-1to1175
run T1bbbb_mGluino-1225-1250_mLSP-1000to1175-1000to1150
run T1bbbb_mGluino-1300_mLSP-1to1250
run T1bbbb_mGluino-1325-1350_mLSP-1100to1175-1000to1300
run T1bbbb_mGluino-1350-1375_mLSP-1to950-1150to1175
run T1bbbb_mGluino-1400-1450_mLSP-1to1350-1to200
run T1bbbb_mGluino-1450-1500_mLSP-250to1400-1350to1450
run T1bbbb_mGluino-1500-1550_mLSP-1to1300-1000to1300
run T1bbbb_mGluino-1550_mLSP-1to950-1350to1450
run T1bbbb_mGluino-1600_mLSP-1to1450
run T1bbbb_mGluino-1650-1700_mLSP-350to1450-1to650
run T1bbbb_mGluino-1700-1750_mLSP-700to1450-1to650

run T1bbbb_mGluino-1750_mLSP-700to1450
run T1bbbb_mGluino-1800-1850_mLSP-1to1450-1to450
run T1bbbb_mGluino-1850-1900_mLSP-500to1450-1to950
run T1bbbb_mGluino-1900-1950_mLSP-1000to1450-1to1450
run T1bbbb_mGluino-2000_mLSP-1to1450
run T1bbbb_mGluino-600_mLSP-1to300
run T1bbbb_mGluino-600_mLSP-400to475
run T1bbbb_mGluino-600_mLSP-500to575
run T1bbbb_mGluino-625-650_mLSP-525to600-400to450
run T1bbbb_mGluino-650_mLSP-475to625
run T1bbbb_mGluino-675_mLSP-450to650
run T1bbbb_mGluino-700-725-750_mLSP-625to675-500to700-400
run T1bbbb_mGluino-700_mLSP-1to600

run T1bbbb_mGluino-750-775_mLSP-400to725-550to600
run T1bbbb_mGluino-775_mLSP-625to750
run T1bbbb_mGluino-800-825_mLSP-1to775-600
run T1bbbb_mGluino-825-850_mLSP-625to800-400to700
run T1bbbb_mGluino-850-875_mLSP-725to825-650to825
run T1bbbb_mGluino-875-900-925_mLSP-850-1to875-700
run T1bbbb_mGluino-925-950_mLSP-725to900-400to800
run T1bbbb_mGluino-950-975_mLSP-825to925-750to925
run T1bbbb_mGluino-975_mLSP-950
