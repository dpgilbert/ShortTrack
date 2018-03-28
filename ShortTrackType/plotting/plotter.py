# Produce plots comparing only T5qqqq to itself

import ROOT
import re
from math import sqrt
from sys import argv,exit
import os

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

verbose = True # Print more error messages
#doScan = False # Not implemented

if len(argv) < 3:
    print "Usage: python plotter.py <input dir> <output dir>"
    exit(1)
indir = argv[1]
outdir = argv[2]
if (not os.path.exists(outdir)): os.mkdir(outdir)

# This is typically in ShortTrackLooper/output/some_directory
f_total = ROOT.TFile("{0}/total.root".format(indir))

if f_total.IsZombie():
    print "{0} does not have a \"total.root\" file, which is used to create the list of directories to iterate over. Please create one using hadd.".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)
if f_ttsl.IsZombie():
    print "{0} does not have a ttsl file".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)
if f_ttdl.IsZombie():
    print "{0} does not have a ttdl file".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)
if f_qcd.IsZombie():
    print "{0} does not have a qcd_ht file".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)
if f_zinv.IsZombie():
    print "{0} does not have a zinv_zpt file".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)
if f_wjets.IsZombie():
    print "{0} does not have a wjets_incl file".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)
if f_dy.IsZombie():
    print "{0} does not have a dyjetstoll_incl_ext1 file".format(indir)
    print "Usage: python plotter.py <input dir> <outpur dir>"
    exit(1)

