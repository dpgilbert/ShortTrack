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
pads[0].SetLeftMargin(0.14)
pads[0].SetTopMargin(0.12)
pads[0].SetRightMargin(0.12)
pads[0].cd()

ROOT.gStyle.SetLegendBorderSize(0)
tl = ROOT.TLegend(0.55,0.70,0.85,0.85)
tl.SetMargin(0.1)

verbose = True # Print more error messages
doScan = False

if len(argv) < 6:
    print "Usage: python plotter.py <input directory> <T1qqqq> <T5qqqq incl> <T5qqqq 1> <T5qqqq 2+>"
    exit(1)
indir = argv[1]
T1 = argv[2]
T5i = argv[3]
T5_1 = argv[4]
T5_2 = argv[5]

# These are typically in ShortTrackLooper/output/some_directory
f_T1 = ROOT.TFile("{0}/{1}.root".format(indir,T1))
f_T5i = ROOT.TFile("{0}/{1}.root".format(indir,T5i))
f_T5_1 = ROOT.TFile("{0}/{1}.root".format(indir,T5_1))
f_T5_2 = ROOT.TFile("{0}/{1}.root".format(indir,T5_2))

if f_T1.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,T1)
    exit(1)
if f_T5i.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,T5i)
    exit(1)
if f_T5_1.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,T5_1)
    exit(1)
