#! /usr/bin/env python
import ROOT
from ResultPlotMaker import MakePlot, MakeComparison, MakeMacroRegionPlot
import ResultPlotUtils as utils

datacard_dir = "/home/users/mderdzinski/winter2017/clean_master/scripts/cards_T2bb_Feb1"
#datacard_dir = "../cards_test"

# {0} is the HT region, {1} is the jet/bjet region, {2} is the mt2 bin
datacard_name = "datacard_{0}_{1}_{2}_T2bb_375_425.txt"
#datacard_name = "datacard_{0}_{1}_{2}_ttw_mg_lo.txt"

outdir = "plots"

utils.lumi = 36.8

MakePlot("HT250to450",datacard_dir, datacard_name, outdir, userMax=5e6)
MakePlot("HT450to575",datacard_dir, datacard_name, outdir, userMax=7e5)
MakePlot("HT575to1000",datacard_dir, datacard_name, outdir, userMax=7e5)
MakePlot("HT1000to1500",datacard_dir, datacard_name, outdir, userMax=7e5)
MakePlot("HT1500toInf",datacard_dir, datacard_name, outdir, userMax=7e5)
# monojet is handled specially, just use "monojet" as the ht region name
MakePlot("monojet",datacard_dir, datacard_name, outdir, userMax=7e7)

# datacard_dir_eth = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/scripts/cards_test_eth"
# datacard_name_eth = "datacard_{0}_{1}_{2}.txt"
# MakeComparison("HT200to450", datacard_dir, datacard_dir_eth, datacard_name, datacard_name_eth, outdir, iBkgd=1, userMax=7e5)
# MakeComparison("HT200to450", datacard_dir, datacard_dir_eth, datacard_name, datacard_name_eth, outdir, iBkgd=2, userMax=7e5)
# MakeComparison("HT575to1000", datacard_dir, datacard_dir_eth, datacard_name, datacard_name_eth, outdir, iBkgd=1, userMax=7e5)
# MakeComparison("HT575to1000", datacard_dir, datacard_dir_eth, datacard_name, datacard_name_eth, outdir, iBkgd=2, userMax=7e5)

#datacard_name_topo = "datacard_{0}_{1}_ttw_mg_lo.txt"
#MakeMacroRegionPlot(1,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(2,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(3,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(4,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(5,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(6,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(7,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
#MakeMacroRegionPlot(8,datacard_dir, datacard_name_topo, outdir, userMax=5e6)