def makePlot(directory):

    region = directory.replace("sr","region")
    topo = directory.replace("sr","")

    fullhistname = "{0}/h_mt2bins".format(directory)
    crsl_directory = directory.replace("sr","crsl")
    fullhistnameCRSL = "{0}/h_mt2bins".format(crsl_directory)
    crqcd_directory = directory.replace("sr","crqcd")
    fullhistnameCRQCD = "{0}/h_mt2bins".format(crqcd_directory)
    crdy_directory = directory.replace("sr","crdy")
    fullhistnameCRDY = "{0}/h_mt2bins".format(crdy_directory)

    fullhistnamei = "{0}/h_mt2bins".format(directory+"i")
    fullhistnameCRSLi = "{0}/h_mt2bins".format(crsl_directory+"i")
    fullhistnameCRQCDi = "{0}/h_mt2bins".format(crqcd_directory+"i")
    fullhistnameCRDYi = "{0}/h_mt2bins".format(crdy_directory+"i")

    h_nst_sig_qcd = f_qcd.Get(fullhistname)
    h_incl_sig_qcd = f_qcd.Get(fullhistnamei)
    h_nst_crsl_qcd = f_qcd.Get(fullhistnameCRSL)
    h_incl_crsl_qcd = f_qcd.Get(fullhistnameCRSLi)
    h_nst_crqcd_qcd = f_qcd.Get(fullhistnameCRQCD)
    h_incl_crqcd_qcd = f_qcd.Get(fullhistnameCRQCDi)
    h_nst_crdy_qcd = f_qcd.Get(fullhistnameCRDY)
    h_incl_crdy_qcd = f_qcd.Get(fullhistnameCRDYi)

    h_nst_sig_wjets = f_wjets.Get(fullhistname)
    h_incl_sig_wjets = f_wjets.Get(fullhistnamei)
    h_nst_crsl_wjets = f_wjets.Get(fullhistnameCRSL)
    h_incl_crsl_wjets = f_wjets.Get(fullhistnameCRSLi)
    h_nst_crqcd_wjets = f_wjets.Get(fullhistnameCRQCD)
    h_incl_crqcd_wjets = f_wjets.Get(fullhistnameCRQCDi)
    h_nst_crdy_wjets = f_wjets.Get(fullhistnameCRDY)
    h_incl_crdy_wjets = f_wjets.Get(fullhistnameCRDYi)

    h_nst_sig_ttsl = f_ttsl.Get(fullhistname)
    h_incl_sig_ttsl = f_ttsl.Get(fullhistnamei)
    h_nst_crsl_ttsl = f_ttsl.Get(fullhistnameCRSL)
    h_incl_crsl_ttsl = f_ttsl.Get(fullhistnameCRSLi)
    h_nst_crqcd_ttsl = f_ttsl.Get(fullhistnameCRQCD)
    h_incl_crqcd_ttsl = f_ttsl.Get(fullhistnameCRQCDi)
    h_nst_crdy_ttsl = f_ttsl.Get(fullhistnameCRDY)
    h_incl_crdy_ttsl = f_ttsl.Get(fullhistnameCRDYi)

    h_nst_sig_ttdl = f_ttdl.Get(fullhistname)
    h_incl_sig_ttdl = f_ttdl.Get(fullhistnamei)
    h_nst_crsl_ttdl = f_ttdl.Get(fullhistnameCRSL)
    h_incl_crsl_ttdl = f_ttdl.Get(fullhistnameCRSLi)
    h_nst_crqcd_ttdl = f_ttdl.Get(fullhistnameCRQCD)
    h_incl_crqcd_ttdl = f_ttdl.Get(fullhistnameCRQCDi)
    h_nst_crdy_ttdl = f_ttdl.Get(fullhistnameCRDY)
    h_incl_crdy_ttdl = f_ttdl.Get(fullhistnameCRDYi)

    h_nst_sig_dy = f_dy.Get(fullhistname)
    h_incl_sig_dy = f_dy.Get(fullhistnamei)
    h_nst_crsl_dy = f_dy.Get(fullhistnameCRSL)
    h_incl_crsl_dy = f_dy.Get(fullhistnameCRSLi)
    h_nst_crqcd_dy = f_dy.Get(fullhistnameCRQCD)
    h_incl_crqcd_dy = f_dy.Get(fullhistnameCRQCDi)
    h_nst_crdy_dy = f_dy.Get(fullhistnameCRDY)
    h_incl_crdy_dy = f_dy.Get(fullhistnameCRDYi)

    h_nst_sig_zinv = f_zinv.Get(fullhistname)
    h_incl_sig_zinv = f_zinv.Get(fullhistnamei)
    h_nst_crsl_zinv = f_zinv.Get(fullhistnameCRSL)
    h_incl_crsl_zinv = f_zinv.Get(fullhistnameCRSLi)
    h_nst_crqcd_zinv = f_zinv.Get(fullhistnameCRQCD)
    h_incl_crqcd_zinv = f_zinv.Get(fullhistnameCRQCDi)
    h_nst_crdy_zinv = f_zinv.Get(fullhistnameCRDY)
    h_incl_crdy_zinv = f_zinv.Get(fullhistnameCRDYi)
    
    tl.Clear()
    ths_sig = ROOT.THStack("sig_stack","{0} Stacked Backgrounds;;Count / fb^{{-1}}".format(directory))
    ths_crsl = ROOT.THStack("crsl_stack","{0} Stacked Backgrounds;;Count / fb^{{-1}}".format(crsl_directory))
    ths_crqcd = ROOT.THStack("crqcd_stack","{0} Stacked Backgrounds;;Count / fb^{{-1}}".format(crqcd_directory))
    ths_crdy = ROOT.THStack("crdy_stack","{0} Stacked Backgrounds;;Count / fb^{{-1}}".format(crdy_directory))

    # Signal Stack
    if (not h_nst_sig_qcd == None):
        h_nst_sig_qcd.SetFillColor(ROOT.kRed)
        ths_sig.Add(h_nst_sig_qcd)
        tl.AddEntry(h_nst_sig_qcd,"QCD")
    if (not h_nst_sig_dy == None):
        h_nst_sig_dy.SetFillColor(ROOT.kGreen)
        ths_sig.Add(h_nst_sig_dy)
        tl.AddEntry(h_nst_sig_dy,"DY")
    if (not h_nst_sig_zinv == None):
        h_nst_sig_zinv.SetFillColor(ROOT.kBlue)
        ths_sig.Add(h_nst_sig_zinv)
        tl.AddEntry(h_nst_sig_zinv,"Z>#nu#nu")
    if (not h_nst_sig_wjets == None):
        h_nst_sig_wjets.SetFillColor(ROOT.kYellow)
        ths_sig.Add(h_nst_sig_wjets)
        tl.AddEntry(h_nst_sig_wjets,"W+jets")
    if (not h_nst_sig_ttdl == None):
        h_nst_sig_ttdl.SetFillColor(ROOT.kMagenta)
        ths_sig.Add(h_nst_sig_ttdl)
        tl.AddEntry(h_nst_sig_ttdl,"ttdl")
    if (not h_nst_sig_ttsl == None):
        h_nst_sig_ttsl.SetFillColor(ROOT.kCyan)
        ths_sig.Add(h_nst_sig_ttsl)
        tl.AddEntry(h_nst_sig_ttsl,"ttsl")
    if (ths_sig.GetNhists() > 0):
        ths_sig.SetMinimum(0)
        ths_sig.Draw("hist")
        ths_sig.GetYaxis().SetTitleOffset(2.0)
        canvas.Modified()
        tl.Draw()
        canvas.SaveAs("{0}/{1}_sig.png".format(outdir,region))
    tl.Clear()

    # CRSL Stack
    if (not h_nst_crsl_qcd == None):
        h_nst_crsl_qcd.SetFillColor(ROOT.kRed)
        ths_crsl.Add(h_nst_crsl_qcd)
        tl.AddEntry(h_nst_crsl_qcd,"QCD")
    if (not h_nst_crsl_dy == None):
        h_nst_crsl_dy.SetFillColor(ROOT.kGreen)
        ths_crsl.Add(h_nst_crsl_dy)
        tl.AddEntry(h_nst_crsl_dy,"DY")
    if (not h_nst_crsl_zinv == None):
        h_nst_crsl_zinv.SetFillColor(ROOT.kBlue)
        ths_crsl.Add(h_nst_crsl_zinv)
        tl.AddEntry(h_nst_crsl_zinv,"Z>#nu#nu")
    if (not h_nst_crsl_wjets == None):
        h_nst_crsl_wjets.SetFillColor(ROOT.kYellow)
        ths_crsl.Add(h_nst_crsl_wjets)
        tl.AddEntry(h_nst_crsl_wjets,"W+jets")
    if (not h_nst_crsl_ttdl == None):
        h_nst_crsl_ttdl.SetFillColor(ROOT.kMagenta)
        ths_crsl.Add(h_nst_crsl_ttdl)
        tl.AddEntry(h_nst_crsl_ttdl,"ttdl")
    if (not h_nst_crsl_ttsl == None):
        h_nst_crsl_ttsl.SetFillColor(ROOT.kCyan)
        ths_crsl.Add(h_nst_crsl_ttsl)
        tl.AddEntry(h_nst_crsl_ttsl,"ttsl")
    if (ths_crsl.GetNhists() > 0):
        ths_crsl.SetMinimum(0)
        ths_crsl.Draw("hist")
        ths_crsl.GetYaxis().SetTitleOffset(2.0)
        canvas.Modified()
        tl.Draw()
        canvas.SaveAs("{0}/{1}_crsl.png".format(outdir,region))
    tl.Clear()

    # CRQCD Stack
    if (not h_nst_crqcd_qcd == None):
        h_nst_crqcd_qcd.SetFillColor(ROOT.kRed)
        ths_crqcd.Add(h_nst_crqcd_qcd)
        tl.AddEntry(h_nst_crqcd_qcd,"QCD")
    if (not h_nst_crqcd_dy == None):
        h_nst_crqcd_dy.SetFillColor(ROOT.kGreen)
        ths_crqcd.Add(h_nst_crqcd_dy)
        tl.AddEntry(h_nst_crqcd_dy,"DY")
    if (not h_nst_crqcd_zinv == None):
        h_nst_crqcd_zinv.SetFillColor(ROOT.kBlue)
        ths_crqcd.Add(h_nst_crqcd_zinv)
        tl.AddEntry(h_nst_crqcd_zinv,"Z>#nu#nu")
    if (not h_nst_crqcd_wjets == None):
        h_nst_crqcd_wjets.SetFillColor(ROOT.kYellow)
        ths_crqcd.Add(h_nst_crqcd_wjets)
        tl.AddEntry(h_nst_crqcd_wjets,"W+jets")
    if (not h_nst_crqcd_ttdl == None):
        h_nst_crqcd_ttdl.SetFillColor(ROOT.kMagenta)
        ths_crqcd.Add(h_nst_crqcd_ttdl)
        tl.AddEntry(h_nst_crqcd_ttdl,"ttdl")
    if (not h_nst_crqcd_ttsl == None):
        h_nst_crqcd_ttsl.SetFillColor(ROOT.kCyan)
        ths_crqcd.Add(h_nst_crqcd_ttsl)
        tl.AddEntry(h_nst_crqcd_ttsl,"ttsl")
    if (ths_crqcd.GetNhists() > 0):
        ths_crqcd.SetMinimum(0)
        ths_crqcd.Draw("hist")
        ths_crqcd.GetYaxis().SetTitleOffset(2.0)
        canvas.Modified()
        tl.Draw()
        canvas.SaveAs("{0}/{1}_crqcd.png".format(outdir,region))
    tl.Clear()

    # CRDY Stack
    if (not h_nst_crdy_qcd == None):
        h_nst_crdy_qcd.SetFillColor(ROOT.kRed)
        ths_crdy.Add(h_nst_crdy_qcd)
        tl.AddEntry(h_nst_crdy_qcd,"QCD")
    if (not h_nst_crdy_dy == None):
        h_nst_crdy_dy.SetFillColor(ROOT.kGreen)
        ths_crdy.Add(h_nst_crdy_dy)
        tl.AddEntry(h_nst_crdy_dy,"DY")
    if (not h_nst_crdy_zinv == None):
        h_nst_crdy_zinv.SetFillColor(ROOT.kBlue)
        ths_crdy.Add(h_nst_crdy_zinv)
        tl.AddEntry(h_nst_crdy_zinv,"Z>#nu#nu")
    if (not h_nst_crdy_wjets == None):
        h_nst_crdy_wjets.SetFillColor(ROOT.kYellow)
        ths_crdy.Add(h_nst_crdy_wjets)
        tl.AddEntry(h_nst_crdy_wjets,"W+jets")
    if (not h_nst_crdy_ttdl == None):
        h_nst_crdy_ttdl.SetFillColor(ROOT.kMagenta)
        ths_crdy.Add(h_nst_crdy_ttdl)
        tl.AddEntry(h_nst_crdy_ttdl,"ttdl")
    if (not h_nst_crdy_ttsl == None):
        h_nst_crdy_ttsl.SetFillColor(ROOT.kCyan)
        ths_crdy.Add(h_nst_crdy_ttsl)
        tl.AddEntry(h_nst_crdy_ttsl,"ttsl")
    if (ths_crdy.GetNhists() > 0):
        ths_crdy.SetMinimum(0)
        ths_crdy.Draw("hist")
        ths_crdy.GetYaxis().SetTitleOffset(2.0)
        canvas.Modified()
        tl.Draw()
        canvas.SaveAs("{0}/{1}_crdy.png".format(outdir,region))
    tl.Clear()

    return True

iterator = f_total.GetListOfKeys()
skip = "srbase"
keep = "sr"
# Loop through every directory in the standard file
for key in iterator:
    directory = key.GetTitle()
    # If the directory name contains "srbase", skip it
    if directory.find(skip) >= 0: continue
    if directory.endswith("i"): continue
    # If the directory name contains "sr" but not "srbase", process it
    if directory.find(keep) >= 0:
        sr_number = int(re.findall("\d+",directory)[0]) # Search for the first integer of any length in the directory string
        if int(sr_number) < 32: continue
        if verbose: print directory
        makePlot(directory)

