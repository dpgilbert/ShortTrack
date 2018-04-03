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
tl = ROOT.TLegend(0.55,0.30,0.85,0.45)
tl.SetMargin(0.1)

verbose = False # Print more status messages
#doScan = False # Not implemented

region = CRSL
indir = "../output_{0}".format(region)
outdir = pngs_CRSL
if (not os.path.exists(outdir)): os.mkdir(outdir)

#selection_suffixes = ["incl","HT","HTMET","HTMETDphiDiff","HTMETDphiDiffMT2"]
#selection_suffixes = ["incl","HT","HTMET","DphiDiff","MT2","Lep","HTLep","LepNj"]
#selection_suffixes = ["incl","LepNj","HTLep","Lep"]
#selection_suffixes = ["incl","HTLep","HTMETLep"]
#selection_suffixes = ["incl","HTLep"]
selection_suffixes = ["incl"]
histnames_base = ["h_el_pix","h_el_ext","h_el_tot","h_fake_pix","h_fake_ext","h_fake_tot","h_el_etaphi","h_fake_etaphi","h_el_ratio","h_el_met","h_fake_met"]
histnames_s = [histname + "_s" for histname in histnames_base]
histnames_ts = [histname + "_ts" for histname in histnames_base]
histnames_tl = [histname + "_tl" for histname in histnames_base]
histnames_unique = ["h_all_chDR_s","h_all_anyDR_s","h_all_chID_s","h_all_anyID_s","h_all_nearby_s"]
#histnames = histnames_base + histnames_s + histnames_ts + histnames_tl + histnames_unique
histnames = histnames_unique
if (verbose): print histnames
samples = ["ttsl","ttdl","DY","Wjets","qcd300to500","qcd500to700","qcd700to1000","qcd1000to1500","qcd1500to2000","qcd2000toInf","zinv100to200","zinv200toInf"]
colors = [ROOT.kBlack,ROOT.kRed,ROOT.kGreen,ROOT.kCyan,ROOT.kMagenta,ROOT.kBlue,ROOT.kOrange,ROOT.kYellow-2,ROOT.kGray+1,ROOT.kAzure+7,ROOT.kRed+3]
if (len(selection_suffixes) > len(colors)):
    print str(len(selection_suffixes)) + " selections and only " + str(len(colors)) + " colors"
    exit(1)
selection_colors = dict(zip(selection_suffixes,colors))
if (verbose):
    print selection_suffixes
    print selection_colors

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
    draw_option = ""
    if (histname.find("ID") >= 0): draw_option += "hist"
    for sample in samples:
        print "> " + sample
        tl.Clear()
        canvas.SetLogy(True)
        if (histname.find("etaphi") < 0): 
            for selection in selection_suffixes:
                print "> > " + selection
                current_hist = filedict[selection][sample].Get(histname)
                if (verbose): print "> > " + str(current_hist.Integral())
                current_hist.SetLineColor(selection_colors[selection])
                if (selection.find("incl") >= 0):
                    current_hist.SetTitle(sample+" "+histname)
                    current_hist.SetMinimum(0.1)                    
                else:
                    draw_option += "same"
                current_hist.Draw(draw_option)
                tl.AddEntry(current_hist,selection)
            tl.Draw()
        else:
            for selection in selection_suffixes:
                print "> > " + selection
                current_hist = filedict[selection][sample].Get(histname).ProjectionX(selection)
                rebinned_hist = current_hist.Rebin(100)
                if (verbose): print "> > projection: " + str(rebinned_hist.Integral())
                rebinned_hist.SetLineColor(selection_colors[selection])
                tl.AddEntry(rebinned_hist,selection)
                if (selection.find("incl") >= 0):
                    rebinned_hist.SetTitle(sample+" "+histname+"_etaproj")
                    rebinned_hist.SetMinimum(0.1)
                    rebinned_hist.Draw()
                else:
                    rebinned_hist.Draw("same")
            tl.Draw()
            canvas.SaveAs("{0}/{1}_{2}.png".format(outdir,sample,histname+"_etaproj"))
            canvas.SetLogy(False)
            current_hist = filedict["incl"][sample].Get(histname)
            current_hist.SetTitle(sample+" "+histname)
            current_hist.Draw("colz")
        canvas.SaveAs("{0}/{1}_{2}.png".format(outdir,sample,histname))
        canvas.SetLogy(False)

canvas.SetTicks(1,2)
            
outfile = ROOT.TFile.Open("{0}.root".format(region),"RECREATE")
for selection in selection_suffixes:
    h_ratios = ROOT.TH1F("h_ratios","N_{ST} > 0 Reduction Factor, by Sample",len(samples),0,len(samples))
    h_ratios_fakes = ROOT.TH1F("h_ratios_fakes","N_{ST} > 0 Reduction Factor (Fakes Only), by Sample",len(samples),0,len(samples))
    for i in range(len(samples)):
        sample = samples[i]
        num = filedict[selection][sample].Get("h_unskimmed").GetBinContent(1)
        denomfile = filedict[selection][sample]
        denom1 = denomfile.Get("h_fake_tot").Integral()
        denom2 = denomfile.Get("h_el_tot").Integral() + denom1
        bin = i+1
        toSet = 0
        if (denom2 > 0): 
            toSet = num / denom2
            dsq = denom2 * denom2
            # Error calculation taken from ROOT: https://root.cern.ch/doc/master/TH1_8cxx_source.html#l02817
            h_ratios.SetBinError(bin, sqrt(num * dsq + denom2 * num * num) / dsq )
        h_ratios.SetBinContent(bin,toSet)
        h_ratios.GetXaxis().SetBinLabel(bin, "{0} ({1})".format( sample,str(int(denom2)) ) )
        toSet = 0
        if (denom1 > 0): 
            toSet = num / denom1
            dsq = denom1 * denom1
            h_ratios_fakes.SetBinError(bin, sqrt(num * dsq + denom1 * num * num) / dsq )
        h_ratios_fakes.SetBinContent(bin,toSet)
        h_ratios_fakes.GetXaxis().SetBinLabel(bin,"{0} ({1})".format( sample,str(int(denom1)) ) )
    h_ratios.SetMinimum(0)
    h_ratios.SetFillColor(ROOT.kBlue)
    h_ratios.SetLineColor(ROOT.kBlue)
    h_ratios.Draw()
    canvas.SaveAs("{0}/factor_{1}.png".format(outdir,selection))
    h_ratios.Write()
    h_ratios_fakes.SetMinimum(0)
    h_ratios_fakes.SetFillColor(ROOT.kRed)
    h_ratios_fakes.SetLineColor(ROOT.kRed)
    h_ratios_fakes.Draw()
    canvas.SaveAs("{0}/factor_fakes_{1}.png".format(outdir,selection))
    h_ratios_fakes.Write()
outfile.Close()
