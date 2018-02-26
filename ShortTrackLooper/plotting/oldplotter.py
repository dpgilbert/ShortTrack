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
pads[0].SetLeftMargin(0.14)
pads[0].SetTopMargin(0.12)
pads[0].SetRightMargin(0.12)
pads[0].cd()

ROOT.gStyle.SetLegendBorderSize(0)
tl = ROOT.TLegend(0.55,0.70,0.85,0.85)
tl.SetMargin(0.1)

verbose = True # Print more error messages
doScan = False

if len(argv) < 7:
    print "Usage: python ShortTrackContam.py <input directory> <T1qqqq> <T5qqqq incl> <T5qqqq 1+> <T5qqqq 2+> <output name>"
    exit(1)
indir = argv[1]
T1 = argv[2]
T5i = argv[3]
T5_1 = argv[4]
T5_2 = argv[5]
outname = argv[6]

t1_compare = (second.find("T1") >= 0)

print t1_compare

# These are typically in MT2looper/output/some_directory
f_first = ROOT.TFile("{0}/{1}.root".format(indir,first))
f_second = ROOT.TFile("{0}/{1}.root".format(indir,second))

if f_first.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,first)
    exit(1)
if f_second.IsZombie():
    print "{0}/{1}.root does not exist\n".format(indir,second)
    exit(1)

f_out = ROOT.TFile("{0}.root".format(outname),"RECREATE")

# All signal mass points share the same backgrounds. Assemble the background portion of the 
# cards once here, then fill in the mass-point-specific signal information later in makeCard.
def makePlot(directory):

    region = directory.replace("sr","region")
    topo = directory.replace("sr","")

    this_dir = f_out.Get(region)
    if (this_dir == None):
        this_dir = f_out.mkdir(region)

    fullhistname = "{0}/h_mt2bins".format(directory)
    fullhistnameGenMet = "{0}/h_mt2bins_genmet".format(directory)
    fullhistnameScan = "{0}_sigscan".format(fullhistname)
    fullhistnameScanGenMet = "{0}_sigscan_genmet".format(fullhistname)
    crsl_directory = directory.replace("sr","crsl")
    fullhistnameCRSL = "{0}/h_mt2bins".format(crsl_directory)
    fullhistnameCRSLGenMet = "{0}/h_mt2bins_genmet".format(crsl_directory)
    fullhistnameCRSLScan = "{0}_sigscan".format(fullhistnameCRSL)
    fullhistnameCRSLScanGenMet = "{0}_sigscan_genmet".format(fullhistnameCRSL)
    crqcd_directory = directory.replace("sr","crqcd")
    fullhistnameCRQCD = "{0}/h_mt2bins".format(crqcd_directory)
    fullhistnameCRQCDGenMet = "{0}/h_mt2bins_genmet".format(crqcd_directory)
    fullhistnameCRQCDScan = "{0}_sigscan".format(fullhistnameCRQCD)
    fullhistnameCRQCDScanGenMet = "{0}_sigscan_genmet".format(fullhistnameCRQCD)
    crdy_directory = directory.replace("sr","crdy")
    fullhistnameCRDY = "{0}/h_mt2bins".format(crdy_directory)
    fullhistnameCRDYGenMet = "{0}/h_mt2bins_genmet".format(crdy_directory)
    fullhistnameCRDYScan = "{0}_sigscan".format(fullhistnameCRDY)
    fullhistnameCRDYScanGenMet = "{0}_sigscan_genmet".format(fullhistnameCRDY)

    h_1_sig = f_first.Get(fullhistname)
    h_2_sig = f_second.Get(fullhistname)

    h_1_crsl = f_first.Get(fullhistnameCRSL)
    h_2_crsl = f_second.Get(fullhistnameCRSL)
#    if not t1_compare:
#        h_2_crsl = f_second.Get(fullhistnameCRSL)
#    else:
#        h_2_crsl_scan = f_second.Get(fullhistnameCRSLScan)
#        if not h_2_crsl_scan == None:
#            bin1 = h_2_crsl_scan.GetYaxis().FindBin(1800)
#            bin2 = h_2_crsl_scan.GetZaxis().FindBin(1400)
#            h_2_crsl = h_2_crsl_scan.ProjectionX("h_mt2bins_1800_1400",bin1,bin1,bin2,bin2)
#        else:
#            h_2_crsl = None
        
    h_1_crsl_genmet = f_first.Get(fullhistnameCRSLGenMet)
    h_2_crsl_genmet = f_second.Get(fullhistnameCRSLGenMet)
