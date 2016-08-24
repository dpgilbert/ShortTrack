import ROOT

justplot = True

if not justplot:

    c = ROOT.TChain("mt2")
    c.Add("/nfs-6/userdata/mt2/V00-08-07_nojson/data_Run2016B_JetHT*.root")
    c.Add("/nfs-6/userdata/mt2/V00-08-07_nojson/data_Run2016C_JetHT*.root")
    c.Add("/nfs-6/userdata/mt2/V00-08-07_nojson/data_Run2016D_JetHT*.root")
    # c.Add("/nfs-6/userdata/mt2/rphi_skims/data_for_prescale_meas.root")

    h_PFHT800 = ROOT.TH1D("h_PFHT800","PFHT800",80,0,2000)
    h_PFHT475 = ROOT.TH1D("h_PFHT475","PFHT475",80,0,2000)
    h_PFHT350 = ROOT.TH1D("h_PFHT350","PFHT350",80,0,2000)
    h_PFHT125 = ROOT.TH1D("h_PFHT125","PFHT125",80,0,2000)
    
    print "Getting PFHT800 histogram..."
    c.Draw("ht>>h_PFHT800","HLT_PFHT800!=0","goff")
    print "Getting PFHT475 histogram..."
    c.Draw("ht>>h_PFHT475","HLT_PFHT475_Prescale!=0","goff")
    print "Getting PFHT350 histogram..."
    c.Draw("ht>>h_PFHT350","HLT_PFHT350_Prescale!=0","goff")
    print "Getting PFHT125 histogram..."
    c.Draw("ht>>h_PFHT125","HLT_PFHT125_Prescale!=0","goff")

    h_PFHT800over475 = h_PFHT800.Clone("h_PFHT800over475")
    h_PFHT800over475.Sumw2()
    h_PFHT800over475.Divide(h_PFHT475)
    h_PFHT800over350 = h_PFHT800.Clone("h_PFHT800over350")
    h_PFHT800over350.Sumw2()
    h_PFHT800over350.Divide(h_PFHT350)
    h_PFHT800over125 = h_PFHT800.Clone("h_PFHT800over125")
    h_PFHT800over125.Sumw2()
    h_PFHT800over125.Divide(h_PFHT125)
    h_PFHT475over350 = h_PFHT475.Clone("h_PFHT475over350")
    h_PFHT475over350.Sumw2()
    h_PFHT475over350.Divide(h_PFHT350)
    h_PFHT475over125 = h_PFHT475.Clone("h_PFHT475over125")
    h_PFHT475over125.Sumw2()
    h_PFHT475over125.Divide(h_PFHT125)

    fout = ROOT.TFile("prescales.root","RECREATE")
    h_PFHT800.Write()
    h_PFHT475.Write()
    h_PFHT350.Write()
    h_PFHT125.Write()
    h_PFHT800over475.Write()
    h_PFHT800over350.Write()
    h_PFHT800over125.Write()
    h_PFHT475over350.Write()
    h_PFHT475over125.Write()
    fout.Close()

fin = ROOT.TFile("prescales.root")

fit_800_475 = ROOT.TF1("fit_800_475","[0]",900,2000)
h_PFHT800over475 = fin.Get("h_PFHT800over475")
h_PFHT800over475.Fit("fit_800_475","QN","goff",900,2000)

fit_800_350 = ROOT.TF1("fit_800_350","[0]",900,2000)
h_PFHT800over350 = fin.Get("h_PFHT800over350")
h_PFHT800over350.Fit("fit_800_350","QN","goff",900,2000)

fit_800_125 = ROOT.TF1("fit_800_125","[0]",900,2000)
h_PFHT800over125 = fin.Get("h_PFHT800over125")
h_PFHT800over125.Fit("fit_800_125","QN","goff",900,2000)

fit_475_350 = ROOT.TF1("fit_475_350","[0]",575,2000)
h_PFHT475over350 = fin.Get("h_PFHT475over350")
h_PFHT475over350.Fit("fit_475_350","QN","goff",575,2000)

fit_475_125 = ROOT.TF1("fit_475_125","[0]",575,2000)
h_PFHT475over125 = fin.Get("h_PFHT475over125")
h_PFHT475over125.Fit("fit_475_125","QN","goff",575,2000)

ROOT.gStyle.SetOptStat(0)
c = ROOT.TCanvas("c1","c1",650,650)
c.SetLogy(1)
h_PFHT800over475.SetLineColor(ROOT.kBlack)
h_PFHT800over350.SetLineColor(ROOT.kRed)
h_PFHT800over125.SetLineColor(ROOT.kBlue)
h_PFHT475over350.SetLineColor(ROOT.kGreen)
h_PFHT475over125.SetLineColor(ROOT.kViolet-1)
h_PFHT800over475.SetLineWidth(2)
h_PFHT800over350.SetLineWidth(2)
h_PFHT800over125.SetLineWidth(2)
h_PFHT475over350.SetLineWidth(2)
h_PFHT475over125.SetLineWidth(2)

h_PFHT800over125.GetXaxis().SetRangeUser(300,2000)

fit_800_475.SetLineColor(h_PFHT800over475.GetLineColor())
fit_800_350.SetLineColor(h_PFHT800over350.GetLineColor())
fit_800_125.SetLineColor(h_PFHT800over125.GetLineColor())
fit_475_350.SetLineColor(h_PFHT475over350.GetLineColor())
fit_475_125.SetLineColor(h_PFHT475over125.GetLineColor())
fit_800_475.SetLineWidth(2)
fit_800_350.SetLineWidth(2)
fit_800_125.SetLineWidth(2)
fit_475_350.SetLineWidth(2)
fit_475_125.SetLineWidth(2)

h_PFHT800over125.Draw("E0")
h_PFHT800over350.Draw("E0 SAME")
h_PFHT800over475.Draw("E0 SAME")
h_PFHT475over125.Draw("E0 SAME")
h_PFHT475over350.Draw("E0 SAME")

fit_800_475.Draw("SAME")
fit_800_350.Draw("SAME")
fit_800_125.Draw("SAME")
fit_475_350.Draw("SAME")
fit_475_125.Draw("SAME")

leg = ROOT.TLegend(0.3,0.12,0.8,0.35)
leg.SetTextFont(62)
leg.AddEntry(fit_800_475,"PFHT800/475: {0:.1f} #pm {1:.1f}".format(fit_800_475.GetParameter(0),fit_800_475.GetParError(0)),'l')
leg.AddEntry(fit_800_350,"PFHT800/350: {0:.1f} #pm {1:.1f}".format(fit_800_350.GetParameter(0),fit_800_350.GetParError(0)),'l')
leg.AddEntry(fit_800_125,"PFHT800/125: {0:.0f} #pm {1:.0f}".format(fit_800_125.GetParameter(0),fit_800_125.GetParError(0)),'l')
leg.AddEntry(fit_475_350,"PFHT475/350: {0:.2f} #pm {1:.2f}".format(fit_475_350.GetParameter(0),fit_475_350.GetParError(0)),'l')
leg.AddEntry(fit_475_125,"PFHT475/125: {0:.1f} #pm {1:.1f}".format(fit_475_125.GetParameter(0),fit_475_125.GetParError(0)),'l')
leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.Draw()

raw_input()

fin.Close()
