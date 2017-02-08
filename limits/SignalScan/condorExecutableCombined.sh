#!/bin/bash

HOME=`pwd`

SAMPLE=$1
OUTPUT_DIR=$2

echo "No.of args:  $#"
echo "args: $@"

#Tell us where we're running
echo "host: `hostname`" 

#Set environment
export SCRAM_ARCH=slc6_amd64_gcc491
source /cvmfs/cms.cern.ch/cmsset_default.sh
pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_1_5/src/
eval `scramv1 runtime -sh`
echo "should be in cvmfs: $PWD"
popd

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export PATH=$PATH:.

#untar tarball containing input files
if [ -e job_input.tar.gz ]
then
  tar -xzvf job_input.tar.gz
else
  echo "job input missing!"
  exit 1
fi

if [ -e cards_$SAMPLE.tar.gz ]
then
  tar -xzvf cards_$SAMPLE.tar.gz
else
  echo "datacards missing!"
  exit 1
fi

export PYTHONPATH="${PYTHONPATH}:${PWD}/HiggsAnalysis/CombinedLimit"
echo "PYTHONPATH is set to:"
echo $PYTHONPATH

which root

echo "ls -lrth"
ls -lrth

#echo "running combineCards.py"
#python combineCards.py -S "datacard_"*"_$SAMPLE.txt" > "card_all_$SAMPLE.txt"  
mv "datacard_${SAMPLE}_combined.txt" "card_all_$SAMPLE.txt"  #use this line instead of combineCards.py if cards are already combined
#echo "ls -lrth after combineCards.py"
#ls -lrth
echo "running text2workspace.py"
./text2workspace.py card_all_$SAMPLE.txt -b -o $SAMPLE.root
echo "ls -lrth after text2workspace.py"
ls -lrth
#echo "Running command: combine -M Asymptotic -n "$SAMPLE" "$SAMPLE.root --noFitAsimov" > "log_$SAMPLE.log" 2>&1"
#combine -M Asymptotic -n "$SAMPLE" "$SAMPLE.root" --noFitAsimov > "log_$SAMPLE.log" 2>&1
echo "Running command: combine -M Asymptotic -n "$SAMPLE" "$SAMPLE.root" > "log_$SAMPLE.log" 2>&1"
combine -M Asymptotic -n "$SAMPLE" "$SAMPLE.root" > "log_$SAMPLE.log" 2>&1
echo "ls -lrth after calculating the limit"
ls -lrth
mv "higgsCombine"$SAMPLE".Asymptotic.mH120.root" "limit_"$SAMPLE".root"

echo "ls -lrth"
ls -lrth

if [ ! -d "${OUTPUT_DIR}" ]; then
    echo "creating output directory " ${OUTPUT_DIR}
    mkdir ${OUTPUT_DIR}
fi

#Copy the output
gfal-copy -p -f -t 4200 --verbose file://`pwd`/limit_$SAMPLE.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${OUTPUT_DIR}/limit_$SAMPLE.root
gfal-copy -p -f -t 4200 --verbose file://`pwd`/log_$SAMPLE.log srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${OUTPUT_DIR}/logs/log_$SAMPLE.log
#lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/limit_$SAMPLE.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${OUTPUT_DIR}/limit_$SAMPLE.root
stageout_error=$?

if [ $stageout_error != 0 ]
then
  echo "Stageout with lcg-cp failed."
fi

#cleanup
echo "cleaning up"
for FILE in `find . -not -name "*stderr" -not -name "*stdout"`; do rm -rf $FILE; done
echo "cleaned up"
ls
