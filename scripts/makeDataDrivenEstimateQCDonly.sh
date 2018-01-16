#!/bin/bash

INDIR=/home/users/dpgilber/MT2AnalysisFinal16/MT2looper/output/full2016
QCDFILE=data_Run2016
QCDESTIMATE=./../../MT2AnalysisNebraskaCMS3/scripts/qcdEstimate/output/full2016/qcdEstimate
QCDMONOJET=./inputs/qcdEstimateMonojet

echo "root -b -q qcdRphiMaker.C+(${INDIR},${QCDFILE},${QCDESTIMATE},${QCDMONOJET})"
root -b -q "qcdRphiMaker.C+(\"${INDIR}\",\"${QCDFILE}\",\"${QCDESTIMATE}\",\"${QCDMONOJET}\")" >> dataDrivenEstimates.log
echo "done"
