import ROOT
import glob
import os

ROOT.gROOT.SetBatch(1)

dir_rs = "looper_output/v10/data_noRS/"
dir_mt2 = "/home/users/fgolf/mt2/devel/MT2Analysis/MT2looper/output/full2016/"

frs = ROOT.TFile(os.path.join(dir_rs,"merged_hists_mt2babies.root"))
fms = ROOT.TFile(os.path.join(dir_mt2,"data_Run2016.root"))

hrs = ROOT.TH1D("hrs","",51,0,51)
hms = ROOT.TH1D("hms","",51,0,51)

h_evts_rs = ROOT.TH1D("h_evts_rs","",1,0,2)
h_evts_ms = ROOT.TH1D("h_evts_ms","",1,0,2)

top_regs_vl=[1,2,3,12,13,14,15]

ibin = 0
for ht_reg in ["VL","L","M","H","UH"]:
  top_regs = []
  if ht_reg == "VL":
    top_regs = top_regs_vl
  else:
    top_regs.extend(range(1,12))
  for top_reg in top_regs:      
    ibin+=1
    print ibin

    h_evts_rs.Reset()
    h_evts_ms.Reset()
    try:
      h_evts_rs = frs.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg))
    except:
      pass
    try:
      h_evts_ms = fms.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg))
    except:
      pass

    hrs.SetBinContent(ibin, h_evts_rs.GetBinContent(1))
    hrs.SetBinError(ibin, h_evts_rs.GetBinError(1))

    hms.SetBinContent(ibin, h_evts_ms.GetBinContent(1))
    hms.SetBinError(ibin, h_evts_ms.GetBinError(1))


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

hms.SetLineColor(ROOT.kBlue)
hms.SetMarkerColor(ROOT.kBlue)
hms.SetMarkerStyle(20)

hms.GetYaxis().SetRangeUser(1e-3,1e5)
hms.GetXaxis().SetLabelSize(0)

hms.Draw("PE")
hrs.Draw("PE SAME")

line = ROOT.TLine()
line.SetLineStyle(2)
for ix in [7,18,29,40]:
    x = pads[0].GetLeftMargin() + ix/51.0 * (1-pads[0].GetLeftMargin()-pads[0].GetRightMargin())
    line.DrawLineNDC(x,1-pads[0].GetTopMargin(),x,pads[0].GetBottomMargin())

leg = ROOT.TLegend(0.815,0.78,0.94,0.9)
leg.AddEntry(hrs, "R&S looper")
leg.AddEntry(hms, "MT2 looper")
leg.Draw()

text = ROOT.TLatex()
text.SetNDC(1)
text.SetTextSize(0.03)
text.DrawLatex(0.12,0.79,"Very Low H_{T}")
text.DrawLatex(0.3,0.79,"Low H_{T}")
text.DrawLatex(0.45,0.79,"Medium H_{T}")
text.DrawLatex(0.65,0.79,"High H_{T}")
text.DrawLatex(0.8,0.73,"Extreme H_{T}")
text.SetTextFont(42)
text.SetTextSize(0.04)
text.DrawLatex(0.8,0.93,"36.5 fb^{-1} (13 TeV)")


binWidth = (1-pads[0].GetLeftMargin()-pads[1].GetRightMargin())/51.0
binLabels_vl = ["2-3j, 0b", "2-3j, 1b", "2-3j, 2b", "#geq4j, 0b", "#geq4j, 1b", "#geq4j, 2b", "#geq2j, #geq3b"] 
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
  if ibin < 7:
    text.DrawLatex(x,y,binLabels_vl[ibin])
  text.DrawLatex(x+7*binWidth,y,binLabels[ibin])    
  text.DrawLatex(x+18*binWidth,y,binLabels[ibin])
  text.DrawLatex(x+29*binWidth,y,binLabels[ibin])
  text.DrawLatex(x+40*binWidth,y,binLabels[ibin])


## ratio
pads[1].cd()

h_ratio = hms.Clone("h_ratio")
h_ratio.Divide(hrs)

h_ratio.GetYaxis().SetRangeUser(0,2)
h_ratio.GetYaxis().SetNdivisions(505)
h_ratio.GetYaxis().SetTitle("MT2/R&S")
h_ratio.GetYaxis().SetTitleSize(0.16)
h_ratio.GetYaxis().SetTitleOffset(0.18)
h_ratio.GetYaxis().SetLabelSize(0.13)
h_ratio.GetYaxis().CenterTitle()
h_ratio.GetYaxis().SetTickLength(0.02)
h_ratio.GetXaxis().SetLabelSize(0)
h_ratio.GetXaxis().SetTitle("")
h_ratio.GetXaxis().SetNdivisions(51,0,0)
h_ratio.GetXaxis().SetTickSize(0.06)
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerSize(1.0)
h_ratio.SetLineWidth(1)

h_ratio.Draw("PE")

line = ROOT.TLine()
line.DrawLine(0,1,51,1)

username = os.environ["USER"]
c.SaveAs("/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/MT2Looper_comp.pdf".format(username))
c.SaveAs("/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/MT2Looper_comp.png".format(username))
c.SaveAs("/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/MT2Looper_comp.root".format(username))


raw_input()
