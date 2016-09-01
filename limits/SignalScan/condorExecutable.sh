#!/bin/bash

HOME=`pwd`

SAMPLE=$1
OUTPUT_DIR=$2

echo "No.of args:  $#"
echo "args: $@"

#Tell us where we're running
echo "host: `hostname`" 

#untar tarball containing input files
if [ -e job_input.tar.gz ]
then
  tar -xzvf job_input.tar.gz
else
  echo "job input missing!"
  exit 1
fi

mv cards*.tar.gz CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/scripts

#Set environment
export SCRAM_ARCH=slc6_amd64_gcc491
source /cvmfs/cms.cern.ch/cmsset_default.sh
# jumping through hoops to try to get this to work.....
cd CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit
eval `scramv1 runtime -sh`
scramv1 b ProjectRename
scramv1 b clean; scramv1 b
eval `scramv1 runtime -sh`
cd scripts

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export PATH=$PATH:.

if [ -e cards_$SAMPLE.tar.gz ]
then
  tar -xzvf cards_$SAMPLE.tar.gz
else
  echo "datacards missing!"
  exit 1
fi

# export PYTHONPATH="${PYTHONPATH}:${PWD}/HiggsAnalysis/CombinedLimit"
# echo "PYTHONPATH is set to:"
# echo $PYTHONPATH

which root

echo "ls -lrth"
ls -lrth

echo "running combineCards.py"
python combineCards.py -S "datacard_"*"_$SAMPLE.txt" > "card_all_$SAMPLE.txt"  
echo "ls -lrth after combineCards.py"
ls -lrth
# echo "running text2workspace.py"
# ./text2workspace.py card_all_$SAMPLE.txt -b -o $SAMPLE.root
# echo "ls -lrth after text2workspace.py"
# ls -lrth
#echo "Running command: combine -M Asymptotic -n "$SAMPLE" "$SAMPLE.root --noFitAsimov" > "log_$SAMPLE.log" 2>&1"
#combine -M Asymptotic -n "$SAMPLE" "$SAMPLE.root" --noFitAsimov > "log_$SAMPLE.log" 2>&1
echo "Running command: combine -M Asymptotic -n "$SAMPLE" "$card_all_SAMPLE.txt" > "log_$SAMPLE.log" 2>&1"
combine -M Asymptotic -n "$SAMPLE" "card_all_$SAMPLE.txt" 2>&1
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
gfal-copy -p -f -t 4200 --verbose file:`pwd`/limit_$SAMPLE.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${OUTPUT_DIR}/limit_$SAMPLE.root
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
