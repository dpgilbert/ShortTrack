#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-00-02"

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
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_DYJetsToLL_M-50_HT-400to600

#
# Z INVISIBLE
#
./writeConfig.sh /hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-03/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/merged/ ${TAG}_ZInv_HT200to400

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

#
# SIGNAL
#

# --- write submit script ---
mkdir configs_${TAG}
mv condor_${TAG}*.cmd configs_${TAG}
echo "#!/bin/bash" > submitAll.sh
echo "voms-proxy-init -voms cms -valid 240:00" >> submitAll.sh
for file in configs_${TAG}/*.cmd
do 
    echo "condor_submit ${file}" >> submitAll.sh
done
chmod +x submitAll.sh
echo "[writeAllConfig] wrote submit script submitAll.sh"
