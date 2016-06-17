#! /usr/bin/env python
# run this over the output of QCDLooper

import glob
import os
import ROOT
import rphiPlotMaker as rpm

ROOT.gROOT.SetBatch(1)

indir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/QCDLooper/output/test"
outdir = "rphi_plots/test"

try:
    os.makedirs(outdir)
except:
    pass

qcd_sample = "qcd_ht"
nqcd_samples = ["wjets_ht","ttsl", "dyjetsll_incl", "singletop","ttdl", "zinv_ht"]

qcd_file = os.path.join(indir,qcd_sample+".root")
nqcd_files = [os.path.join(indir,s+".root") for s in nqcd_samples]

for reg in ["VL","L","M","H","UH"]:
    rpm.makeMCPlot(qcd_file, nqcd_files, outdir, reg)