if f_T5_2.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,T5_2)
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

    h_T1_sig = f_T1.Get(fullhistname)
    h_T5i_sig = f_T5i.Get(fullhistname)
    h_T5_1_sig = f_T5_1.Get(fullhistname)
    h_T5_2_sig = f_T5_2.Get(fullhistname)

    h_T1_crsl = f_T1.Get(fullhistnameCRSL)
    h_T5i_crsl = f_T5i.Get(fullhistnameCRSL)
    h_T5_1_crsl = f_T5_1.Get(fullhistnameCRSL)
    h_T5_2_crsl = f_T5_2.Get(fullhistnameCRSL)

    h_T1_crqcd = f_T1.Get(fullhistnameCRQCD)
    h_T5i_crqcd = f_T5i.Get(fullhistnameCRQCD)
    h_T5_1_crqcd = f_T5_1.Get(fullhistnameCRQCD)
    h_T5_2_crqcd = f_T5_2.Get(fullhistnameCRQCD)

    h_T1_crdy = f_T1.Get(fullhistnameCRDY)
    h_T5i_crdy = f_T5i.Get(fullhistnameCRDY)
    h_T5_1_crdy = f_T5_1.Get(fullhistnameCRDY)
    h_T5_2_crdy = f_T5_2.Get(fullhistnameCRDY)
    
    tl.Clear()
    if (not h_T1_sig == None and not h_T5i_sig == None and not h_T5_1_sig == None and not h_T5_2_sig == None):
        tl.AddEntry(h_T1_sig,"T1qqqq")
        h_T5i_sig.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_T5i_sig,"T5qqqq, incl")
        h_sig_ratio_i = h_T5i_sig.Clone("h_sig_ratio_i")
        h_sig_ratio_i.Divide(h_T1_sig)
        h_T5_1_sig.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_T5_1_sig,"T5qqqq, 1")
        h_sig_ratio_1 = h_T5_1_sig.Clone("h_sig_ratio_1")
        h_sig_ratio_1.Divide(h_T1_sig)
        h_T5_2_sig.SetLineColor(ROOT.kMagenta)
        tl.AddEntry(h_T5_2_sig,"T5qqqq, 2+")
        h_sig_ratio_2 = h_T5_2_sig.Clone("h_sig_ratio_2")
        h_sig_ratio_2.Divide(h_T1_sig)

        # ratio_i is largest by construction
        h_sig_ratio_i.SetMinimum(0)
        h_sig_ratio_i.Draw()
        h_sig_ratio_1.Draw("same")
        h_sig_ratio_2.Draw("same")
        canvas.SaveAs("pngs/{0}_sig_ratio.png".format(region))

        h_sig_ratio_1_i = h_T5_1_sig.Clone("h_sig_ratio_1_i")
        h_sig_ratio_1_i.Divide(h_T5i_sig)
        h_sig_ratio_2_i = h_T5_2_sig.Clone("h_sig_ratio_2_i")
        h_sig_ratio_2_i.Divide(h_T5i_sig)
        h_sig_ratio_1_i.SetMinimum(0)
        h_sig_ratio_1_i.Draw()
        h_sig_ratio_2_i.Draw("same")
        canvas.SaveAs("pngs/{0}_sig_ratio_i.png".format(region))

        h_T1_sig.GetYaxis().SetTitleOffset(2.0)
        h_T1_sig.SetTitle("{0} SR;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_T1_sig.SetMaximum( 1.1 * max(h_T1_sig.GetMaximum(),h_T5i_sig.GetMaximum()) ) 
        h_T1_sig.Draw()
        h_T5i_sig.Draw("same")
        h_T5_1_sig.Draw("same")
        h_T5_2_sig.Draw("same")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_sig.png".format(region))

    tl.Clear()
    if (not h_T1_crsl == None and not h_T5i_crsl == None and not h_T5_1_crsl == None and not h_T5_2_crsl == None):
        tl.AddEntry(h_T1_crsl,"T1qqqq")
        h_T5i_crsl.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_T5i_crsl,"T5qqqq, incl")
        h_crsl_ratio_i = h_T5i_crsl.Clone("h_crsl_ratio_i")
        h_crsl_ratio_i.Divide(h_T1_crsl)
        h_T5_1_crsl.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_T5_1_crsl,"T5qqqq, 1")
        h_crsl_ratio_1 = h_T5_1_crsl.Clone("h_crsl_ratio_1")
        h_crsl_ratio_1.Divide(h_T1_crsl)
        h_T5_2_crsl.SetLineColor(ROOT.kMagenta)
        tl.AddEntry(h_T5_2_crsl,"T5qqqq, 2+")
        h_crsl_ratio_2 = h_T5_2_crsl.Clone("h_crsl_ratio_2")
        h_crsl_ratio_2.Divide(h_T1_crsl)

        # inclusive ratio is largest by construction
        h_crsl_ratio_i.SetMinimum(0)
        h_crsl_ratio_i.Draw()
        h_crsl_ratio_1.Draw("same")
        h_crsl_ratio_2.Draw("same")
        canvas.SaveAs("pngs/{0}_crsl_ratio.png".format(region))

        h_crsl_ratio_1_i = h_T5_1_crsl.Clone("h_crsl_ratio_1_i")
        h_crsl_ratio_1_i.Divide(h_T5i_crsl)
        h_crsl_ratio_2_i = h_T5_2_crsl.Clone("h_crsl_ratio_2_i")
        h_crsl_ratio_2_i.Divide(h_T5i_crsl)
        h_crsl_ratio_1_i.SetMinimum(0)
        h_crsl_ratio_1_i.Draw()
        h_crsl_ratio_2_i.Draw("same")
        canvas.SaveAs("pngs/{0}_crsl_ratio_i.png".format(region))

        h_T1_crsl.GetYaxis().SetTitleOffset(2.0)
        h_T1_crsl.SetTitle("{0} CRSL;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_T1_crsl.SetMaximum( 1.1 * max(h_T1_crsl.GetMaximum(),h_T5i_crsl.GetMaximum()) ) 
        h_T1_crsl.Draw()
        h_T5i_crsl.Draw("same")
        h_T5_1_crsl.Draw("same")
        h_T5_2_crsl.Draw("same")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_crsl.png".format(region))

    tl.Clear()
    if (not h_T1_crqcd == None and not h_T5i_crqcd == None and not h_T5_1_crqcd == None and not h_T5_2_crqcd == None):
        tl.AddEntry(h_T1_crqcd,"T1qqqq")
        h_T5i_crqcd.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_T5i_crqcd,"T5qqqq, incl")
        h_crqcd_ratio_i = h_T5i_crqcd.Clone("h_crqcd_ratio_i")
        h_crqcd_ratio_i.Divide(h_T1_crqcd)
        h_T5_1_crqcd.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_T5_1_crqcd,"T5qqqq, 1")
        h_crqcd_ratio_1 = h_T5_1_crqcd.Clone("h_crqcd_ratio_1")
        h_crqcd_ratio_1.Divide(h_T1_crqcd)
        h_T5_2_crqcd.SetLineColor(ROOT.kMagenta)
        tl.AddEntry(h_T5_2_crqcd,"T5qqqq, 2+")
        h_crqcd_ratio_2 = h_T5_2_crqcd.Clone("h_crqcd_ratio_2")
        h_crqcd_ratio_2.Divide(h_T1_crqcd)

        # inclusive ratio is largest by construction
        h_crqcd_ratio_i.SetMinimum(0)
        h_crqcd_ratio_i.Draw()
        h_crqcd_ratio_1.Draw("same")
        h_crqcd_ratio_2.Draw("same")
        canvas.SaveAs("pngs/{0}_crqcd_ratio.png".format(region))

        h_crqcd_ratio_1_i = h_T5_1_crqcd.Clone("h_crqcd_ratio_1_i")
        h_crqcd_ratio_1_i.Divide(h_T5i_crqcd)
        h_crqcd_ratio_2_i = h_T5_2_crqcd.Clone("h_crqcd_ratio_2_i")
        h_crqcd_ratio_2_i.Divide(h_T5i_crqcd)
        h_crqcd_ratio_1_i.SetMinimum(0)
        h_crqcd_ratio_1_i.Draw()
        h_crqcd_ratio_2_i.Draw("same")
        canvas.SaveAs("pngs/{0}_crqcd_ratio_i.png".format(region))

        h_T1_crqcd.GetYaxis().SetTitleOffset(2.0)
        h_T1_crqcd.SetTitle("{0} CRQCD;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_T1_crqcd.SetMaximum( 1.1 * max(h_T1_crqcd.GetMaximum(),h_T5i_crqcd.GetMaximum()) ) 
        h_T1_crqcd.Draw()
        h_T5i_crqcd.Draw("same")
        h_T5_1_crqcd.Draw("same")
        h_T5_2_crqcd.Draw("same")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_crqcd.png".format(region))

    tl.Clear()
    if (not h_T1_crdy == None and not h_T5i_crdy == None and not h_T5_1_crdy == None and not h_T5_2_crdy == None):
        tl.AddEntry(h_T1_crdy,"T1qqqq")
        h_T5i_crdy.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_T5i_crdy,"T5qqqq, incl")
        h_crdy_ratio_i = h_T5i_crdy.Clone("h_crdy_ratio_i")
        h_crdy_ratio_i.Divide(h_T1_crdy)
        h_T5_1_crdy.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_T5_1_crdy,"T5qqqq, 1")
        h_crdy_ratio_1 = h_T5_1_crdy.Clone("h_crdy_ratio_1")
        h_crdy_ratio_1.Divide(h_T1_crdy)
        h_T5_2_crdy.SetLineColor(ROOT.kMagenta)
        tl.AddEntry(h_T5_2_crdy,"T5qqqq, 2+")
        h_crdy_ratio_2 = h_T5_2_crdy.Clone("h_crdy_ratio_2")
        h_crdy_ratio_2.Divide(h_T1_crdy)

        # inclusive ratio is largest by construction
        h_crdy_ratio_i.SetMinimum(0)
        h_crdy_ratio_i.Draw()
        h_crdy_ratio_1.Draw("same")
        h_crdy_ratio_2.Draw("same")
        canvas.SaveAs("pngs/{0}_crdy_ratio.png".format(region))

        h_crdy_ratio_1_i = h_T5_1_crdy.Clone("h_crdy_ratio_1_i")
        h_crdy_ratio_1_i.Divide(h_T5i_crdy)
        h_crdy_ratio_2_i = h_T5_2_crdy.Clone("h_crdy_ratio_2_i")
        h_crdy_ratio_2_i.Divide(h_T5i_crdy)
        h_crdy_ratio_1_i.SetMinimum(0)
        h_crdy_ratio_1_i.Draw()
        h_crdy_ratio_2_i.Draw("same")
        canvas.SaveAs("pngs/{0}_crdy_ratio_i.png".format(region))

        h_T1_crdy.GetYaxis().SetTitleOffset(2.0)
        h_T1_crdy.SetTitle("{0} CRDY;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_T1_crdy.SetMaximum( 1.1 * max(h_T1_crdy.GetMaximum(),h_T5i_crdy.GetMaximum()) ) 
        h_T1_crdy.Draw()
        h_T5i_crdy.Draw("same")
        h_T5_1_crdy.Draw("same")
        h_T5_2_crdy.Draw("same")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_crdy.png".format(region))

    return True

signal_points = [] # This list of successfully processed mass points
iterator = f_T1.GetListOfKeys()
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