#    if not t1_compare:
#        h_2_crsl_genmet = f_second.Get(fullhistnameCRSLGenMet)
#    else:
#        h_2_crsl_genmet_scan = f_second.Get(fullhistnameCRSLScanGenMet)
#        if not h_2_crsl_genmet_scan == None:
#            bin1 = h_2_crsl_genmet_scan.GetYaxis().FindBin(1800)
#            bin2 = h_2_crsl_genmet_scan.GetZaxis().FindBin(1400)
#            h_2_crsl_genmet = h_2_crsl_genmet_scan.ProjectionX("h_mt2bins_1800_1400",bin1,bin1,bin2,bin2)
#        else:
#            h_2_crsl_genmet = None

    h_1_crqcd = f_first.Get(fullhistnameCRQCD)
    h_2_crqcd = f_second.Get(fullhistnameCRQCD)
#    if not t1_compare:
#        h_1_crqcd = f_second.Get(fullhistnameCRQCD)
#    else:
#        h_2_crqcd_scan = f_second.Get(fullhistnameCRQCDScan)
#        if not h_2_crqcd_scan == None:
#            bin1 = h_2_crqcd_scan.GetYaxis().FindBin(1800)
#            bin2 = h_2_crqcd_scan.GetZaxis().FindBin(0)
#            h_2_crqcd = h_2_crqcd_scan.ProjectionX("h_mt2bins_1800_1400",bin1,bin1,bin2,bin2)
#        else:
#            h_2_crqcd = None
        
    h_1_crqcd_genmet = f_first.Get(fullhistnameCRQCDGenMet)
    h_2_crqcd_genmet = f_second.Get(fullhistnameCRQCDGenMet)
#    if not t1_compare:
#        h_2_crqcd_genmet = f_second.Get(fullhistnameCRQCDGenMet)
#    else:
#        h_2_crqcd_genmet_scan = f_second.Get(fullhistnameCRQCDScanGenMet)
#        if not h_2_crqcd_genmet_scan == None:
#            bin1 = h_2_crqcd_genmet_scan.GetYaxis().FindBin(1800)
#            bin2 = h_2_crqcd_genmet_scan.GetZaxis().FindBin(1400)
#            h_2_crqcd_genmet = h_2_crqcd_genmet_scan.ProjectionX("h_mt2bins_1800_1400",bin1,bin1,bin2,bin2)
#        else:
#            h_2_crqcd_genmet = None

    h_1_crdy = f_first.Get(fullhistnameCRDY)
    h_2_crdy = f_second.Get(fullhistnameCRDY)
#    if not t1_compare:
#        h_2_crdy = f_second.Get(fullhistnameCRDY)
#    else:
#        h_2_crdy_scan = f_second.Get(fullhistnameCRDYScan)
#        if not h_2_crdy_scan == None:
#            bin1 = h_2_crdy_scan.GetYaxis().FindBin(1800)
#            bin2 = h_2_crdy_scan.GetZaxis().FindBin(0)
#            h_2_crdy = h_2_crdy_scan.ProjectionX("h_mt2bins_1800_1400",bin1,bin1,bin2,bin2)
#        else:
#            h_2_crdy = None
        
    h_1_crdy_genmet = f_first.Get(fullhistnameCRDYGenMet)
    h_2_crdy_genmet = f_second.Get(fullhistnameCRDYGenMet)
