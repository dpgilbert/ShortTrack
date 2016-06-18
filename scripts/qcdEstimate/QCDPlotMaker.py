## for use within makeQCDPlots.py

import os
import sys
import numpy as np
import ROOT

def makeRbPlot(h_mc, h_data, nj_reg, outfile):
    nj_reg = ["j2to3","j4to6","j7toInf"].index(nj_reg)

    nj_bounds = [(2,3),(4,6),(7,"Inf")]

    h_mcerr = h_mc.Clone("h_mcerr")

    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)

    h_mc.SetLineColor(ROOT.kRed)
    h_mc.SetLineWidth(2)
    h_mc.SetTitle("")
    h_mc.GetXaxis().SetTitle("N(bjets)")
    h_mc.GetXaxis().SetTitleOffset(1.3)
    h_mc.GetXaxis().SetNdivisions(6)
    h_mc.GetYaxis().SetTitle("r_{b}")
    h_mc.GetYaxis().SetTitleOffset(1.4)
    h_mc.GetYaxis().SetRangeUser(0,1.0)

    h_data.SetMarkerStyle(20)
    h_data.SetMarkerColor(ROOT.kBlack)
    h_data.SetLineColor(ROOT.kBlack)

    h_mcerr.SetFillStyle(3244)
    h_mcerr.SetFillColor(ROOT.kGray+1)

    h_mc.Draw("HIST")
    h_mcerr.Draw("SAME E2")
    h_data.Draw("SAME P E0")

    leg = ROOT.TLegend(0.6,0.75,0.89,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} #leq N(jets) #leq {1} ".format(*(nj_bounds[nj_reg])))
    leg.AddEntry(h_data,"Data",'p')
    leg.AddEntry(h_mc,"QCD MC",'l')
    leg.SetBorderSize(0)
    leg.Draw()

    c.SaveAs(outfile+".pdf")
    c.SaveAs(outfile+".png")
    

def makeFjPlot(h_mc, h_data, ht_reg, outfile):
    ht_reg = ["ht200to450","ht450to575","ht575to1000",
              "ht1000to1500","ht1500toInf"].index(ht_reg)

    ht_bounds = [(200,450),(450,575),(575,1000),(1000,1500),(1500,"Inf")]

    h_mcerr = h_mc.Clone("h_mcerr")

    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)

    h_mc.SetLineColor(ROOT.kRed)
    h_mc.SetLineWidth(2)
    h_mc.SetTitle("")
    h_mc.GetXaxis().SetTitle("N(jets)")
    h_mc.GetXaxis().SetTitleOffset(1.3)
    h_mc.GetXaxis().SetNdivisions(9)
    h_mc.GetYaxis().SetTitle("f_{j}")
    h_mc.GetYaxis().SetTitleOffset(1.4)
    h_mc.GetYaxis().SetRangeUser(0,1.2)

    h_data.SetMarkerStyle(20)
    h_data.SetMarkerColor(ROOT.kBlack)
    h_data.SetLineColor(ROOT.kBlack)

    h_mcerr.SetFillStyle(3244)
    h_mcerr.SetFillColor(ROOT.kGray+1)

    h_mc.Draw("HIST")
    h_mcerr.Draw("SAME E2")
    h_data.Draw("SAME P E0")


    leg = ROOT.TLegend(0.6,0.75,0.89,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} < H_{{T}} < {1} GeV".format(*(ht_bounds[ht_reg])))
    leg.AddEntry(h_data,"Data",'p')
    leg.AddEntry(h_mc,"QCD MC",'l')
    leg.SetBorderSize(0)
    leg.Draw()

    c.SaveAs(outfile+".pdf")
    c.SaveAs(outfile+".png")
    

def makeRphiPlot(h_all, h_qcd, ht_reg, isData, outfile):
    ht_reg = ["ht200to450","ht450to575","ht575to1000",
              "ht1000to1500","ht1500toInf"].index(ht_reg)

    ht_bounds = [(200,450),(450,575),(575,1000),(1000,1500),(1500,"Inf")]

    
    fit = ROOT.TF1("fitfunc","[0]*x^[1]",50,1500)
    fit.SetParameter(0,20)
    # fit was strangely converging to bad values in some cases, so restrict the values
    fit.SetParLimits(0,10,1e9)
    lowbound = 60 if ht_reg in [0,1,2] else 70
    h_qcd.Fit("fitfunc","QN","goff",lowbound,100)
    
    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLogx(1)
    c.SetLogy(1)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)
    
    h_qcd.SetMarkerStyle(24)
    h_qcd.SetLineColor(ROOT.kBlack)
    h_all.SetMarkerStyle(20)
    h_all.SetLineColor(ROOT.kBlack)

    h_qcd.SetTitle("")
    h_qcd.GetXaxis().SetRangeUser(50,1500)
    h_qcd.GetXaxis().SetTitle("M_{T2} [GeV]")
    h_qcd.GetXaxis().SetTitleOffset(1.3)
    h_qcd.GetYaxis().SetRangeUser(0.01,200)
    h_qcd.GetYaxis().SetTitle("r_{#phi}")
    h_qcd.GetYaxis().SetTitleOffset(1.4)
    
    fit.SetLineWidth(2)
    fit.SetLineColor(ROOT.kRed+1)

    h_qcd.Draw("PE0")

    line = ROOT.TLine()
    line.SetLineColor(ROOT.kBlack)
    line.SetLineWidth(1)
    line.SetLineStyle(2)
    line.DrawLine(lowbound,0.01,lowbound,200)
    line.DrawLine(100,0.01,100,200)
    
    fit.Draw("SAME")
    h_qcd.Draw("SAMEPE0")
    h_all.Draw("SAMEPE0")

    leg = ROOT.TLegend(0.3,0.75,0.59,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} < H_{{T}} < {1} GeV".format(*(ht_bounds[ht_reg])))
    if isData:
        leg.AddEntry(h_all,"Data (all)",'p')
        leg.AddEntry(h_qcd,"Data (non-QCD subtracted)",'p')
    else:
        leg.AddEntry(h_all,"MC (all)",'p')
        leg.AddEntry(h_qcd,"MC (QCD only)",'p')
    leg.AddEntry(fit,"Fit",'l')
    leg.SetBorderSize(0)
    leg.Draw()
    
    c.SaveAs(outfile+".pdf")
    c.SaveAs(outfile+".png")
    
