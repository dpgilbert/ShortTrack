#! /bin/bash


#INPATH=/nfs-6/userdata/mt2/bemarsh_V00-08-00
#OUTPATH=/nfs-6/userdata/mt2/V00-08-00_json_Cert_271036-273450_skim_base_mt2gt200_ZinvV4

# INPATH=/nfs-6/userdata/mt2/76x_miniaodv2_test_v5
# OUTPATH=/nfs-6/userdata/mt2/bemarsh_76X_test_skim_tmp

#INPATH=/nfs-6/userdata/mt2/V00-01-10_25ns_data_json_246908-260627_v2
#OUTPATH=/nfs-6/userdata/mt2/V00-01-10_25ns_data_json_246908-260627_v2_skim_base_mt2gt200_ZinvV4

#INPATH=/nfs-6/userdata/mt2/V00-08-01_json_Cert_271036-274421
# OUTPATH=/nfs-6/userdata/mt2/V00-08-01_json_Cert_271036-274421_skim_base_mt2gt200_ZinvV4
#OUTPATH=/nfs-6/userdata/mt2/bemarsh_test

#INPATH=/nfs-6/userdata/mt2/V00-08-02_json_Cert_271036-274421
#INPATH=/nfs-6/userdata/mt2/V00-08-02_json_Cert_271036-274421/extmerge
#OUTPATH=/nfs-6/userdata/mt2/V00-08-02_json_Cert_271036-274421_skim_base_mt2gt200_ZinvV4

# INPATH=/nfs-6/userdata/mt2/V00-08-02_nojson
#INPATH=/nfs-6/userdata/mt2/V00-08-02_nojson/extmerge
#INPATH=/hadoop/cms/store/user/olivito/mt2babies/V00-08-02_nojson_data_Run2016B_SingleElectron_PromptReco
#OUTPATH=/nfs-6/userdata/mt2/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4

#INPATH=/nfs-6/userdata/mt2/V00-08-03_MiniAODv2_nojson
#INPATH=/nfs-6/userdata/mt2/V00-08-04_MiniAODv2_nojson/extmerge
#INPATH=/nfs-6/userdata/mt2/V00-08-05_nojson/extmerge
#OUTPATH=/nfs-6/userdata/mt2/V00-08-05_nojson_skim_base_mt2gt200_ZinvV4
#OUTPATH=/nfs-6/userdata/mt2/V00-08-05_nojson_skim_base_mt2gt100_ZinvV4

#INPATH=/nfs-6/userdata/mt2/V00-08-04_MiniAODv2_nojson
#OUTPATH=/nfs-6/userdata/mt2/V00-08-04_MiniAODv2_nojson_skim_base_mt2gt100_ZinvV4

#INPATH=/nfs-6/userdata/mt2/V00-08-07_nojson
#OUTPATH=/nfs-6/userdata/mt2/V00-08-07_nojson_skim_base_mt2gt200_ZinvV6

# INPATH=/nfs-6/userdata/mt2/V00-08-06_nojson_bemarsh
# OUTPATH=/nfs-6/userdata/mt2/V00-08-06_nojson_bemarsh_skim_base_mt2gt200_ZinvV4

#INPATH=/nfs-6/userdata/mt2/2015results_eta2p4_v2
#OUTPATH=/nfs-6/userdata/mt2/2015results_eta2p4_v2_skim_base_mt2gt200_ZinvV4

#INPATH=/nfs-6/userdata/mt2/V00-08-07_nojson_jec_UP/extmerge
#OUTPATH=/nfs-6/userdata/mt2/V00-08-07_nojson_jec_UP_skim_base_mt2gt200_ZinvV6

#INPATH=/nfs-6/userdata/mt2/V00-08-07_nojson_fastsim_fixisrweight
#OUTPATH=/nfs-6/userdata/mt2/V00-08-07_nojson_fastsim_fixisrweight_skim_base_mt2gt200_ZinvV6

