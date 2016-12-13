import ROOT
import glob
import os

ROOT.gROOT.SetBatch(1)

dir = "looper_output/RebalanceAndSmear_V00-08-12_data"
dir_mc = "looper_output/RebalanceAndSmear_V00-08-12_fixedBtag_v2"

hrs = ROOT.TH1D("hrs","",44,0,44)
hnrs = ROOT.TH1D("hnrs","",44,0,44)

h_evts_nrs = ROOT.TH1D("h_evts_nrs","",1,0,2)

ibin = 0
for ht_reg in ["L","M","H","UH"]:
    for top_reg in range(1,12):
        ibin+=1
        print ibin

        h_evts_nrs.Reset()
        for fn in [os.path.join(dir_mc,"qcd_ht_nonext.root"), os.path.join(dir_mc,"qcd_ht_ext.root")]:
            fact = 0.686
            if fn.find("nonext") > -1:
                fact = 1-0.686
            bn = fn.split("/")[-1]
            fnrs = ROOT.TFile(os.path.join(dir_mc,bn))
            try:
                h_evts_nrs.Add(fnrs.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg)),fact*12.9)
            except:
                pass

        # if ht_reg=='UH': scale = 1.0/2
        # if ht_reg=='H': scale = 1.0/4
        # if ht_reg=='M': scale = 1.0/8
        # if ht_reg=='L': scale = 1.0/16        
        fdata = ROOT.TFile(os.path.join(dir,"data_Run2016.root"))
        h_data = fdata.Get("sr{0}{1}/h_met".format(top_reg,ht_reg))
        h_data.Scale(scale)

        err = ROOT.Double()
        hrs.SetBinContent(ibin, h_data.IntegralAndError(0,-1,err))
        hrs.SetBinError(ibin, err)

        hnrs.SetBinContent(ibin, h_evts_nrs.GetBinContent(1))
        hnrs.SetBinError(ibin, h_evts_nrs.GetBinError(1))


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

hnrs.SetLineColor(401)
hnrs.SetMarkerColor(401)
hnrs.SetMarkerStyle(20)

hnrs.GetYaxis().SetRangeUser(1e-2,1e4)
hnrs.GetXaxis().SetLabelSize(0)

hnrs.Draw("PE")
hrs.Draw("PE SAME")

line = ROOT.TLine()
line.SetLineStyle(2)
for ix in [11,22,33]:
    x = pads[0].GetLeftMargin() + ix/44.0 * (1-pads[0].GetLeftMargin()-pads[0].GetRightMargin())
    line.DrawLineNDC(x,1-pads[0].GetTopMargin(),x,pads[0].GetBottomMargin())

leg = ROOT.TLegend(0.815,0.78,0.94,0.9)
leg.AddEntry(hrs, "R&S from Data")
leg.AddEntry(hnrs, "R&S from MC")
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
text.DrawLatex(0.8,0.93,"12.9 fb^{-1} (13 TeV)")


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

h_ratio = hrs.Clone("h_ratio")
h_ratio.Divide(hnrs)

h_ratio.GetYaxis().SetRangeUser(0,3)
h_ratio.GetYaxis().SetNdivisions(505)
h_ratio.GetYaxis().SetTitle("R&S/MC")
h_ratio.GetYaxis().SetTitleSize(0.16)
h_ratio.GetYaxis().SetTitleOffset(0.18)
h_ratio.GetYaxis().SetLabelSize(0.13)
h_ratio.GetYaxis().CenterTitle()
h_ratio.GetYaxis().SetTickLength(0.02)
h_ratio.GetXaxis().SetLabelSize(0)
h_ratio.GetXaxis().SetTitle("")
h_ratio.GetXaxis().SetNdivisions(44,0,0)
h_ratio.GetXaxis().SetTickSize(0.06)
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerSize(1.0)
h_ratio.SetLineWidth(1)

h_ratio.Draw("PE")

line = ROOT.TLine()
line.DrawLine(0,1,44,1)

# c.SaveAs("/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/MCclosure/RS_MC_closure.pdf")
# c.SaveAs("/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/MCclosure/RS_MC_closure.png")

c.SaveAs("/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/data_tests/ICHEP/closure.pdf")
c.SaveAs("/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/data_tests/ICHEP/closure.png")

raw_input()
