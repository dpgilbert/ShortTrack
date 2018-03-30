# Produce plots comparing only T5qqqq to itself

import ROOT
import re
from math import sqrt
from sys import argv,exit
import os
import time

# Suppresses warnings about TH1::Sumw2
ROOT.gErrorIgnoreLevel = ROOT.kError

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(False)
canvas = ROOT.TCanvas()
canvas.SetCanvasSize(700,700)
canvas.SetTicks(2,2)
pads = [canvas]
pads[0].SetLeftMargin(0.14)
pads[0].SetTopMargin(0.12)
pads[0].SetRightMargin(0.12)
pads[0].cd()

ROOT.gStyle.SetLegendBorderSize(0)
tl = ROOT.TLegend(0.55,0.70,0.85,0.85)
tl.SetMargin(0.1)

verbose = False # Print more status messages
#doScan = False # Not implemented

if len(argv) < 3:
    print "Usage: python plotter.py <indir_suffix (region)> <outdir>"
    exit(1)
region = argv[1]
indir = "../output_{0}".format(region)
outdir = argv[2]
if (not os.path.exists(outdir)): os.mkdir(outdir)

selection_suffixes = ["incl","HT","HTMET","HTMETDphiDiff","HTMETDphiDiffMT2"]
histnames = ["h_mu_pix","h_mu_ext","h_mu_tot","h_fake_pix","h_fake_ext","h_fake_tot","h_mu_etaphi","h_fake_etaphi","h_mu_ratio"]
samples = ["ttsl","ttdl","DY","Wjets","qcd300to500","qcd500to700","qcd700to1000","qcd1000to1500","qcd1500to2000","qcd2000toInf","zinv100to200","zinv200toInf"]
colors = [ROOT.kBlack,ROOT.kRed,ROOT.kGreen,ROOT.kCyan,ROOT.kMagenta]
selection_colors = dict(zip(selection_suffixes,colors))

filedict = {}

print "Collecting files"

for selection in selection_suffixes:
    fullindir = "{0}/{1}".format(indir,selection)
    f_ttsl = ROOT.TFile("{0}/ttsl.root".format(fullindir))
    f_ttdl = ROOT.TFile("{0}/ttdl.root".format(fullindir))
    f_dy = ROOT.TFile("{0}/dyjetstoll_incl_ext1.root".format(fullindir))
    f_wjets = ROOT.TFile("{0}/wjets_incl.root".format(fullindir))
    f_qcd300to500 = ROOT.TFile("{0}/qcd_ht300to500.root".format(fullindir))
    f_qcd500to700 = ROOT.TFile("{0}/qcd_ht500to700.root".format(fullindir))
    f_qcd700to1000 = ROOT.TFile("{0}/qcd_ht700to1000.root".format(fullindir))
    f_qcd1000to1500 = ROOT.TFile("{0}/qcd_ht1000to1500.root".format(fullindir))
    f_qcd1500to2000 = ROOT.TFile("{0}/qcd_ht1500to2000_ext1.root".format(fullindir))
    f_qcd2000toInf = ROOT.TFile("{0}/qcd_ht2000toInf.root".format(fullindir))
    f_zpt100to200 = ROOT.TFile("{0}/zinv_zpt100to200.root".format(fullindir))
    f_zpt200toInf = ROOT.TFile("{0}/zinv_zpt200toInf.root".format(fullindir))
    
    if f_ttsl.IsZombie():
        print "{0} does not have a ttsl file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_ttdl.IsZombie():
        print "{0} does not have a ttdl file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_qcd300to500.IsZombie():
        print "{0} does not have a qcd_ht300to500 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_qcd500to700.IsZombie():
        print "{0} does not have a qcd_ht500to700 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_qcd700to1000.IsZombie():
        print "{0} does not have a qcd_ht700to1000 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_qcd1000to1500.IsZombie():
        print "{0} does not have a qcd_ht1000to1500 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_qcd1500to2000.IsZombie():
        print "{0} does not have a qcd_ht1500to2000_ext1 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_qcd2000toInf.IsZombie():
        print "{0} does not have a qcd_ht2000toInf file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_zpt100to200.IsZombie():
        print "{0} does not have a zinv_zpt100to200 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_zpt200toInf.IsZombie():
        print "{0} does not have a zinv_zpt200toInf file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_wjets.IsZombie():
        print "{0} does not have a wjets_incl file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)
    if f_dy.IsZombie():
        print "{0} does not have a dyjetstoll_incl_ext1 file".format(fullindir)
        print "Usage: python plotter.py <input dir> <outpur dir>"
        exit(1)

    files = [f_ttsl,f_ttdl,f_dy,f_wjets,f_qcd300to500,f_qcd500to700,f_qcd700to1000,f_qcd1000to1500,f_qcd1500to2000,f_qcd2000toInf,f_zpt100to200,f_zpt200toInf]
    print "Adding files for selection {0}".format(selection)
    filedict[selection] = dict(zip(samples,files))

print "Looping over histograms"
for histname in histnames:
    print histname
    for sample in samples:
        print "> " + sample
        tl.Clear()
        if (histname.find("etaphi") < 0): 
            canvas.SetLogy(True)
            for selection in selection_suffixes:
                print "> > " + selection
                current_hist = filedict[selection][sample].Get(histname)
                if (verbose): print "> > " + str(current_hist.Integral())
                current_hist.SetLineColor(selection_colors[selection])
                if (selection.find("incl") >= 0):
                    current_hist.SetTitle(sample+" "+histname)
                    current_hist.SetMinimum(0.1)
                    current_hist.Draw()
                else:
                    current_hist.Draw("same")
                tl.AddEntry(current_hist,selection)
            tl.Draw()
        else:
            current_hist = filedict["incl"][sample].Get(histname)
            current_hist.SetTitle(sample+" "+histname)
            current_hist.Draw("colz")
        canvas.SaveAs("{0}/{1}_{2}.png".format(outdir,sample,histname))
        canvas.SetLogy(False)

            