#INPATH=/nfs-6/userdata/mt2/V00-08-08_nojson/others
#OUTPATH=/nfs-6/userdata/mt2/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/others

#INPATH=/nfs-6/userdata/mt2/V00-08-08_nojson_fastsim
#OUTPATH=/nfs-6/userdata/mt2/V00-08-08_nojson_fastsim_skim_base_mt2gt200_ZinvV6

#INPATH=/nfs-6/userdata/mt2/V00-08-08_json_271036-278808_NoL1T_20p1fb
#OUTPATH=/nfs-6/userdata/mt2/V00-08-08_json_271036-278808_NoL1T_20p1fb_skim_base_mt2gt200_ZinvV6

# INPATH=/nfs-6/userdata/mt2/V00-08-09_json_271036-280385_NoL1T_24p6fb
# OUTPATH=/nfs-6/userdata/mt2/V00-08-09_json_271036-280385_NoL1T_24p6fb_skim_base_mt2gt200_ZinvV6

INPATH=/nfs-6/userdata/mt2/V00-08-09_json_271036-283059_NoL1T_31p24pb
OUTPATH=/nfs-6/userdata/mt2/V00-08-09_json_271036-283059_NoL1T_31p24pb_skim_base_mt2gt200_ZinvV6

#declare -a Samples=(data_Run2016B_MuonEG data_Run2016B_MET data_Run2016B_HTMHT data_Run2016B_DoubleMuon data_Run2016B_SingleMuon data_Run2016B_SingleElectron data_Run2016B_JetHT data_Run2016B_DoubleEG data_Run2016B_SinglePhoton )
#declare -a Samples=(data_Run2016B_JetHT)
#declare -a Samples=(data_Run2016B_MuonEG  data_Run2016B_HTMHT data_Run2016B_DoubleMuon  data_Run2016B_SingleMuon )
#declare -a Samples=(data_Run2016B_JetHT  data_Run2016B_MET data_Run2016B_DoubleEG  data_Run2016B_SinglePhoton )
#declare -a Samples=(data_Run2016C_SinglePhoton data_Run2016D_SingleElectron)
#declare -a Samples=(data_Run2016C_MuonEG data_Run2016C_MET data_Run2016C_HTMHT data_Run2016C_DoubleMuon data_Run2016C_SingleMuon data_Run2016C_SingleElectron data_Run2016C_JetHT data_Run2016C_DoubleEG data_Run2016C_SinglePhoton )
#declare -a Samples=(data_Run2016D_MuonEG data_Run2016D_MET data_Run2016D_HTMHT data_Run2016D_DoubleMuon data_Run2016D_SingleMuon data_Run2016D_SingleElectron data_Run2016D_JetHT data_Run2016D_DoubleEG data_Run2016D_SinglePhoton )
#declare -a Samples=(data_Run2016E_MuonEG data_Run2016E_MET data_Run2016E_HTMHT data_Run2016E_DoubleMuon data_Run2016E_SingleMuon data_Run2016E_SingleElectron data_Run2016E_JetHT data_Run2016E_DoubleEG data_Run2016E_SinglePhoton )
#declare -a Samples=(data_Run2016F_MuonEG data_Run2016F_MET data_Run2016F_HTMHT data_Run2016F_DoubleMuon data_Run2016F_SingleMuon data_Run2016F_SingleElectron data_Run2016F_JetHT data_Run2016F_DoubleEG data_Run2016F_SinglePhoton )
#declare -a Samples=(data_Run2016G_MuonEG data_Run2016G_MET data_Run2016G_HTMHT data_Run2016G_DoubleMuon data_Run2016G_SingleMuon data_Run2016G_SingleElectron data_Run2016G_JetHT data_Run2016G_DoubleEG data_Run2016G_SinglePhoton )
declare -a Samples=(data_Run2016H_MuonEG data_Run2016H_MET data_Run2016H_HTMHT data_Run2016H_DoubleMuon data_Run2016H_SingleMuon data_Run2016H_SingleElectron data_Run2016H_JetHT data_Run2016H_DoubleEG data_Run2016H_SinglePhoton )

