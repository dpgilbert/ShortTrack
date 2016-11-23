#! /bin/bash

INPATH=/nfs-6/userdata/mt2/V00-08-09_HLTPFJet450_json_271036-283059_NoL1T_31p24pb/
OUTPATH=/nfs-6/userdata/mt2/V00-08-09_HLTPFJet450_json_271036-283059_NoL1T_31p24pb/trigdenom


#declare -a Samples=( data_Run2016B_MET  data_Run2016B_SingleElectron data_Run2016B_JetHT data_Run2016C_MET  data_Run2016C_SingleElectron data_Run2016C_JetHT data_Run2016D_MET  data_Run2016D_SingleElectron data_Run2016D_JetHT )
#declare -a Samples=( data_Run2016E_MET  data_Run2016E_SingleElectron data_Run2016E_JetHT data_Run2016F_MET  data_Run2016F_SingleElectron data_Run2016F_JetHT data_Run2016G_MET  data_Run2016G_SingleElectron data_Run2016G_JetHT )
declare -a Samples=( data_Run2016H_SingleElectron   )

for SAMPLE in ${Samples[@]}
  do nohup nice -n 19 root -b -q skim_trigdenom.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) >& skimLogs/log_skim_trigdenom_$SAMPLE.txt &
done
