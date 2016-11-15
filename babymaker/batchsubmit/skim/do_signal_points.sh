#!/bin/bash

#INDIR="/nfs-6/userdata/mt2/V00-01-10_25ns_fastsim_fixpfmet/"
#OUTDIR="/nfs-6/userdata/mt2/V00-01-10_25ns_fastsim_fixpfmet/mass_points/"

# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2bb_mSbottom-625to1050_0to550\",800,0,\"T2bb_800_1\")" >& log_skim_T2bb_800_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2bb_mSbottom-425to600_0to575\",500,350,\"T2bb_500_350\")" >& log_skim_T2bb_800_1.txt &

# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2qq_mSq950to1250_mLSP-0to1050\",1200,0,\"T2qq_1200_1\")" >& log_skim_T2bb_800_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2qq_mSq600to900_mLSP-0to850\",600,0,\"T2qq_600_1\")" >& log_skim_T2qq_600_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2qq_mSq600to900_mLSP-0to850\",700,500,\"T2qq_700_500\")" >& log_skim_T2qq_700_500.txt &

# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2tt_mStop-600-950_mLSP-1to450\",750,0,\"T2tt_750_1\")" >& log_skim_T2tt_750_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2tt_mStop-600-950_mLSP-1to450\",600,250,\"T2tt_600_250\")" >& log_skim_T2tt_600_250.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T2tt_mStop-250_mLSP-1to175\",250,150,\"T2tt_250_150\")" >& log_skim_T2tt_250_150.txt &

# INDIR="/nfs-6/userdata/mt2/V00-01-10_25ns_fastsim/"
# OUTDIR="/nfs-6/userdata/mt2/V00-01-10_25ns_fastsim/mass_points/"

# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1bbbb_mGluino-1650-1700_mLSP-350to1450-1to650\",1700,0,\"T1bbbb_1700_1\")" >& log_skim_T1bbbb_1700_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1bbbb_mGluino-1000-1025_mLSP-1to975-1000\",1000,950,\"T1bbbb_1000_950\")" >& log_skim_T1bbbb_1000_950.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1qqqq_mGluino-1600to1650_mLSP-1to1250\",1600,0,\"T1qqqq_1600_1\")" >& log_skim_T1qqqq_1600_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1qqqq_mGluino-1000to1025_mLSP-1to1000\",1000,850,\"T1qqqq_1000_850\")" >& log_skim_T1qqqq_1000_850.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1tttt_mGluino-1475to1500_mLSP-1to1250\",1500,0,\"T1tttt_1500_1\")" >& log_skim_T1tttt_1500_1.txt &
# nohup root -b -q "skim_signal_point.C(\"${INDIR}\",\"${OUTDIR}\",\"T1tttt_mGluino-875to900_mLSP-1to650\",900,600,\"T1tttt_900_600\")" >& log_skim_T1tttt_900_600.txt &

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
