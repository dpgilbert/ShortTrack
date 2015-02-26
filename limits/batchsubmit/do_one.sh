#!/bin/bash

args=("$@")

INDIR=${args[4]}

echo "Running command: combine -M Asymptotic -n "${args[0]}" "$INDIR/$i.root" > "log/limit_${args[0]}.txt" 2>&1"
combine -M Asymptotic -n "${args[0]}" "$INDIR/${args[0]}.root" > "log/limit_${args[0]}.log" 2>&1
mv "higgsCombine"${args[0]}".Asymptotic.mH120.root" "limit_"${args[0]}".root"
root -b -q "make_rValues.C(\"${args[1]}\",${args[2]},${args[3]})"
root -b -q "getGridPoints.C(\"${args[1]}\",${args[2]},${args[3]})"
