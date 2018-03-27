# Produce plots comparing only T5qqqq to itself

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
#doScan = False # Not implemented

if len(argv) < 3:
    print "Usage: python plotter.py <input file> <output dir>"
    exit(1)
infile_name = argv[1]
outdir = argv[2]

# This is typically in ShortTrackLooper/output/some_directory
f_in = ROOT.TFile("{0}.root".format(infile_name))

if f_in.IsZombie():
    print "{0}.root does not exist\n".format(infile_name)
    print "Usage: python plotter.py <input file> <outpur dir>"
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

    h_nst_sig = f_in.Get(fullhistname)
    h_incl_sig = f_in.Get(fullhistnamei)

    h_nst_crsl = f_in.Get(fullhistnameCRSL)
    h_incl_crsl = f_in.Get(fullhistnameCRSLi)

    h_nst_crqcd = f_in.Get(fullhistnameCRQCD)
    h_incl_crqcd = f_in.Get(fullhistnameCRQCDi)

    h_nst_crdy = f_in.Get(fullhistnameCRDY)
    h_incl_crdy = f_in.Get(fullhistnameCRDYi)
    
    tl.Clear()
    if (not h_nst_sig == None and not h_incl_sig == None):
        h_incl_sig.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_incl_sig,"Inclusive")
        h_nst_sig.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_nst_sig,"N_{st} > 0")

        h_sig_ratio = h_incl_sig.Clone("h_sig_ratio")
        h_sig_ratio.Divide(h_nst_sig)
        h_sig_ratio.SetLineColor(ROOT.kBlue)
        h_sig_ratio.SetMinimum(0)
        h_sig_ratio.Draw()
        canvas.SaveAs("{0}/{1}_sig_ratio.png".format(outdir,region))

        h_incl_sig.GetYaxis().SetTitleOffset(2.0)
        h_incl_sig.SetTitle("{0} SR;M_{{T2}} [GeV];Count / fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_incl_sig.SetMaximum( 10 * h_incl_sig.GetMaximum() )
        h_incl_sig.SetMinimum( max(0.1 * h_nst_sig.GetMinimum(), 1e-9) )
        h_incl_sig.Draw()
        h_nst_sig.Draw("same")
        tl.Draw()
        canvas.SetLogy(True)
        canvas.SaveAs("{0}/{1}_sig.png".format(outdir,region))
        canvas.SetLogy(False)

    tl.Clear()
    if (not h_incl_crsl == None and not h_nst_crsl == None):
        h_incl_crsl.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_incl_crsl,"Inclusive")
        h_nst_crsl.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_nst_crsl,"N_{st} > 0")

        h_crsl_ratio = h_incl_crsl.Clone("h_crsl_ratio")
        h_crsl_ratio.Divide(h_nst_crsl)
        h_crsl_ratio.SetLineColor(ROOT.kBlue)
        h_crsl_ratio.SetMinimum(0)
        h_crsl_ratio.Draw()
        canvas.SaveAs("{0}/{1}_crsl_ratio.png".format(outdir,region))

        h_incl_crsl.GetYaxis().SetTitleOffset(2.0)
        h_incl_crsl.SetTitle("{0} CRSL;M_{{T2}} [GeV];Count / fb^{{-1}}".format(topo))
        h_incl_crsl.SetMaximum( 10 * h_incl_crsl.GetMaximum() )
        h_incl_crsl.SetMinimum( max(0.1 * h_nst_crsl.GetMinimum(), 1e-9) )
        h_incl_crsl.Draw()
        h_nst_crsl.Draw("same")
        tl.Draw()
        canvas.SetLogy(True)
        canvas.SaveAs("{0}/{1}_crsl.png".format(outdir,region))
        canvas.SetLogy(False)

    tl.Clear()
    if (not h_incl_crqcd == None and not h_nst_crqcd == None):
        h_incl_crqcd.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_incl_crqcd,"Inclusive")
        h_nst_crqcd.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_nst_crqcd,"N_{st}")

        h_crqcd_ratio = h_incl_crqcd.Clone("h_crqcd_ratio")
        h_crqcd_ratio.Divide(h_nst_crqcd)
        h_crqcd_ratio.SetLineColor(ROOT.kBlue)
        h_crqcd_ratio.SetMinimum(0)
        h_crqcd_ratio.Draw()
        canvas.SaveAs("{0}/{1}_crqcd_ratio.png".format(outdir,region))

        h_incl_crqcd.GetYaxis().SetTitleOffset(2.0)
        h_incl_crqcd.SetTitle("{0} CRQCD;M_{{T2}} [GeV];Count / fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_incl_crqcd.SetMaximum( 10 * h_incl_crqcd.GetMaximum() )
        h_incl_crqcd.SetMinimum( max(0.1 * h_nst_crqcd.GetMinimum(), 1e-9) )
        h_incl_crqcd.Draw()
        h_nst_crqcd.Draw("same")
        tl.Draw()
        canvas.SetLogy(True)
        canvas.SaveAs("{0}/{1}_crqcd.png".format(outdir,region))
        canvas.SetLogy(False)

    tl.Clear()
    if (not h_incl_crdy == None and not h_nst_crdy == None):
        h_incl_crdy.SetLineColor(ROOT.kRed)
        tl.AddEntry(h_incl_crdy,"Inclusive")
        h_nst_crdy.SetLineColor(ROOT.kGreen)
        tl.AddEntry(h_nst_crdy,"N_{st}")

        h_crdy_ratio = h_incl_crdy.Clone("h_crdy_ratio")
        h_crdy_ratio.Divide(h_nst_crdy)
        h_crdy_ratio.SetLineColor(ROOT.kBlue)
        h_crdy_ratio.SetMinimum(0)
        h_crdy_ratio.Draw()
        canvas.SaveAs("{0}/{1}_crdy_ratio.png".format(outdir,region))

        h_incl_crdy.GetYaxis().SetTitleOffset(2.0)
        h_incl_crdy.SetTitle("{0} CRDY;M_{{T2}} [GeV];Count / fb^{{-1}}".format(topo))
        # largest is either T1qqqq inclusive, or T5qqqq inclusive
        h_incl_crdy.SetMaximum( 10 * h_incl_crdy.GetMaximum() )
        h_incl_crdy.SetMinimum( max(0.1 * h_nst_crdy.GetMinimum(), 1e-9) )
        h_incl_crdy.Draw()
        h_nst_crdy.Draw("same")
        tl.Draw()
        canvas.SetLogy(True)
        canvas.SaveAs("{0}/{1}_crdy.png".format(outdir,region))
        canvas.SetLogy(False)

    return True

iterator = f_in.GetListOfKeys()
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

