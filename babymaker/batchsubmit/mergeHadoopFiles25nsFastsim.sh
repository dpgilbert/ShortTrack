#!/bin/bash

function run () {
    echo root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\)
    nohup nice -n 19 root -b -q mergeHadoopFiles.C\(\"${HADOOPDIR}/${TAG}_$1/\",\"${OUTPUTDIR}/$1.root\"\) >& log_merge_${TAG}_$1.txt &
}

TAG=V00-01-10_25ns_fastsim

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

#
# T1tttt
#

run T1tttt_mGluino-1000_mLSP-1to700
run T1tttt_mGluino-1000to1050_mLSP-1to800
run T1tttt_mGluino-1050_mLSP-50to775
run T1tttt_mGluino-1100_mLSP-1to775
run T1tttt_mGluino-1100to1125_mLSP-700to900
run T1tttt_mGluino-1150_mLSP-1to800
run T1tttt_mGluino-1150to1175_mLSP-750to925

run T1tttt_mGluino-1175_mLSP-950
run T1tttt_mGluino-1200_mLSP-1to825
run T1tttt_mGluino-1200to1225_mLSP-800to1000
run T1tttt_mGluino-1225to1250_mLSP-1to1025
run T1tttt_mGluino-1250to1275_mLSP-700to1050
run T1tttt_mGluino-1275_mLSP-900to975
run T1tttt_mGluino-1300_mLSP-1to1075
run T1tttt_mGluino-1300to1325_mLSP-700to1100

run T1tttt_mGluino-1325to1350_mLSP-1to1125
run T1tttt_mGluino-1350to1375_mLSP-50to1025
run T1tttt_mGluino-1375_mLSP-950to1150
run T1tttt_mGluino-1400_mLSP-1to1175
run T1tttt_mGluino-1400to1425_mLSP-50to1100
run T1tttt_mGluino-1425to1450_mLSP-1to1200
run T1tttt_mGluino-1450to1475_mLSP-50to1075
run T1tttt_mGluino-1475to1500_mLSP-1to1250
run T1tttt_mGluino-1500to1525_mLSP-50to1125
run T1tttt_mGluino-1525to1550_mLSP-1to1300

run T1tttt_mGluino-1550to1575_mLSP-500to1175
run T1tttt_mGluino-1600to1650_mLSP-1to1350
run T1tttt_mGluino-1650to1700_mLSP-1to1400
run T1tttt_mGluino-1700to1750_mLSP-1to1450
run T1tttt_mGluino-1750_mLSP-50to1450
run T1tttt_mGluino-1800to1850_mLSP-1to1450
run T1tttt_mGluino-1850to1900_mLSP-1to1450
run T1tttt_mGluino-1900to1950_mLSP-0to1450
run T1tttt_mGluino-1950_mLSP-700to950
run T1tttt_mGluino-600_mLSP-1to225

run T1tttt_mGluino-600_mLSP-250to325
run T1tttt_mGluino-600to625_mLSP-250to375
run T1tttt_mGluino-625_mLSP-275to375
run T1tttt_mGluino-625to650_mLSP-200to400
run T1tttt_mGluino-650to675_mLSP-250to425
run T1tttt_mGluino-675_mLSP-325to450
run T1tttt_mGluino-700_mLSP-1to450
run T1tttt_mGluino-700to750_mLSP-200to500

run T1tttt_mGluino-750to775_mLSP-350to525
run T1tttt_mGluino-775_mLSP-475to550
run T1tttt_mGluino-800to825_mLSP-1to575
run T1tttt_mGluino-825to850_mLSP-200to600
run T1tttt_mGluino-850to875_mLSP-450to625
run T1tttt_mGluino-875to900_mLSP-1to650
run T1tttt_mGluino-900to950_mLSP-200to700
run T1tttt_mGluino-950to975_mLSP-350to725
run T1tttt_mGluino-975_mLSP-600to750


#
# T1qqqq
#

