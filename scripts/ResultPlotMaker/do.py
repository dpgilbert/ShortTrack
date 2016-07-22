#! /usr/bin/env python
import ROOT
from ResultPlotMaker import MakePlot

datacard_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/scripts/cards_test"

# {0} is the HT region, {1} is the jet/bjet region, {2} is the mt2 bin
datacard_name = "datacard_{0}_{1}_{2}_T1bbbb_1500_100.txt"

outdir = "/home/users/bemarsh/public_html/mt2/result_plots"

MakePlot("HT200to450",datacard_dir, datacard_name, outdir, userMax=5e6)
MakePlot("HT450to575",datacard_dir, datacard_name, outdir, userMax=7e5)
MakePlot("HT575to1000",datacard_dir, datacard_name, outdir, userMax=7e5)
MakePlot("HT1000to1500",datacard_dir, datacard_name, outdir, userMax=7e5)
MakePlot("HT1500toInf",datacard_dir, datacard_name, outdir, userMax=7e5)
# monojet is handled specially, just use "monojet" as the ht region name
MakePlot("monojet",datacard_dir, datacard_name, outdir, userMax=7e7)
