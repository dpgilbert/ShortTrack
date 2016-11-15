#!/bin/bash

INDIR="/nfs-6/userdata/mt2/V00-08-08_nojson_fastsim/"
OUTDIR="/nfs-6/userdata/mt2/V00-08-08_nojson_fastsim/points/"

nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1bbbb\",1900,1,\"T1bbbb_1900_1\")" >& log_skim_T1bbbb_1900_1.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1bbbb\",1000,900,\"T1bbbb_1000_900\")" >& log_skim_T1bbbb_1000_900.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1tttt\",1800,1,\"T1tttt_1800_1\")" >& log_skim_T1tttt_1800_1.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1tttt\",1000,700,\"T1tttt_1000_700\")" >& log_skim_T1tttt_1000_700.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1qqqq\",1700,1,\"T1qqqq_1700_1\")" >& log_skim_T1qqqq_1700_1.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1qqqq\",1000,900,\"T1qqqq_1000_900\")" >& log_skim_T1qqqq_1000_900.txt &

nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2tt_mStop-400to1200\",900,1,\"T2tt_900_1\")" >& log_skim_T2tt_900_1.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2tt_mStop-350to400\",350,250,\"T2tt_350_250\")" >& log_skim_T2tt_350_250.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2bb\",1000,1,\"T2bb_1000_1\")" >& log_skim_T2bb_1000_1.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2bb\",450,400,\"T2bb_450_400\")" >& log_skim_T2bb_450_400.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2qq\",1400,1,\"T2qq_1400_1\")" >& log_skim_T2qq_1400_1.txt &
nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2qq\",650,600,\"T2qq_650_600\")" >& log_skim_T2qq_650_600.txt &
