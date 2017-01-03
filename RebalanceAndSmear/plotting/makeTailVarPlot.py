import ROOT
import glob
import os

ROOT.gROOT.SetBatch(1)

dir = "looper_output/RebalanceAndSmear_V00-08-12_fixedBtag_v2"
dir_tail50 = "looper_output/RebalanceAndSmear_V00-08-12_tail50"
dir_tail100 = "looper_output/RebalanceAndSmear_V00-08-12_tail100"
dir_noRS = "looper_output/RebalanceAndSmear_V00-08-12_noRS"

hrs = ROOT.TH1D("hrs","",44,0,44)
h50 = ROOT.TH1D("h50","",44,0,44)
h100 = ROOT.TH1D("h100","",44,0,44)
hnrs = ROOT.TH1D("hnrs","",44,0,44)

h_evts_rs = ROOT.TH1D("h_evts_rs","",1,0,2)
h_evts_50 = ROOT.TH1D("h_evts_50","",1,0,2)
h_evts_100 = ROOT.TH1D("h_evts_100","",1,0,2)
h_evts_nrs = ROOT.TH1D("h_evts_nrs","",1,0,2)

ibin = 0
for ht_reg in ["L","M","H","UH"]:
    sum_rs = 0
    sum_rs50 = 0
    sum_rs100 = 0    
    for top_reg in range(1,12):
        ibin+=1
        # print ibin

        h_evts_rs.Reset()
        h_evts_50.Reset()
        h_evts_100.Reset()
        h_evts_nrs.Reset()
        for fn in [os.path.join(dir,"qcd_ht_nonext.root"), os.path.join(dir,"qcd_ht_ext.root")]:
            fact = 0.686
            if fn.find("nonext") > -1:
                fact = 1-0.686
            bn = fn.split("/")[-1]
            frs = ROOT.TFile(os.path.join(dir,bn))
            f50 = ROOT.TFile(os.path.join(dir_tail50,bn))
            f100 = ROOT.TFile(os.path.join(dir_tail100,bn))
            fnrs = ROOT.TFile(os.path.join(dir_noRS,bn))
            try:
                h_evts_rs.Add(frs.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg)),fact)
            except:
                pass
            try:
                h_evts_50.Add(f50.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg)),fact)
            except:
                pass
            try:
                htemp = f100.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg)).Clone()
                err = ROOT.Double()
                htemp.SetBinContent(1,f100.Get("sr{0}{1}/h_met".format(top_reg,ht_reg)).IntegralAndError(0,-1,err))
                htemp.SetBinError(1,err)
                                 
                h_evts_100.Add(htemp, fact)
            except:
                pass
            try:
                h_evts_nrs.Add(fnrs.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg)),fact)
            except:
                pass

        hrs.SetBinContent(ibin, h_evts_rs.GetBinContent(1))
        hrs.SetBinError(ibin, h_evts_rs.GetBinError(1))

        h50.SetBinContent(ibin, h_evts_50.GetBinContent(1))
        h50.SetBinError(ibin, h_evts_50.GetBinError(1))

        h100.SetBinContent(ibin, h_evts_100.GetBinContent(1))
        h100.SetBinError(ibin, h_evts_100.GetBinError(1))

        hnrs.SetBinContent(ibin, h_evts_nrs.GetBinContent(1))
        hnrs.SetBinError(ibin, h_evts_nrs.GetBinError(1))

        if "UH" in ht_reg and top_reg == 5: continue
        sum_rs += h_evts_rs.GetBinContent(1)
        sum_rs50 += h_evts_50.GetBinContent(1)
        sum_rs100 += h_evts_100.GetBinContent(1)

    print "{0} HT: rs = {1}, rs50 = {2}, ratio = {3}".format(ht_reg, sum_rs, sum_rs50, sum_rs50/sum_rs)
    print "{0} HT: rs = {1}, rs100 = {2}, ratio = {3}".format(ht_reg, sum_rs, sum_rs100, sum_rs100/sum_rs)        

ROOT.gStyle.SetOptStat(0)

c = ROOT.TCanvas("c","c",900,600)

pads = []
pads.append(ROOT.TPad("1","1",0.0,0.18,1.0,1.0))
pads.append(ROOT.TPad("2","2",0.0,0.0,1.0,0.19))

pads[0].SetTopMargin(0.08)
pads[0].SetBottomMargin(0.13)
pads[0].SetRightMargin(0.05)
pads[0].SetLeftMargin(0.10)

pads[1].SetRightMargin(0.05)
pads[1].SetLeftMargin(0.10)

pads[0].Draw()
pads[1].Draw()
pads[0].cd()

pads[0].SetLogy(1)
pads[1].SetTickx(1)
pads[0].SetTicky(1)
pads[1].SetTicky(1)