run T1qqqq_mGluino-1000to1025_mLSP-1to1000
run T1qqqq_mGluino-1025to1075_mLSP-400to1050
run T1qqqq_mGluino-1075_mLSP-925to975
run T1qqqq_mGluino-1100to1125_mLSP-1to1100
run T1qqqq_mGluino-1125to1175_mLSP-925to1125
run T1qqqq_mGluino-1175_mLSP-975to1150
run T1qqqq_mGluino-1200to1225_mLSP-1to1175

run T1qqqq_mGluino-1225to1250_mLSP-1to1225
run T1qqqq_mGluino-1250to1275_mLSP-550to1250
run T1qqqq_mGluino-1300to1325_mLSP-1to1275
run T1qqqq_mGluino-1325to1350_mLSP-1to1275
run T1qqqq_mGluino-1350to1375_mLSP-650to1275
run T1qqqq_mGluino-1400to1450_mLSP-1to1275
run T1qqqq_mGluino-1450to1500_mLSP-1to1275
run T1qqqq_mGluino-1500to1550_mLSP-1to1250

run T1qqqq_mGluino-1550_mLSP-950
run T1qqqq_mGluino-1600to1650_mLSP-1to1250
run T1qqqq_mGluino-1650to1750_mLSP-1to1250
run T1qqqq_mGluino-1750_mLSP-50to1250
run T1qqqq_mGluino-1800to1850_mLSP-1to1250
run T1qqqq_mGluino-1850to1950_mLSP-1to1250
run T1qqqq_mGluino-1950to2000_mLSP-1to1250
run T1qqqq_mGluino-2000_mLSP-50to950

run T1qqqq_mGluino-600_mLSP-1to300
run T1qqqq_mGluino-600_mLSP-400to475
run T1qqqq_mGluino-600_mLSP-500to575
run T1qqqq_mGluino-625_mLSP-425to525
run T1qqqq_mGluino-625to650_mLSP-475to550
run T1qqqq_mGluino-650to675_mLSP-475to500
run T1qqqq_mGluino-675_mLSP-500to650

run T1qqqq_mGluino-700_mLSP-1to575
run T1qqqq_mGluino-700to750_mLSP-400to700
run T1qqqq_mGluino-750to775_mLSP-450to750
run T1qqqq_mGluino-800to825_mLSP-1to750
run T1qqqq_mGluino-825to875_mLSP-400to825
run T1qqqq_mGluino-875to925_mLSP-1to850
run T1qqqq_mGluino-925to975_mLSP-400to900
run T1qqqq_mGluino-925to975_mLSP-950

#
# T2tt
#

run T2tt_mStop-100-125_mLSP-1to50
run T2tt_mStop-150-175_mLSP-1to100
run T2tt_mStop-200_mLSP-1to125
run T2tt_mStop-225_mLSP-25to150
run T2tt_mStop-250_mLSP-1to175
run T2tt_mStop-275_mLSP-75to200
run T2tt_mStop-300to375_mLSP-1to300
run T2tt_mStop-400to475_mLSP-1to400
run T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475
run T2tt_mStop-600-950_mLSP-1to450

#
# T2bb
#

run T2bb_mSbottom-300to400_0to375
run T2bb_mSbottom-425to600_0to575
run T2bb_mSbottom-625to1050_0to550

#
# T2cc
#

run T2cc_mStop-175to225_mLSP-175to215
run T2cc_mStop-250to300_mLSP-170to290
run T2cc_mStop-400to450_mLSP-320to440
run T2cc_mStop-475to525_mLSP-395to515
run T2cc_mStop-550to600_mLSP-470to590

#
# T2qq
#

run T2qq_mSq1300to1550_mLSP-0to1050
run T2qq_mSq300to400_mLSP-0to350
run T2qq_mSq450to550_mLSP-0to500
run T2qq_mSq600to900_mLSP-0to850
run T2qq_mSq950to1250_mLSP-0to1050
