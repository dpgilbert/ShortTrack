#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-00-04"

#
# DATA
#

#
# TTBAR
#

./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_TTJetsMG

#
# ALTERNATIVE TTBAR
#

#
# W+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_WJets_HT100to200
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_WJets_HT200to400
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_WJets_HT400to600
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_WJets_HT600toInf


#
# SINGLE TOP
#

#
# DY+JETS
#

./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_DYJetsToLL_M-50_HT-100to200
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_DYJetsToLL_M-50_HT-200to400
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_DYJetsToLL_M-50_HT-400to600
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_DYJetsToLL_M-50_HT-600toInf


#
# GAMMA + JETS
#
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/GJet_Pt-15to3000_Tune4C_13TeV_pythia8/merged/ ${TAG}_GJet_Pt15to3000
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_GJet_HT100to200
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/GJets_HT-200to400_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_GJet_HT200to400
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_GJet_HT400to600
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/GJets_HT-600toInf_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_GJet_HT600toInf

#
# Z INVISIBLE
#

./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_ZInv_HT100to200
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_ZInv_HT200to400
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_ZInv_HT400to600
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_ZInv_HT600toInf

#
# DIBOSON
#

#
# TRIBOSON
#

#
# TTV
#

#
# QCD
#

./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-30to50_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT30to50
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-50to80_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT50to80
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-80to120_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT80to120
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-120to170_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT120to170
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-170to300_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT170to300
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-300to470_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT300to470
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-470to600_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT470to600
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-600to800_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT600to800
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-800to1000_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT800to1000
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT1000to1400
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT1400to1800
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/QCD_Pt-1800_Tune4C_13TeV_pythia8/merged/ ${TAG}_QCD_PT1800

#
# SIGNAL
#

./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_T1tttt_1500_100
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_T1tttt_1200_800
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_T1qqqq_1400_100
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_T1bbbb_1000_900
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_T1bbbb_1500_100

# --- write submit script ---
mkdir -p configs_${TAG}

mv condor_${TAG}*.cmd configs_${TAG}
echo "#!/bin/bash" > submitAll.sh
echo "voms-proxy-init -voms cms -valid 240:00" >> submitAll.sh
for file in configs_${TAG}/*.cmd
do 
    echo "condor_submit ${file}" >> submitAll.sh
done
chmod +x submitAll.sh
echo "[writeAllConfig] wrote submit script submitAll.sh"
