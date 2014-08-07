#!/bin/bash

#
# All MT2 related datasets available on hadoop
#

TAG="V00-00-01"

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


#
# SINGLE TOP
#

#
# DY+JETS
#

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
