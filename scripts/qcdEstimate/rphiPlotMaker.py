#! /usr/bin/env python
## Usage: python rPhiPlotMaker.py rphi_hists/rphi_hists_VL.root
## makes a plot and puts in rphi_plots directory
## runs on the output of rphiLooper.C

import sys
import numpy as np
import ROOT

basename = sys.argv[1].split('/')[-1].replace(".root","")

ROOT.gStyle.SetOptStat(0)

ht_reg = basename[-2:]
if ht_reg[0]=='_':
    ht_reg = ht_reg[1:]
ht_bounds = {'VL':(200,450),'L':(450,575),'M':(575,1000),'H':(1000,1500),'UH':(1500,"Inf")}

f = ROOT.TFile("rphi_hists/{0}.root".format(basename))
h_rphi_num = f.Get("h_rphi_num")
h_rphi_den = f.Get("h_rphi_den")

mt2bins = [50,55,60,65,70,75,80,90,100,120,200,300,450,800,1500]
mt2bins = np.array(mt2bins, dtype=float)

h_rphi_num = h_rphi_num.Rebin(mt2bins.size-1,"h_rphi_num",mt2bins)
h_rphi_den = h_rphi_den.Rebin(mt2bins.size-1,"h_rphi_den",mt2bins)

h_rphi = ROOT.TH1D()
h_rphi_num.Copy(h_rphi)
h_rphi.Divide(h_rphi_den)

fit = ROOT.TF1("fitfunc","[0]*x^[1]",mt2bins[0],mt2bins[-1])
fit.SetParameter(0,20)
#fit was strangely converging to bad values in some cases, so restrict the values
fit.SetParLimits(0,10,1e9)
lowbound = 60 if ht_reg in ['VL','L','M'] else 70
h_rphi.Fit("fitfunc","QN","goff",lowbound,100)

c = ROOT.TCanvas("c1","c1",600,650)
c.SetLogx(1)
c.SetLogy(1)
c.SetLeftMargin(0.12)
c.SetTopMargin(0.08)

h_rphi.SetMarkerStyle(20)
h_rphi.SetLineColor(ROOT.kBlack)
h_rphi.SetMarkerColor(ROOT.kBlack)
h_rphi.SetTitle(basename)
h_rphi.GetXaxis().SetRangeUser(50,1500)
h_rphi.GetXaxis().SetTitle("M_{T2} [GeV]")
h_rphi.GetXaxis().SetTitleOffset(1.3)
h_rphi.GetYaxis().SetRangeUser(0.01,200)
h_rphi.GetYaxis().SetTitle("Ratio")
h_rphi.GetYaxis().SetTitleOffset(1.4)

fit.SetLineWidth(2)
fit.SetLineColor(ROOT.kRed+1)

h_rphi.Draw("PE0")

line = ROOT.TLine()
line.SetLineColor(ROOT.kBlack)
line.SetLineWidth(1)
line.SetLineStyle(2)
line.DrawLine(lowbound,0.01,lowbound,200)
line.DrawLine(100,0.01,100,200)

fit.Draw("SAME")
h_rphi.Draw("SAMEPE0")

leg = ROOT.TLegend(0.6,0.75,0.89,0.89)
leg.SetFillStyle(0)
leg.SetTextFont(62)
leg.SetHeader("{0} < H_{{T}} < {1} GeV".format(*(ht_bounds[ht_reg])))
leg.AddEntry(h_rphi,"MC (QCD only)",'p')
leg.AddEntry(fit,"Fit",'l')
leg.SetBorderSize(0)
leg.Draw()

c.SaveAs("rphi_plots/{0}.pdf".format(basename))
c.SaveAs("rphi_plots/{0}.png".format(basename))

## uncomment to pause to view plot
#raw_input()