#    if not t1_compare:
#        h_2_crdy_genmet = f_second.Get(fullhistnameCRDYGenMet)
#    else:
#        h_2_crdy_genmet_scan = f_second.Get(fullhistnameCRDYScanGenMet)
#        if not h_2_crdy_genmet_scan == None:
#            bin1 = h_2_crdy_genmet_scan.GetYaxis().FindBin(1800)
#            bin2 = h_2_crdy_genmet_scan.GetZaxis().FindBin(0)
#            h_2_crdy_genmet = h_2_crdy_genmet_scan.ProjectionX("h_mt2bins_1800_1400",bin1,bin1,bin2,bin2)
#        else:
#            h_2_crdy_genmet = None
    
    this_dir.cd()

    tl.Clear()
    if (not h_1_sig == None):
        h_1_sig.Write("sig_first")
    if (not h_2_sig == None):
        h_2_sig.Write("sig_second")
    if h_1_sig == None and not h_2_sig == None:
        h_1_sig = h_2_sig.Clone()
        h_1_sig.Scale(0)
    if not h_1_sig == None and h_2_sig == None:
        h_2_sig = h_1_sig.Clone()
        h_2_sig.Scale(0)
    if not h_1_sig == None and not h_2_sig == None:
        h_sig_ratio = h_1_sig.Clone("h_sig_ratio")
        h_sig_ratio.Divide(h_2_sig)
        h_sig_ratio.Write()
        h_sig_ratio.Draw()
        canvas.SaveAs("pngs/{0}_sig_ratio.png".format(region))
        h_1_sig.GetYaxis().SetTitleOffset(2.0)
        h_1_sig.SetTitle("{0} SR;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        h_1_sig.SetMaximum( 1.1 * max(h_1_sig.GetMaximum(),h_2_sig.GetMaximum()) )
        h_1_sig.Draw()
        h_2_sig.SetLineColor(ROOT.kRed)
        h_2_sig.Draw("same")
        if (not t1_compare):
            tl.AddEntry(h_1_sig,"Inclusive")
            tl.AddEntry(h_2_sig,"1+ Short Track")
        else:
            tl.AddEntry(h_1_sig,"T5qqqq")
            tl.AddEntry(h_2_sig,"T1qqqq")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_sig.png".format(region))

    tl.Clear()
    if (not h_1_crsl == None):
        h_1_crsl.Write("crsl_first")
    if (not h_2_crsl == None):
        h_2_crsl.Write("crsl_second")
    if h_1_crsl == None and not h_2_crsl == None:
        h_1_crsl = h_2_crsl.Clone()
        h_1_crsl.Scale(0)
    if not h_1_crsl == None and h_2_crsl == None:
        h_2_crsl = h_1_crsl.Clone()
        h_2_crsl.Scale(0)
    if not h_1_crsl == None and not h_2_crsl == None:
        h_crsl_ratio = h_1_crsl.Clone("h_crsl_ratio")
        h_crsl_ratio.Divide(h_2_crsl)
        h_crsl_ratio.Write()
        h_crsl_ratio.Draw()
        canvas.SaveAs("pngs/{0}_crsl_ratio.png".format(region))
        h_1_crsl.GetYaxis().SetTitleOffset(2.0)
        h_1_crsl.SetTitle("{0} CRSL;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        h_1_crsl.SetMaximum( 1.1 * max(h_1_crsl.GetMaximum(),h_2_crsl.GetMaximum()) )
        h_1_crsl.Draw()
        h_2_crsl.SetLineColor(ROOT.kRed)
        h_2_crsl.Draw("same")
        if (not t1_compare):
            tl.AddEntry(h_1_crsl,"Inclusive")
            tl.AddEntry(h_2_crsl,"1+ Short Track")
        else:
            tl.AddEntry(h_1_crsl,"T5qqqq")
            tl.AddEntry(h_2_crsl,"T1qqqq")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_crsl.png".format(region))

    tl.Clear()
    if (not h_1_crqcd == None):
        h_1_crqcd.Write("crqcd_first")
    if (not h_2_crqcd == None):
        h_2_crqcd.Write("crqcd_second")
    if h_1_crqcd == None and not h_2_crqcd == None:
        h_1_crqcd = h_2_crqcd.Clone()
        h_1_crqcd.Scale(0)
    if not h_1_crqcd == None and h_2_crqcd == None:
        h_2_crqcd = h_1_crqcd.Clone()
        h_2_crqcd.Scale(0)
    if (not h_1_crqcd == None and not h_2_crqcd == None):
        h_crqcd_ratio = h_1_crqcd.Clone("h_crqcd_ratio")
        h_crqcd_ratio.Divide(h_2_crqcd)
        h_crqcd_ratio.Write()
        h_crqcd_ratio.Draw()
        canvas.SaveAs("pngs/{0}_crqcd_ratio.png".format(region))
        h_1_crqcd.GetYaxis().SetTitleOffset(2.0)
        h_1_crqcd.SetTitle("{0} CRQCD;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        h_1_crqcd.SetMaximum( 1.1 * max(h_1_crqcd.GetMaximum(),h_2_crqcd.GetMaximum()) )
        h_1_crqcd.Draw()
        h_2_crqcd.SetLineColor(ROOT.kRed)
        h_2_crqcd.Draw("same")
        if (not t1_compare):
            tl.AddEntry(h_1_crqcd,"Inclusive")
            tl.AddEntry(h_2_crqcd,"1+ Short Track")
        else:
            tl.AddEntry(h_1_crqcd,"T5qqqq")
            tl.AddEntry(h_2_crqcd,"T1qqqq")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_crqcd.png".format(region))

    tl.Clear()
    if (not h_1_crdy == None):
        h_1_crdy.Write("crdy_first")
    if (not h_2_crdy == None):
        h_2_crdy.Write("crdy_second")
    if h_1_crdy == None and not h_2_crdy == None:
        h_1_crdy = h_2_crdy.Clone()
        h_1_crdy.Scale(0)
    if not h_1_crdy == None and h_2_crdy == None:
        h_2_crdy = h_1_crdy.Clone()
        h_2_crdy.Scale(0)
    if (not h_1_crdy == None and not h_2_crdy == None):
        h_crdy_ratio = h_1_crdy.Clone("h_crdy_ratio")
        h_crdy_ratio.Divide(h_2_crdy)
        h_crdy_ratio.Write()
        h_crdy_ratio.Draw()
        canvas.SaveAs("pngs/{0}_crdy_ratio.png".format(region))
        h_1_crdy.GetYaxis().SetTitleOffset(2.0)
        h_1_crdy.SetTitle("{0} CRDY;M_{{T2}} [GeV];Count / 35.9 fb^{{-1}}".format(topo))
        h_1_crdy.SetMaximum( 1.1 * max(h_1_crdy.GetMaximum(),h_2_crdy.GetMaximum()) )
        h_1_crdy.Draw()
        h_2_crdy.SetLineColor(ROOT.kRed)
        h_2_crdy.Draw("same")        
        if (not t1_compare):
            tl.AddEntry(h_1_crdy,"Inclusive")
            tl.AddEntry(h_2_crdy,"1+ Short Track")
        else:
            tl.AddEntry(h_1_crdy,"T5qqqq")
            tl.AddEntry(h_2_crdy,"T1qqqq")
        tl.Draw()
        canvas.SaveAs("pngs/{0}_crdy.png".format(region))

    return True

signal_points = [] # This list of successfully processed mass points
iterator = f_first.GetListOfKeys()
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
    #    if sr_number >= 20: continue
        # Loop over MT2 bins in this signal region
        if (doScan):
            y_binwidth = 25
            y_max = 1600
            if signal.find("T2cc") != -1:
                y_binwidth = 5
                y_max = 800
            for im1 in range(0,2301,25):
                for im2 in range(0,y_max+1,y_binwidth):
                    if (suppressUHmt2bin and directory.find("UH") != -1 and imt2 == 1): continue
                    # Replace signal placeholders and print the card
                    success = makePlot(directory)
                    # If no histograms are found for that mass point, don't add it to the list of processed points
                    if success:
                        signal_points.append( (im1,im2) )
        else:
            makePlot(directory)

    # Print signal_points to a file
    if (doScan):
        points_file = open("{0}/points_{1}.txt".format(outdir,signal),"w")
        points_file.write("--------------------------------------------\n")
        points_file.write("- saw nonzero signal entries for the following points: \n")
        for mass_point in signal_points:
            points_file.write("{0}_{1}_{2}\n".format(signal,mass_point[0],mass_point[1]))
        points_file.close()

f_out.Close()
