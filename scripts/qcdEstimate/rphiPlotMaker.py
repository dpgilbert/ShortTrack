## used from makeRphiPlots.py

import os
import sys
import numpy as np
import ROOT

def makeMCPlot(qcd_file, nqcd_files, outdir, ht_reg):
    ht_bounds = {'VL':(200,450),'L':(450,575),'M':(575,1000),'H':(1000,1500),'UH':(1500,"Inf")}

    fqcd = ROOT.TFile(qcd_file)
    h_qcd_num = fqcd.Get("rphi_{0}/h_mt2_num".format(ht_reg))
    h_qcd_den = fqcd.Get("rphi_{0}/h_mt2_den".format(ht_reg))
    h_qcd_num.SetDirectory(0)
    h_qcd_den.SetDirectory(0)
    fqcd.Close()

    h_nqcd_num = ROOT.TH1D()
    h_nqcd_den = ROOT.TH1D()
    h_qcd_num.Copy(h_nqcd_num)
    h_qcd_den.Copy(h_nqcd_den)
    for fn in nqcd_files:
        f = ROOT.TFile(fn)
        if f.GetSize==-1:
            raise Exception
        try:
            h_nqcd_num.Add(f.Get("rphi_{0}/h_mt2_num".format(ht_reg)))
            h_nqcd_den.Add(f.Get("rphi_{0}/h_mt2_den".format(ht_reg)))
        except:
            pass
        f.Close()

    mt2bins = [50,55,60,65,70,75,80,90,100,120,200,300,450,800,1500]
    mt2bins = np.array(mt2bins, dtype=float)
    
    h_qcd_num  = h_qcd_num.Rebin(mt2bins.size-1,"h_qcd_num",mt2bins)
    h_qcd_den  = h_qcd_den.Rebin(mt2bins.size-1,"h_qcd_den",mt2bins)
    h_nqcd_num = h_nqcd_num.Rebin(mt2bins.size-1,"h_nqcd_num",mt2bins)
    h_nqcd_den = h_nqcd_den.Rebin(mt2bins.size-1,"h_nqcd_den",mt2bins)
    
    h_qcd_rphi = ROOT.TH1D()
    h_qcd_num.Copy(h_qcd_rphi)
    h_qcd_rphi.Divide(h_qcd_den)

    h_nqcd_rphi = ROOT.TH1D()
    h_nqcd_num.Copy(h_nqcd_rphi)
    h_nqcd_rphi.Divide(h_nqcd_den)
    
    fit = ROOT.TF1("fitfunc","[0]*x^[1]",mt2bins[0],mt2bins[-1])
    fit.SetParameter(0,20)
    # fit was strangely converging to bad values in some cases, so restrict the values
    fit.SetParLimits(0,10,1e9)
    lowbound = 60 if ht_reg in ['VL','L','M'] else 70
    h_qcd_rphi.Fit("fitfunc","QN","goff",lowbound,100)
    
    ROOT.gStyle.SetOptStat(0)
    c = ROOT.TCanvas("c1","c1",600,650)
    c.SetLogx(1)
    c.SetLogy(1)
    c.SetLeftMargin(0.12)
    c.SetTopMargin(0.08)
    
    h_qcd_rphi.SetMarkerStyle(24)
    h_qcd_rphi.SetLineColor(ROOT.kBlack)
    h_nqcd_rphi.SetMarkerStyle(20)
    h_nqcd_rphi.SetLineColor(ROOT.kBlack)

    h_qcd_rphi.SetTitle("")
    h_qcd_rphi.GetXaxis().SetRangeUser(50,1500)
    h_qcd_rphi.GetXaxis().SetTitle("M_{T2} [GeV]")
    h_qcd_rphi.GetXaxis().SetTitleOffset(1.3)
    h_qcd_rphi.GetYaxis().SetRangeUser(0.01,200)
    h_qcd_rphi.GetYaxis().SetTitle("Ratio")
    h_qcd_rphi.GetYaxis().SetTitleOffset(1.4)
    
    fit.SetLineWidth(2)
    fit.SetLineColor(ROOT.kRed+1)

    h_qcd_rphi.Draw("PE0")

    line = ROOT.TLine()
    line.SetLineColor(ROOT.kBlack)
    line.SetLineWidth(1)
    line.SetLineStyle(2)
    line.DrawLine(lowbound,0.01,lowbound,200)
    line.DrawLine(100,0.01,100,200)
    
    fit.Draw("SAME")
    h_qcd_rphi.Draw("SAMEPE0")
    h_nqcd_rphi.Draw("SAMEPE0")

    leg = ROOT.TLegend(0.3,0.75,0.59,0.89)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetHeader("{0} < H_{{T}} < {1} GeV".format(*(ht_bounds[ht_reg])))
    leg.AddEntry(h_nqcd_rphi,"MC (all)",'p')
    leg.AddEntry(h_qcd_rphi,"MC (QCD only)",'p')
    leg.AddEntry(fit,"Fit",'l')
    leg.SetBorderSize(0)
    leg.Draw()
    
    c.SaveAs(os.path.join(outdir,"rphi_{0}.pdf".format(ht_reg)))
    c.SaveAs(os.path.join(outdir,"rphi_{0}.png".format(ht_reg)))
    
