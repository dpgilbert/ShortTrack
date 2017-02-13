#!/bin/bash

MODEL=T1tttt
#MODEL=T1bbbb
#MODEL=T1qqqq
#MODEL=T2tt
#MODEL=T2bb
#MODEL=T2-4bd

DIR=/home/users/mderdzinski/winter2017/clean_master/limits/SignalScan

cp "${DIR}/r-values_${MODEL}.root" PlotsSMS/config/mt2/${MODEL}_results.root
#cp "${DIR}/r-values_${MODEL}_modified.root" PlotsSMS/config/mt2/${MODEL}_results.root
#cp "${DIR}/r-values_${MODEL}_scan.root" PlotsSMS/config/mt2/${MODEL}_results.root
pushd PlotsSMS
python python/makeSMSplots.py config/mt2/${MODEL}_mt2.cfg $MODEL
popd
