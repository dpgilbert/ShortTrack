#!/bin/bash

#MODEL=T1tttt
#MODEL=T1bbbb
#MODEL=T1qqqq
#MODEL=T2tt
MODEL=T2bb

cp "r-values_$MODEL.root" PlotsSMS/config/mt2/${MODEL}_results.root
cd PlotsSMS
python python/makeSMSplots.py config/mt2/${MODEL}_mt2.cfg $MODEL
