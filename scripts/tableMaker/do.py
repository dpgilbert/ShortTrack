#! /usr/bin/env python
import ROOT
import os
from MT2TableMaker import *

indir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-07_nojson_skim_base_mt2gt200_ZinvV6_12p9fb"

# # single lep monojet table
# samples = ["wjets_ht","top","qcd_ht"]
# dirname = ["crslbaseJ0B", "crslbaseJ1B"]
# hname = "h_mt2bins"
# bins = [[1,2,3,4,5,6,7,-1],
#         [1,2,3,4,5,-1]]
# maxCols = 4
# print makeTableFromHistBins(indir, samples, dirname, hname, bins=bins, maxCols=maxCols)

# # single lep multijet table, ht<1000
# samples = ["wjets_ht","top","qcd_ht"]
# ht_regs = ["VL","L","M"]
# dirnames = ["crsl1","crsl2","crsl3","crsl4","crsl5","crsl6","crsl7","crsl8","crsl10"]
# maxCols = 5
# print makeTableFromHTregs(indir, samples, ht_regs, dirnames, maxCols=maxCols)

# single lep multijet table, ht>1000
samples = ["wjets_ht","top","qcd_ht"]
ht_regs = ["H","UH"]
dirnames = ["crsl1","crsl2","crsl3","crsl4","crsl5","crsl6","crsl7","crsl8","crsl10"]
maxCols = 5
print makeTableFromHTregs(indir, samples, ht_regs, dirnames, maxCols=maxCols)
