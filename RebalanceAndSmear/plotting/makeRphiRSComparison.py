import ROOT as r
import glob
import os
import math

doRatioToTotal = False

def getRatioAndError (h1, h2):
  err1 = r.Double(0)
  val1 = h1.IntegralAndError(0,99,err1)
  err2 = r.Double(0)
  val2 = h2.IntegralAndError(0,99,err2)
  val = val1/val2
  err = val * math.sqrt((err1/val1)*(err1/val1) + (err2/val2)*(err2/val2))
  return {'val':val, 'err':err}
  
r.gROOT.SetBatch(1)

rphi_file = "/home/users/fgolf/mt2/devel/MT2Analysis/MT2looper/output/full2016/qcdFromCRs.root"
rs_file = "looper_output/v10/data/merged_hists.root"
data_file = "looper_output/v10/data_noRS/merged_hists_skims.root"

hrphi = r.TH1D("hrphi","",51,0,51)
hrs = r.TH1D("hrs","",51,0,51)
hdata = r.TH1D("hdata","",51,0,51)

h_evts_rphi = r.TH1D("h_evts_rphi","",1,0,2)
h_evts_rs = r.TH1D("h_evts_rs","",1,0,2)
h_evts_data = r.TH1D("h_evts_data","",1,0,2)

top_regs_vl=[1,2,3,12,13,14,15]

frphi = r.TFile(rphi_file)
frs = r.TFile(rs_file)
fdata = r.TFile(data_file)
ibin = 0
for ht_reg in ["VL","L","M","H","UH"]:
  sum_rphi = 0
  sum_rs = 0
  top_regs = []
  if ht_reg == "VL":
    top_regs = top_regs_vl
  else:
    top_regs.extend(range(1,12))
  for top_reg in top_regs:
    ibin+=1

    rphi_err = r.Double(0)
    if h_evts_rs:
      h_evts_rs.Reset()
    if h_evts_rphi:
      h_evts_rphi.Reset()
    if h_evts_data:
      h_evts_data.Reset()
    try:
      h_evts_rphi = frphi.Get("sr{0}{1}/h_mt2bins".format(top_reg,ht_reg))
    except:
      pass
    try:
      h_evts_rs = frs.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg))
    except:
      pass
    try:
      h_evts_data = fdata.Get("sr{0}{1}/h_Events_w".format(top_reg,ht_reg))
    except:
      pass      

    if doRatioToTotal:
      if h_evts_rphi and h_evts_data:
        vals = getRatioAndError(h_evts_rphi, h_evts_data)
        hrphi.SetBinContent(ibin, vals['val'])
        hrphi.SetBinError(ibin, vals['err'])
      else:
        hrphi.SetBinContent(ibin, 0)
        hrphi.SetBinError(ibin, 0)
      if h_evts_rs and h_evts_data:
        vals = getRatioAndError(h_evts_rs, h_evts_data)
        hrs.SetBinContent(ibin, vals['val'])
        hrs.SetBinError(ibin, vals['err'])
      else:
        hrs.SetBinContent(ibin, 0)
        hrs.SetBinError(ibin, 0)        
    else:
      if h_evts_rphi:
        hrphi.SetBinContent(ibin, h_evts_rphi.IntegralAndError(0,99,rphi_err))
        hrphi.SetBinError(ibin, rphi_err)
      else:
        hrphi.SetBinContent(ibin, 0)
        hrphi.SetBinError(ibin, 0)
            
      if h_evts_rs:
        hrs.SetBinContent(ibin, h_evts_rs.GetBinContent(1))
        hrs.SetBinError(ibin, h_evts_rs.GetBinError(1))
      else:
        hrs.SetBinContent(ibin, 0)
        hrs.SetBinError(ibin, 0)

    sum_rphi += hrphi.GetBinContent(ibin)            
    sum_rs += hrs.GetBinContent(ibin)

  ratio = sum_rs/sum_rphi if sum_rphi > 0 else 0
  print "{0} HT: rs = {1}, rphi = {2}, ratio = {3}".format(ht_reg, sum_rs, sum_rphi, ratio)
    
r.gStyle.SetOptStat(0)

c = r.TCanvas("c","c",900,600)

pads = []
pads.append(r.TPad("1","1",0.0,0.18,1.0,1.0))
pads.append(r.TPad("2","2",0.0,0.0,1.0,0.19))

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

hrs.SetLineColor(r.kBlack)
hrs.SetMarkerColor(r.kBlack)
hrs.SetMarkerStyle(20)

hrphi.SetLineColor(401)
hrphi.SetMarkerColor(401)
hrphi.SetMarkerStyle(20)

hrphi.GetYaxis().SetRangeUser(1e-3,1e4)
if doRatioToTotal:
  hrphi.GetYaxis().SetRangeUser(1e-3,5)
hrphi.GetXaxis().SetLabelSize(0)

hrphi.Draw("PE")
hrs.Draw("PE SAME")

line = r.TLine()
line.SetLineStyle(2)
for ix in [7,18,29,40]:
  x = pads[0].GetLeftMargin() + ix/51.0 * (1-pads[0].GetLeftMargin()-pads[0].GetRightMargin())
  line.DrawLineNDC(x,1-pads[0].GetTopMargin(),x,pads[0].GetBottomMargin())

leg = r.TLegend(0.815,0.78,0.94,0.9)
leg.AddEntry(hrs, "R&S estimate")
leg.AddEntry(hrphi, "rphi estimate")
leg.Draw()

text = r.TLatex()
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
text = r.TLatex()
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

h_ratio = hrs.Clone("h_ratio")
h_ratio.Divide(hrphi)

h_ratio.GetYaxis().SetRangeUser(0,5)
h_ratio.GetYaxis().SetNdivisions(505)
h_ratio.GetYaxis().SetTitle("R&S/rphi")
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

line = r.TLine()
line.DrawLine(0,1,51,1)

username = os.environ["USER"]
suffix = "_rat" if doRatioToTotal else ""
c.SaveAs("/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/comparison{1}.pdf".format(username,suffix))
c.SaveAs("/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/comparison{1}.png".format(username,suffix))
c.SaveAs("/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/comparison{1}.root".format(username,suffix))

raw_input()


