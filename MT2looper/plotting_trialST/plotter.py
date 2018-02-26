# Print cards for use with combine. See MT2Analysis/limits/SignalScan.
# Reimplementation of cardMaker.C.

import ROOT
import re
from math import sqrt
from sys import argv,exit

# Suppresses warnings about TH1::Sumw2
ROOT.gErrorIgnoreLevel = ROOT.kError

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(False)
canvas = ROOT.TCanvas()
canvas.SetCanvasSize(700,700)
canvas.SetTicks(2,2)
pads = [canvas]
pads[0].SetLogy()
pads[0].SetLeftMargin(0.14)
pads[0].SetTopMargin(0.12)
pads[0].SetRightMargin(0.12)
pads[0].cd()

ROOT.gStyle.SetLegendBorderSize(0)
tl = ROOT.TLegend(0.55,0.70,0.85,0.85)
tl.SetMargin(0.1)

verbose = True # Print more error messages
doScan = False

if len(argv) < 2:
    print "Usage: python plotter.py <input directory>"
    exit(1)
indir = argv[1]

# These are typically in ShortTrackLooper/output/some_directory
f_qcd = ROOT.TFile("{0}/{1}.root".format(indir,"qcd"))
f_lep = ROOT.TFile("{0}/{1}.root".format(indir,"lostlep"))
f_zinv = ROOT.TFile("{0}/{1}.root".format(indir,"zinv_ht"))

if f_qcd.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,"qcd_ht")
    exit(1)
if f_lep.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,"lostlep")
    exit(1)
if f_zinv.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,"zinv_ht")
    exit(1)

