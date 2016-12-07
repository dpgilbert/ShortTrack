import ROOT
import glob

ROOT.gROOT.SetBatch(1)

indir = "looper_output/RebalanceAndSmear_V00-08-12_fixedBtag"

h_ewk = ROOT.TH1D("h_ewk","",50,0,500)
h_qcd = ROOT.TH1D("h_qcd","",50,0,500)

for ht_reg in ["L","M","H","UH"]:
    h_ewk.Reset()
    h_qcd.Reset()
    
    for f in glob.glob(indir + "/*.root"):
        if f.find("qcd") > -1:
            h = h_qcd
            if f.find("ext1") > -1:
                weight = 0.686
            else:
                weight = 1 - 0.686
        else:
            h = h_ewk
            weight = 1.0

        fid = ROOT.TFile(f)

        for top_reg in range(1,12):
            try:
                h.Add(fid.Get("sr{0}{1}/h_reb_met".format(top_reg,ht_reg)), weight)
            except:
                pass

        fid.Close()

    c = ROOT.TCanvas()
    c.SetLogy(1)

    h_qcd.SetLineColor(ROOT.kBlack)
    h_qcd.SetFillColor(ROOT.kBlue)
    h_ewk.SetLineColor(ROOT.kBlack)
    h_ewk.SetFillColor(ROOT.kRed)

    stack = ROOT.THStack()

    stack.Add(h_ewk)
    stack.Add(h_qcd)

    stack.SetMinimum(1e-3)
    stack.SetMaximum(1e2)

    stack.Draw("HIST")

    stack.GetXaxis().SetTitle("Rebalanced MET (GeV)")

    leg = ROOT.TLegend(0.68,0.80,0.88,0.88)
    leg.AddEntry(h_qcd, "QCD", "f")
    leg.AddEntry(h_ewk, "EWK", "f")
    leg.Draw()
    
    qcd_yield = h_qcd.Integral(0,-1)
    under100 = h_qcd.Integral(0,10) + h_ewk.Integral(0,10)
    factor = qcd_yield/under100

    text = ROOT.TLatex()
    text.SetNDC(1)
    text.SetTextFont(42)
    text.SetTextSize(0.03)
    text.DrawLatex(0.15,0.87, "QCD total yield = {0:.2f}".format(qcd_yield))
    text.DrawLatex(0.15,0.835, "(QCD + EWK) w/ reb_met<100 GeV = {0:.2f}".format(under100))
    text.DrawLatex(0.15,0.80, "Correction Factor = {0:.2f}".format(factor))

    c.SaveAs("/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/MCtests/ewk/ewk_{0}.pdf".format(ht_reg))
    c.SaveAs("/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/MCtests/ewk/ewk_{0}.png".format(ht_reg))
        