hrs.SetLineColor(ROOT.kBlack)
hrs.SetMarkerColor(ROOT.kBlack)
hrs.SetMarkerStyle(20)

h50.SetLineColor(ROOT.kBlue)
h50.SetMarkerColor(ROOT.kBlue)
h50.SetMarkerStyle(20)

h100.SetLineColor(ROOT.kRed)
h100.SetMarkerColor(ROOT.kRed)
h100.SetMarkerStyle(20)

hnrs.SetLineColor(401)
hnrs.SetMarkerColor(401)
hnrs.SetMarkerStyle(20)

hnrs.GetYaxis().SetRangeUser(1e-3,1e3)
hnrs.GetXaxis().SetLabelSize(0)

hnrs.Draw("PE")
hrs.Draw("PE SAME")
h50.Draw("PE SAME")
h100.Draw("PE SAME")

line = ROOT.TLine()
line.SetLineStyle(2)
for ix in [11,22,33]:
    x = pads[0].GetLeftMargin() + ix/44.0 * (1-pads[0].GetLeftMargin()-pads[0].GetRightMargin())
    line.DrawLineNDC(x,1-pads[0].GetTopMargin(),x,pads[0].GetBottomMargin())

leg = ROOT.TLegend(0.815,0.78,0.94,0.9)
leg.AddEntry(hnrs, "QCD MC")
leg.AddEntry(hrs, "R&S from MC")
leg.AddEntry(h50, "Tail +50%")
leg.AddEntry(h100, "Tail +100%")
leg.Draw()

text = ROOT.TLatex()
text.SetNDC(1)
text.SetTextSize(0.03)
text.DrawLatex(0.18,0.79,"Low H_{T}")
text.DrawLatex(0.39,0.79,"Medium H_{T}")
text.DrawLatex(0.60,0.79,"High H_{T}")
text.DrawLatex(0.81,0.73,"Extreme H_{T}")
text.SetTextFont(42)
text.SetTextSize(0.04)
text.DrawLatex(0.8,0.93,"1 fb^{-1} (13 TeV)")


binWidth = (1-pads[0].GetLeftMargin()-pads[1].GetRightMargin())/44.0
binLabels = ["2-3j, 0b", "2-3j, 1b", "2-3j, 2b", "4-6j, 0b", "4-6j, 1b", "4-6j, 2b", "#geq7j, 0b", "#geq7j, 1b", "#geq7j, 2b", "2-6j, #geq3b", "#geq7j, #geq3b"]
text = ROOT.TLatex()
text.SetNDC(1)
text.SetTextAlign(32)
text.SetTextAngle(90)
text.SetTextSize(min(binWidth * 1.3,0.027))
text.SetTextFont(42)
for ibin in range(11):
    x = pads[0].GetLeftMargin() + (ibin+0.5)*binWidth
    y = pads[0].GetBottomMargin()-0.009
    text.DrawLatex(x,y,binLabels[ibin])
    text.DrawLatex(x+11*binWidth,y,binLabels[ibin])
    text.DrawLatex(x+22*binWidth,y,binLabels[ibin])
    text.DrawLatex(x+33*binWidth,y,binLabels[ibin])



## ratio
pads[1].cd()

h_ratio50 = h50.Clone("h_ratio")
h_ratio50.Divide(hrs)

h_ratio100 = h100.Clone("h_ratio")
h_ratio100.Divide(hrs)

h_ratio50.GetYaxis().SetRangeUser(0,3)
h_ratio50.GetYaxis().SetNdivisions(505)
h_ratio50.GetYaxis().SetTitle("Shift/R&S")
h_ratio50.GetYaxis().SetTitleSize(0.16)
h_ratio50.GetYaxis().SetTitleOffset(0.18)
h_ratio50.GetYaxis().SetLabelSize(0.13)
h_ratio50.GetYaxis().CenterTitle()
h_ratio50.GetYaxis().SetTickLength(0.02)
h_ratio50.GetXaxis().SetLabelSize(0)
h_ratio50.GetXaxis().SetTitle("")
h_ratio50.GetXaxis().SetNdivisions(44,0,0)
h_ratio50.GetXaxis().SetTickSize(0.06)
h_ratio50.SetMarkerStyle(20)
h_ratio50.SetMarkerSize(1.0)
h_ratio50.SetLineWidth(1)

h_ratio50.Draw("PE")
h_ratio100.Draw("PE SAME")

line = ROOT.TLine()
line.DrawLine(0,1,44,1)

c.SaveAs("/home/users/fgolf/public_html/mt2/RebalanceAndSmear/MCtests/tail_var.pdf")
c.SaveAs("/home/users/fgolf/public_html/mt2/RebalanceAndSmear/MCtests/tail_var.png")