# All signal mass points share the same backgrounds. Assemble the background portion of the 
# cards once here, then fill in the mass-point-specific signal information later in makeCard.
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

    h_qcd_sig = f_qcd.Get(fullhistname)
    h_lep_sig = f_lep.Get(fullhistname)
    h_zinv_sig = f_zinv.Get(fullhistname)

    h_qcd_crsl = f_qcd.Get(fullhistnameCRSL)
    h_lep_crsl = f_lep.Get(fullhistnameCRSL)
    h_zinv_crsl = f_zinv.Get(fullhistnameCRSL)

    h_qcd_crqcd = f_qcd.Get(fullhistnameCRQCD)
    h_lep_crqcd = f_lep.Get(fullhistnameCRQCD)
    h_zinv_crqcd = f_zinv.Get(fullhistnameCRQCD)

    h_qcd_crdy = f_qcd.Get(fullhistnameCRDY)
    h_lep_crdy = f_lep.Get(fullhistnameCRDY)
    h_zinv_crdy = f_zinv.Get(fullhistnameCRDY)
    
    tl.Clear()
    if (not h_qcd_sig == None and not h_lep_sig == None and not h_zinv_sig == None):
        stack = ROOT.THStack("stack","Stacked Backgrounds")

        h_qcd_sig.SetFillColor(ROOT.kRed)
        h_qcd_sig.SetMarkerStyle(21)
        h_qcd_sig.SetMarkerColor(ROOT.kRed)
        tl.AddEntry(h_qcd_sig,"QCD")
        stack.Add(h_qcd_sig)
        h_lep_sig.SetFillColor(ROOT.kGreen)
        h_lep_sig.SetMarkerStyle(21)
        h_lep_sig.SetMarkerColor(ROOT.kGreen)
        tl.AddEntry(h_lep_sig,"Lost Lep")
        stack.Add(h_lep_sig)
        h_zinv_sig.SetFillColor(ROOT.kBlue)
        h_zinv_sig.SetMarkerStyle(21)
        h_zinv_sig.SetMarkerColor(ROOT.kBlue)
        stack.Add(h_zinv_sig)
        tl.AddEntry(h_zinv_sig,"Inv Z")

        stack.SetTitle("{0} SR;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        stack.SetMinimum(1)
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        stack.Draw("hist")
        stack.GetYaxis().SetTitleOffset(2.0)
        canvas.Modified()
        tl.Draw()
        canvas.SaveAs("pngs/{0}_sig.png".format(region))

    tl.Clear()
    stack = ROOT.THStack("stack","Stacked Backgrounds")

    if (not h_qcd_crqcd == None):
        h_qcd_crqcd.SetFillColor(ROOT.kRed)
        h_qcd_crqcd.SetMarkerStyle(21)
        h_qcd_crqcd.SetMarkerColor(ROOT.kRed)
        tl.AddEntry(h_qcd_crqcd,"QCD")
        stack.Add(h_qcd_crqcd)
    if (not h_lep_crqcd == None):
        h_lep_crqcd.SetFillColor(ROOT.kGreen)
        h_lep_crqcd.SetMarkerStyle(21)
        h_lep_crqcd.SetMarkerColor(ROOT.kGreen)
        tl.AddEntry(h_lep_crqcd,"Lost Lep")
        stack.Add(h_lep_crqcd)
    if (not h_zinv_crqcd == None):
        h_zinv_crqcd.SetFillColor(ROOT.kBlue)
        h_zinv_crqcd.SetMarkerStyle(21)
        h_zinv_crqcd.SetMarkerColor(ROOT.kBlue)
        tl.AddEntry(h_zinv_crqcd,"Inv Z")
        stack.Add(h_zinv_crqcd)

    stack.SetTitle("{0} CRQCD;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
    stack.SetMinimum(1)
    # largest is either T1qqqq inclusive, or T5qqqq inclusive
    stack.Draw("hist")
    stack.GetYaxis().SetTitleOffset(2.0)
    canvas.Modified()
    tl.Draw()
    canvas.SaveAs("pngs/{0}_crqcd.png".format(region))

    tl.Clear()
    stack = ROOT.THStack("stack","Stacked Backgrounds")
    
    if (not h_qcd_crsl == None):
        h_qcd_crsl.SetFillColor(ROOT.kRed)
        h_qcd_crsl.SetMarkerStyle(21)
        h_qcd_crsl.SetMarkerColor(ROOT.kRed)
        tl.AddEntry(h_qcd_crsl,"QCD")
        stack.Add(h_qcd_crsl)
    if (not h_lep_crsl == None):
        h_lep_crsl.SetFillColor(ROOT.kGreen)
        h_lep_crsl.SetMarkerStyle(21)
        h_lep_crsl.SetMarkerColor(ROOT.kGreen)
        tl.AddEntry(h_lep_crsl,"Lost Lep")
        stack.Add(h_lep_crsl)
    if (not h_zinv_crsl == None):
        h_zinv_crsl.SetFillColor(ROOT.kBlue)
        h_zinv_crsl.SetMarkerStyle(21)
        h_zinv_crsl.SetMarkerColor(ROOT.kBlue)
        tl.AddEntry(h_zinv_crsl,"Inv Z")
        stack.Add(h_zinv_crsl)

    stack.SetTitle("{0} CRSL;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
    stack.SetMinimum(1)
    # largest is either T1qqqq inclusive, or T5qqqq inclusive
    stack.Draw("hist")
    stack.GetYaxis().SetTitleOffset(2.0)
    canvas.Modified()
    tl.Draw()
    canvas.SaveAs("pngs/{0}_crsl.png".format(region))
    
    tl.Clear()
    stack = ROOT.THStack("stack","Stacked Backgrounds")
        
    if (not h_qcd_crdy == None):
        h_qcd_crdy.SetFillColor(ROOT.kRed)
        h_qcd_crdy.SetMarkerStyle(21)
        h_qcd_crdy.SetMarkerColor(ROOT.kRed)
        tl.AddEntry(h_qcd_crdy,"QCD")
        stack.Add(h_qcd_crdy)
    if (not h_lep_crdy == None):
        h_lep_crdy.SetFillColor(ROOT.kGreen)
        h_lep_crdy.SetMarkerStyle(21)
        h_lep_crdy.SetMarkerColor(ROOT.kGreen)
        tl.AddEntry(h_lep_crdy,"Lost Lep")
        stack.Add(h_lep_crdy)
    if (not h_zinv_crdy == None):
        h_zinv_crdy.SetFillColor(ROOT.kBlue)
        h_zinv_crdy.SetMarkerStyle(21)
        h_zinv_crdy.SetMarkerColor(ROOT.kBlue)
        tl.AddEntry(h_zinv_crdy,"Inv Z")
        stack.Add(h_zinv_crdy)
    
    stack.SetTitle("{0} CRDY;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
    stack.SetMinimum(1)
    # largest is either T1qqqq inclusive, or T5qqqq inclusive
    stack.Draw("hist")
    stack.GetYaxis().SetTitleOffset(2.0)
    canvas.Modified()
    tl.Draw()
    canvas.SaveAs("pngs/{0}_crdy.png".format(region))
    
    return True

signal_points = [] # This list of successfully processed mass points
iterator = f_lep.GetListOfKeys()
skip = "srbase"
keep = "sr"
# Loop through every directory in the standard file
for key in iterator:
    directory = key.GetTitle()
    # If the directory name contains "srbase", skip it
    if directory.find(skip) >= 0: continue
    # If the directory name contains "sr" but not "srbase", process it
    if directory.find(keep) >= 0:
        sr_number = int(re.findall("\d+",directory)[0]) # Search for the first integer of any length in the directory string
        if verbose: print directory
        # Loop over MT2 bins in this signal region
        makePlot(directory)