#declare -a Samples=(wjets_amcatnlo zjets_amcatnlo)
#declare -a Samples=(wjets_ht zinv_ht qcd_ht singletop ttw ttz ttg ttdl_mg_lo ttsl_mg_lo)
#declare -a Samples=(wjets_ht zinv_ht singletop ttw ttz ttg ttdl_mg_lo ttsl_mg_lo )
#declare -a Samples=(wjets_ht zinv_ht qcd_ht singletop ttw ttz ttg ttdl_mg_lo ttsl_mg_lo wjets_incl dyjetsll_incl gjets_ht)
#declare -a Samples=(wjets_ht zinv_ht singletop ttw ttz ttg ttdl_mg_lo ttsl_mg_lo dyjetsll_incl dyjetsll_ht gjets_dr0p4_ht gjets_dr0p05_ht qcd_ht )
#declare -a Samples=(wjets_ht zinv_ht singletop ttw ttz ttg ttdl_mg_lo dyjetsll_incl dyjetsll_ht gjets_dr0p4_ht gjets_dr0p05_ht qcd_ht )
#declare -a Samples=( singletop ttw ttz ttg ttdl_mg_lo wjets_ht )
#declare -a Samples=(  ttsl_mg_lo )
#declare -a Samples=(gjet_ht qcd_ht dyjetsll_ht zinv_ht wjets_ht)
#declare -a Samples=(data_Run2016B)
#declare -a Samples=(gjets_dr0p05_ht)
#declare -a Samples=(wjets_ht zinv_ht qcd_ht ttsl_mg_lo dyjetsll_ht)
#declare -a Samples=(ttw_mg_lo ttz_mg_lo)

#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1bbbb_1500_100 )
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1qqqq_1000_800 T1bbbb_1500_100 T1bbbb_1000_900 T2tt_650_325 T2tt_850_100 T2tt_500_325 T2tt_425_325 T2qq_600_550 T2bb_600_580 T2qq_1200_100 T2bb_900_100)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1qqqq_1000_800 T1bbbb_1500_100 T1bbbb_1000_900)
#declare -a Samples=(T2tt_650_325 T2tt_850_100 T2tt_500_325 T2tt_425_325 T2qq_600_550 T2bb_600_580 T2qq_1200_100 T2bb_900_100)
#declare -a Samples=(T1tttt_1500_100 )
#declare -a Samples=(T1qqqq T1bbbb)
#declare -a Samples=(T1qqqq T1tttt T1bbbb T2tt T2bb T2cc T2qq )
#declare -a Samples=(T2cc )
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800  T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800)
#declare -a Samples=( T1tttt_1200_800  T1bbbb_1500_100 T1bbbb_1000_900)
#declare -a Samples=(T1tttt T2tt)
#declare -a Samples=( T2bb)
#declare -a Samples=(T1qqqq T1tttt T1bbbb T2tt T2bb )
#declare -a Samples=(T1qqqq T1tttt T1bbbb T2tt  )
# declare -a Samples=(T1qqqq T1bbbb T2bb T2qq  )

mkdir -p skimLogs_$USER

for SAMPLE in ${Samples[@]}
  #do root -b -q -l skim_njets30_ht_met_mt2_Zinv.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) &
  #do root -b -q -l skim_trig_njets30_ht_met_mt2_Zinv_v3.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) &
  #do root -b -q -l skim_njets30_ht_met_mt2_Zinv_v3.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) &
#   do nohup root -b -q -l skim_base_mt2_Zinv_v4.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) >& skimLogs_$USER/log_${SAMPLE}.txt &
  do nohup root -b -q -l skim_base_mt2_Zinv_v6.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) >& skimLogs_$USER/log_${SAMPLE}.txt &
  #do root -b -q -l skim_base_mt2_Zinv_v5.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) &
  #do root -b -q -l skim_base_mt2_Zinv_v5_temp.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) &
done
#wait
