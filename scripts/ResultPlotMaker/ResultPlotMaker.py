import ROOT
import os
import ResultPlotUtils as utils
import sys
import ppmUtils
from array import *



ROOT.gROOT.SetBatch(1)

def MakePlot(ht_reg, datacard_dir, datacard_name, outdir, userMax=None):
    jbj_regs = utils.GetJBJregions(ht_reg)

    #list of lists, one per jbj region, of low edges of MT2 bins
    mt2bins = utils.GetMT2bins(ht_reg)

    # nBinsTotal = sum([len(bins)-1 for bins in mt2bins]) + 1
    nBinsTotal = sum([len(bins)-1 for bins in mt2bins])
    bkg_processes = ["zinv","llep","qcd"]
    nBkgs = len(bkg_processes)

    ## setup histograms
    h_data = ROOT.TH1D("h_data","",nBinsTotal,0,nBinsTotal)
    h_bkg_vec = []
    for i,proc in enumerate(bkg_processes):
        h_bkg_vec.append(ROOT.TH1D("h_"+proc,"",nBinsTotal,0,nBinsTotal))
    g_unc = ROOT.TGraphAsymmErrors() # graph to store prediction uncertainties
    g_unc_ratio = ROOT.TGraphAsymmErrors() # graph to store prediction uncertainties

    ## fill histograms
    ibin = 0
    binLabels = []
    
    for ijbj, jbj_reg in enumerate(jbj_regs):
        for imt2 in range(len(mt2bins[ijbj])-1):
            ibin += 1
            mt2left = mt2bins[ijbj][imt2]
            mt2right = mt2bins[ijbj][imt2+1]
            mt2name = utils.GetMT2name(mt2left,mt2right)
            if ht_reg != "monojet":
                datacard_name_fmt = datacard_name.format(ht_reg,jbj_reg,mt2name)
            else:
                ht_name = "HT{0}to{1}".format(mt2left,mt2right)
                ht_name = ht_name.replace("-1","Inf")
                datacard_name_fmt = datacard_name.format(ht_name,jbj_reg,"m0toInf")

            # get yields. first entry is data, rest are background predictions
            yields = utils.GetYieldsFromDatacard(os.path.join(datacard_dir,datacard_name_fmt),bkg_processes)
            h_data.SetBinContent(ibin, yields[0])
            for j in range(1,nBkgs+1):
                h_bkg_vec[j-1].SetBinContent(ibin, yields[j])
            tot_pred = sum(yields[1:])
                
            # get uncertainties
            pred_unc = utils.GetUncertaintiesFromDatacard(os.path.join(datacard_dir,datacard_name_fmt),bkg_processes)
            tot_unc_up   = ROOT.TMath.Sqrt(sum([(pred_unc[i][0]*yields[i+1])**2 for i in range(nBkgs)]))
            tot_unc_down = ROOT.TMath.Sqrt(sum([(pred_unc[i][1]*yields[i+1])**2 for i in range(nBkgs)]))
            thisPoint = g_unc.GetN()
            g_unc.SetPoint(thisPoint, ibin-0.5, tot_pred)
            g_unc.SetPointError(thisPoint, 0.5, 0.5, tot_unc_down, tot_unc_up)            
            g_unc_ratio.SetPoint(thisPoint, ibin-0.5, 1)
            g_unc_ratio.SetPointError(thisPoint, 0.5, 0.5, tot_unc_down/tot_pred, tot_unc_up/tot_pred)

            binLabels.append(utils.GetMT2label(mt2left,mt2right))

    h_bkg_vec[0].SetFillColor(418)
    h_bkg_vec[1].SetFillColor(ROOT.kAzure+4)
    h_bkg_vec[2].SetFillColor(401)
                
    stack = ROOT.THStack("bkg_stack","")
    for j in range(nBkgs):
        h_bkg_vec[nBkgs-1-j].SetLineWidth(1)
        h_bkg_vec[nBkgs-1-j].SetLineColor(ROOT.kBlack)
        stack.Add(h_bkg_vec[nBkgs-1-j])

    h_data.SetMarkerStyle(20)
    h_data.SetMarkerSize(1.3)
    h_data.SetMarkerColor(ROOT.kBlack)
    h_data.SetLineColor(ROOT.kBlack)

    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetLineWidth(1)
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
    # pads[0].SetTickx(1)        
    pads[1].SetTickx(1)
    pads[0].SetTicky(1)
    pads[1].SetTicky(1)
    
    yMin = 1e-3
    if userMax!=None:
        yMax = userMax
    else:
        yMax = h_data.GetMaximum() ** (2.0)
    h_data.GetYaxis().SetRangeUser(yMin,yMax)
    h_data.GetYaxis().SetTitle("Events / Bin")
    h_data.GetYaxis().SetTitleOffset(1.2)
    h_data.GetYaxis().SetTickLength(0.02)
    h_data.GetXaxis().SetRangeUser(0,nBinsTotal)
    h_data.GetXaxis().SetNdivisions(nBinsTotal,0,0)
    h_data.GetXaxis().SetLabelSize(0)
    h_data.GetXaxis().SetTickLength(0.015)

    # just draw dots to get axes. Will draw TGraphAsymmErrors on top later
    h_data.SetMarkerStyle(1)
    h_data.Draw("P")

    # draw the backgrounds
    stack.Draw("SAME HIST")
    
    # draw the prediction uncertainties
    g_unc.SetFillStyle(3244)
    g_unc.SetFillColor(ROOT.kGray+3)
    g_unc.Draw("SAME 2")

    # get a graph using proper asymmetric poissonian errors
    g_data = ROOT.TGraphAsymmErrors()
    ppmUtils.ConvertToPoissonGraph(h_data, g_data, drawZeros=True)
    g_data.SetPointError(g_data.GetN()-1, 0, 0, 0, 0)
    g_data.SetMarkerStyle(20)
    g_data.SetMarkerSize(1.2)
    g_data.SetLineWidth(1)
    
    # draw the graph and then axes again on top
    g_data.Draw("SAME P")      
    h_data.Draw("SAME AXIS")

    # save for later
    left = pads[0].GetLeftMargin()
    right = pads[0].GetRightMargin()
    top = pads[0].GetTopMargin()
    bot = pads[0].GetBottomMargin()

    #draw the x-axis labels
    binWidth = (1.0-right-left)/nBinsTotal
    text = ROOT.TLatex()
    text.SetNDC(1)
    text.SetTextAlign(32)
    text.SetTextAngle(90)
    text.SetTextSize(min(binWidth * 1.2,0.026))
    text.SetTextFont(62)
    for ibin in range(nBinsTotal-1):
        x = left + (ibin+0.5)*binWidth
        y = pads[0].GetBottomMargin()-0.009
        text.DrawLatex(x,y,binLabels[ibin])
        
    # draw the "Pre-fit background" text
    text.SetTextAlign(13)
    text.SetTextFont(42)
    text.SetTextAngle(0)
    text.SetTextSize(0.05)
    text.DrawLatex(left+0.04,1-top-0.01, "Pre-fit background")

    # draw the HT bin  in upper middle
    text.SetTextAlign(21)
    text.SetTextFont(62)
    text.SetTextAngle(0)
    text.SetTextSize(0.035)
    text.DrawLatex(left+(1-right-left)*0.5, 1-top-0.01-0.04, utils.GetHTtitle(ht_reg))

    # Draw the CMS and luminosity text
    ppmUtils.DrawCmsText(pads[0],text="CMS Preliminary",textSize=0.038)
    ppmUtils.DrawLumiText(pads[0],lumi=utils.lumi,textSize=0.038)

    # draw the j/bj region labels
    ibin = 0
    for ijbj,jbj_reg in enumerate(jbj_regs):
        xcenter = left + binWidth*(ibin+(len(mt2bins[ijbj])-1)*0.5)
        lines = utils.GetJBJtitle(jbj_reg)
        text.SetTextAlign(23)
        text.SetTextFont(62)
        text.SetTextSize(0.030)
        # in the last region, move the text left a bit to avoid overlap with tick marks
        if ijbj==len(jbj_regs)-1:
            text.SetTextAlign(13)
            xcenter = left + binWidth*ibin + 0.007
            xcenter = max(xcenter, 1-right-0.25)
        y = bot+(1-top-bot)*0.85
        if xcenter > 1-right-0.19:
            y = 0.67
        text.DrawLatex(xcenter,y,lines[0])
        text.DrawLatex(xcenter,y-text.GetTextSize()-0.001,lines[1])

        ibin += len(mt2bins[ijbj])-1

    #draw the lines separating j-bj region
    line = ROOT.TLine()
    line.SetNDC(1)
    line.SetLineStyle(2)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kBlack)
    ibin = 0
    for i in range(len(jbj_regs)-1):
        ibin += len(mt2bins[i])-1
        x = left+binWidth*ibin
        line.DrawLineNDC(x,bot,x,bot+(1-top-bot)*0.85)        
    
    # legend
    leg = ROOT.TLegend(1-right-0.175,1-top-0.23,1-right-0.02,1-top-0.01)
    leg.SetBorderSize(1)
    leg.SetCornerRadius(0.3)
    leg.AddEntry(g_data,"Data","lp")
    for i in range(nBkgs):
        leg.AddEntry(h_bkg_vec[i], utils.GetLegendName(bkg_processes[i]),'f')
    leg.Draw()


    ####################
    #### RATIO PLOT ####
    ####################
    
    pads[1].cd()
    h_ratio = h_bkg_vec[0].Clone("h_ratio") #h_ratio is just a dummy histogram to draw axes correctly
    h_ratio.Reset()
    g_ratio = ROOT.TGraphAsymmErrors()
    h_pred = h_bkg_vec[0].Clone("h_pred")
    for i in range(1,nBkgs):
        h_pred.Add(h_bkg_vec[i])
    ppmUtils.GetPoissonRatioGraph(h_pred, h_data, g_ratio, drawZeros=True, useMCErr=False)
    h_ratio.GetYaxis().SetRangeUser(0,2)
    h_ratio.GetYaxis().SetNdivisions(505)
    h_ratio.GetYaxis().SetTitle("Data/Pred.")
    h_ratio.GetYaxis().SetTitleSize(0.16)
    h_ratio.GetYaxis().SetTitleOffset(0.18)
    h_ratio.GetYaxis().SetLabelSize(0.13)
    h_ratio.GetYaxis().CenterTitle()
    h_ratio.GetYaxis().SetTickLength(0.02)
    h_ratio.GetXaxis().SetLabelSize(0)
    h_ratio.GetXaxis().SetTitle("")
    h_ratio.GetXaxis().SetNdivisions(nBinsTotal,0,0)
    h_ratio.GetXaxis().SetTickSize(0.06)
    g_ratio.SetMarkerStyle(20)
    g_ratio.SetMarkerSize(1.0)
    g_ratio.SetLineWidth(1)

    g_unc_ratio.SetFillStyle(1001)
    g_unc_ratio.SetFillColor(ROOT.kGray)
    
    h_ratio.Draw()
    g_unc_ratio.Draw("SAME 2")

    # draw line at 1
    line = ROOT.TLine()
    line.SetLineStyle(1)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kGray+2)
    line.DrawLine(0,1,nBinsTotal,1)

    #draw the lines separating j-bj region
    line.SetNDC(1)
    line.SetLineStyle(2)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kBlack)
    ibin = 0
    for i in range(len(jbj_regs)-1):
        ibin += len(mt2bins[i])-1
        line.DrawLine(ibin,0,ibin,2)        
    
    h_ratio.Draw("SAME AXIS")
    g_ratio.Draw("SAME P0")

    name = "prefit_{0}".format(ht_reg)
    try:
        os.makedirs(outdir)
    except:
        pass
    c.SaveAs(os.path.join(outdir,name+".pdf"))
    c.SaveAs(os.path.join(outdir,name+".png"))
    
    h_data.Delete()
    g_data.Delete()
    h_ratio.Delete()
    g_ratio.Delete()
    for h in h_bkg_vec:
        h.Delete()
    

def MakeMacroRegionPlot(macro_reg, datacard_dir, datacard_name, outdir, userMax=None):
    # based on MakePlot, but here we want to sum over several cards

    topo_regs = utils.GetMacroregions(macro_reg) #arbitrary set

    #list of lists, one per maco region, of low edges of MT2 bins
    mt2bins = []
    for topo_reg in topo_regs:
        bins = utils.GetMT2binsFull(topo_reg)
        mt2bins.append(bins)

    bkg_processes = ["zinv","llep","qcd"]
    nBkgs = len(bkg_processes)

    # MT2 binning we want to use: let's take the region with the highest number of bins
    xbins = mt2bins[0] #start with the first region
    for tempmt2bins in mt2bins :
        if (len(xbins) < len(tempmt2bins)) : xbins = tempmt2bins
    xbinsA = array('d', xbins)
    xbinsA[len(xbinsA)-1] = 1800 # set last bin
    nBinsTotal = len(xbinsA)-1

    ## setup histograms
    h_data = ROOT.TH1D("h_data","",len(xbinsA)-1,xbinsA)
    h_bkg_tot = ROOT.TH1D("h_tot","",len(xbinsA)-1,xbinsA)
    h_bkg_vec = []
    for i,proc in enumerate(bkg_processes):
        h_bkg_vec.append(ROOT.TH1D("h_"+proc,"",len(xbinsA)-1,xbinsA))
    h_unc = ROOT.TH1D("h_unc","",len(xbinsA)-1,xbinsA)  #  store prediction uncertainties
    h_unc_ratio = ROOT.TH1D("h_unc_ratio","",len(xbinsA)-1,xbinsA)  #  store prediction uncertainties

    ## fill histograms
    ibin = 0
    binLabels = []
    
#    for ijbj, jbj_reg in enumerate(jbj_regs):
    datacard_list = []
    for imt2 in range(len(xbinsA)-1):
        datacard_list.append([])

    for itopo, topo_reg in enumerate(topo_regs):

        ibin = 0
        for imt2 in range(len(mt2bins[itopo])-1):
            ibin += 1
            mt2left = mt2bins[itopo][imt2]
            mt2right = mt2bins[itopo][imt2+1]
            if mt2right == 1800 : mt2right = "Inf"
            mt2name = utils.GetMT2name(mt2left,mt2right)
            datacard_name_fmt = datacard_name.format(topo_reg,mt2name)
            datacard_list[imt2].append(os.path.join(datacard_dir,datacard_name_fmt))

            # get yields. first entry is data, rest are background predictions
            #print datacard_name_fmt
            yields = utils.GetYieldsFromDatacard(os.path.join(datacard_dir,datacard_name_fmt),bkg_processes)
            #print yields[0]
            h_data.SetBinContent(ibin, h_data.GetBinContent(ibin) + yields[0])
            for j in range(1,nBkgs+1):
                h_bkg_vec[j-1].SetBinContent(ibin, h_bkg_vec[j-1].GetBinContent(ibin) + yields[j])
            tot_pred = sum(yields[1:])
            h_bkg_tot.SetBinContent(ibin, h_bkg_tot.GetBinContent(ibin) + tot_pred)
                
            # get uncertainties
            pred_unc = utils.GetUncertaintiesFromDatacard(os.path.join(datacard_dir,datacard_name_fmt),bkg_processes)
            tot_unc_up   = ROOT.TMath.Sqrt(sum([(pred_unc[i][0]*yields[i+1])**2 for i in range(nBkgs)]))
            tot_unc_down = ROOT.TMath.Sqrt(sum([(pred_unc[i][1]*yields[i+1])**2 for i in range(nBkgs)]))
            tot_unc_sym = max(tot_unc_up, tot_unc_down)
#            h_unc.SetBinContent(ibin, h_unc.GetBinContent(ibin) + tot_unc_sym) # treat as fully correlated for now
            h_unc.SetBinContent(ibin, h_unc.GetBinContent(ibin) + tot_unc_sym*tot_unc_sym) # sum in quadrature
            #print tot_pred, tot_unc_up, tot_unc_down

    h_bkg_vec[0].SetFillColor(418)
    h_bkg_vec[1].SetFillColor(ROOT.kAzure+4)
    h_bkg_vec[2].SetFillColor(401)
    

    # Outdated: simple sum in quadrature
    #for imt2 in range(len(xbinsA)-1):
    #    h_unc_ratio.SetBinContent(imt2+1, h_data.GetBinContent(imt2+1)/h_bkg_tot.GetBinContent(imt2+1))
    #    h_unc_ratio.SetBinError(imt2+1, ROOT.TMath.Sqrt(h_unc.GetBinContent(imt2+1))/h_bkg_tot.GetBinContent(imt2+1))
    #    h_unc.SetBinError(imt2+1, ROOT.TMath.Sqrt(h_unc.GetBinContent(imt2+1)))
    #    h_unc.SetBinContent(imt2+1, h_bkg_tot.GetBinContent(imt2+1))

    for imt2 in range(len(xbinsA)-1):
        unc = utils.getMacroRegionUncertainties("test"+str(imt2), datacard_list[imt2])
        h_unc_ratio.SetBinContent(imt2+1, h_data.GetBinContent(imt2+1)/h_bkg_tot.GetBinContent(imt2+1))
        h_unc_ratio.SetBinError(imt2+1, unc/h_bkg_tot.GetBinContent(imt2+1))
        h_unc.SetBinError(imt2+1, unc)
        h_unc.SetBinContent(imt2+1, h_bkg_tot.GetBinContent(imt2+1))
                
    stack = ROOT.THStack("bkg_stack","")
    for j in range(nBkgs):
        h_bkg_vec[nBkgs-1-j].SetLineWidth(1)
        h_bkg_vec[nBkgs-1-j].SetLineColor(ROOT.kBlack)
        stack.Add(h_bkg_vec[nBkgs-1-j])
#    h_bkg_tot.Print("all")
#    h_unc.Print("all")
#    h_unc_ratio.Print("all")
    h_data.Print("all")

    h_data.SetMarkerStyle(20)
    h_data.SetMarkerSize(1.3)
    h_data.SetMarkerColor(ROOT.kBlack)
    h_data.SetLineColor(ROOT.kBlack)

    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetLineWidth(1)
    c = ROOT.TCanvas("c","c",600,600)

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
    # pads[0].SetTickx(1)        
    pads[1].SetTickx(1)
    pads[0].SetTicky(1)
    pads[1].SetTicky(1)
    
    yMin = 1e-1
    if userMax!=None:
        yMax = userMax
    else:
        yMax = h_data.GetMaximum() ** (2.0)
    h_data.GetYaxis().SetRangeUser(yMin,yMax)
    h_data.GetYaxis().SetTitle("Events / Bin")
    h_data.GetYaxis().SetTitleOffset(1.2)
    h_data.GetYaxis().SetTickLength(0.02)
    #h_data.GetXaxis().SetLabelSize(0)
    h_data.GetXaxis().SetTickLength(0.015)
    h_data.GetXaxis().SetTitle("MT2 [GeV]")
    h_data.GetXaxis().SetTitleSize(0.04)
    h_data.GetXaxis().SetLabelSize(0.036)
    h_data.GetXaxis().SetTitleOffset(1.2)

    # just draw dots to get axes. Will draw TGraphAsymmErrors on top later
    h_data.SetMarkerStyle(1)
    h_data.Draw("P")
    

    # draw the backgrounds
    stack.Draw("SAME HIST")
    
    # draw the prediction uncertainties
    h_unc.SetFillStyle(3244)
    h_unc.SetFillColor(ROOT.kGray+3)
    h_unc.Draw("SAME E2")

    # get a graph using proper asymmetric poissonian errors
    g_data = ROOT.TGraphAsymmErrors()
    ppmUtils.ConvertToPoissonGraph(h_data, g_data, drawZeros=True)
#    g_data.SetPointError(g_data.GetN()-1, 0, 0, 0, 0)
    g_data.SetMarkerStyle(20)
    g_data.SetMarkerSize(1.2)
    g_data.SetLineWidth(1)
    
    # draw the graph and then axes again on top
    g_data.Draw("SAME P")      
    h_data.Draw("SAME AXIS")

    # save for later
    left = pads[0].GetLeftMargin()
    right = pads[0].GetRightMargin()
    top = pads[0].GetTopMargin()
    bot = pads[0].GetBottomMargin()

    text = ROOT.TLatex()
    text.SetNDC(1)
       
    # draw the "Pre-fit background" text
    text.SetTextAlign(13)
    text.SetTextFont(42)
    text.SetTextAngle(0)
    text.SetTextSize(0.05)
    text.DrawLatex(left+0.04,1-top-0.01, "Pre-fit background")

    # draw the HT bin  in upper middle
    text.SetTextAlign(21)
    text.SetTextFont(62)
    text.SetTextAngle(0)
    text.SetTextSize(0.035)
#    text.DrawLatex(left+(1-right-left)*0.5, 1-top-0.01-0.04, utils.GetHTtitle(ht_reg))
    text.DrawLatex(left+(1-right-left)*0.3, 1-top-0.01-0.09, utils.GetMacroregionTitle(macro_reg))


    # Draw the CMS and luminosity text
    ppmUtils.DrawCmsText(pads[0],text="CMS Preliminary",textSize=0.038)
    ppmUtils.DrawLumiText(pads[0],lumi=utils.lumi,textSize=0.038)

    
    # legend
    leg = ROOT.TLegend(1-right-0.175,1-top-0.23,1-right-0.02,1-top-0.01)
    leg.SetBorderSize(1)
    leg.SetCornerRadius(0.3)
    leg.AddEntry(g_data,"Data","lp")
    for i in range(nBkgs):
        leg.AddEntry(h_bkg_vec[i], utils.GetLegendName(bkg_processes[i]),'f')
    leg.Draw()


    ####################
    #### RATIO PLOT ####
    ####################
    
    pads[1].cd()
    h_ratio = h_bkg_vec[0].Clone("h_ratio") #h_ratio is just a dummy histogram to draw axes correctly
    h_ratio.Reset()
    g_ratio = ROOT.TGraphAsymmErrors()
    h_pred = h_bkg_tot.Clone("h_pred")
    ppmUtils.GetPoissonRatioGraph(h_pred, h_data, g_ratio, drawZeros=True, useMCErr=False)
    h_ratio.GetYaxis().SetRangeUser(0,2)
    h_ratio.GetYaxis().SetNdivisions(505)
    h_ratio.GetYaxis().SetTitle("Data/Pred.")
    h_ratio.GetYaxis().SetTitleSize(0.16)
    h_ratio.GetYaxis().SetTitleOffset(0.25)
    h_ratio.GetYaxis().SetLabelSize(0.13)
    h_ratio.GetYaxis().CenterTitle()
    h_ratio.GetYaxis().SetTickLength(0.02)
    h_ratio.GetXaxis().SetLabelSize(0)
    h_ratio.GetXaxis().SetTitle("")
    #h_ratio.GetXaxis().SetNdivisions(nBinsTotal,0,0)
    h_ratio.GetXaxis().SetTickSize(0.06)
    g_ratio.SetMarkerStyle(20)
    g_ratio.SetMarkerSize(1.0)
    g_ratio.SetLineWidth(1)

    h_unc_ratio.SetFillStyle(1001)
    h_unc_ratio.SetFillColor(ROOT.kGray)
    h_ratio.Draw()
    h_unc_ratio.Draw("SAME E2")

    # draw line at 1
    line = ROOT.TLine()
    line.SetLineStyle(1)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kGray+2)
    line.DrawLine(xbinsA[0],1,xbinsA[len(xbinsA)-1],1)

    
    h_ratio.Draw("SAME AXIS")
    g_ratio.Draw("SAME P0")


    name = "MacroRegion_{0}".format(macro_reg)
    try:
        os.makedirs(outdir)
    except:
        pass
    c.SaveAs(os.path.join(outdir,name+".pdf"))
    c.SaveAs(os.path.join(outdir,name+".png"))
    
    h_data.Delete()
    g_data.Delete()
    h_ratio.Delete()
    g_ratio.Delete()
    for h in h_bkg_vec:
        h.Delete()
    




def MakeComparison(ht_reg, datacard_dir1, datacard_dir2, datacard_name1, datacard_name2, outdir, iBkgd=1, userMax=None):
    jbj_regs = utils.GetJBJregions(ht_reg)

    #list of lists, one per jbj region, of low edges of MT2 bins
    mt2bins = utils.GetMT2bins(ht_reg)

    nBinsTotal = sum([len(bins)-1 for bins in mt2bins])
    bkg_processes = ["zinv","llep","qcd"]
    nBkgs = len(bkg_processes)

    ## setup histograms
    h_snt = ROOT.TH1D("h_snt","",nBinsTotal,0,nBinsTotal)
    h_eth = ROOT.TH1D("h_eth","",nBinsTotal,0,nBinsTotal)

    ## fill histograms
    ibin = 0
    binLabels = []
    
    for ijbj, jbj_reg in enumerate(jbj_regs):
        for imt2 in range(len(mt2bins[ijbj])-1):
            ibin += 1
            mt2left = mt2bins[ijbj][imt2]
            mt2right = mt2bins[ijbj][imt2+1]
            mt2name = utils.GetMT2name(mt2left,mt2right)
            if ht_reg != "monojet":
                datacard_name_fmt1 = datacard_name1.format(ht_reg,jbj_reg,mt2name)
                datacard_name_fmt2 = datacard_name2.format(ht_reg,jbj_reg,mt2name)
            else:
                ht_name = "HT{0}to{1}".format(mt2left,mt2right)
                ht_name = ht_name.replace("-1","Inf")
                datacard_name_fmt1 = datacard_name1.format(ht_name,jbj_reg,"m0toInf")
                datacard_name_fmt2 = datacard_name2.format(ht_name,jbj_reg,"m0toInf")

            # get yields. first entry is data, rest are background predictions
            yields1 = utils.GetYieldsFromDatacard(os.path.join(datacard_dir1,datacard_name_fmt1),bkg_processes)
            yields2 = utils.GetYieldsFromDatacard(os.path.join(datacard_dir2,datacard_name_fmt2),bkg_processes)
            h_snt.SetBinContent(ibin, yields1[iBkgd])
            h_eth.SetBinContent(ibin, yields2[iBkgd])
                
            # get uncertainties
            pred_unc1 = utils.GetUncertaintiesFromDatacard(os.path.join(datacard_dir1,datacard_name_fmt1),bkg_processes)
            pred_unc2 = utils.GetUncertaintiesFromDatacard(os.path.join(datacard_dir2,datacard_name_fmt2),bkg_processes)
            h_snt.SetBinError(ibin, pred_unc1[iBkgd-1][0]*yields1[iBkgd])
            h_eth.SetBinError(ibin, pred_unc2[iBkgd-1][0]*yields2[iBkgd])

            binLabels.append(utils.GetMT2label(mt2left,mt2right))


    h_snt.SetMarkerStyle(20)
    h_snt.SetMarkerSize(0.5)
    h_snt.SetMarkerColor(ROOT.kBlack)
    h_snt.SetLineColor(ROOT.kBlack)

    h_eth.SetMarkerStyle(20)
    h_eth.SetMarkerSize(0.5)
    h_eth.SetMarkerColor(ROOT.kRed)
    h_eth.SetLineColor(ROOT.kRed)

    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetLineWidth(1)
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
    # pads[0].SetTickx(1)        
    pads[1].SetTickx(1)
    pads[0].SetTicky(1)
    pads[1].SetTicky(1)
    
    yMin = 1e-1
    if userMax!=None:
        yMax = userMax
    else:
        yMax = h_snt.GetMaximum() ** (2.0)
    h_snt.GetYaxis().SetRangeUser(yMin,yMax)
    h_snt.GetYaxis().SetTitle("Events / Bin")
    h_snt.GetYaxis().SetTitleOffset(1.2)
    h_snt.GetYaxis().SetTickLength(0.02)
    h_snt.GetXaxis().SetRangeUser(0,nBinsTotal)
    h_snt.GetXaxis().SetNdivisions(nBinsTotal,0,0)
    h_snt.GetXaxis().SetLabelSize(0)
    h_snt.GetXaxis().SetTickLength(0.015)

    h_snt.Draw("PE")
    h_eth.Draw("SAME PE")

    # save for later
    left = pads[0].GetLeftMargin()
    right = pads[0].GetRightMargin()
    top = pads[0].GetTopMargin()
    bot = pads[0].GetBottomMargin()

    #draw the x-axis labels
    binWidth = (1.0-right-left)/nBinsTotal
    text = ROOT.TLatex()
    text.SetNDC(1)
    text.SetTextAlign(32)
    text.SetTextAngle(90)
    text.SetTextSize(min(binWidth * 1.2,0.026))
    text.SetTextFont(62)
    for ibin in range(nBinsTotal):
        x = left + (ibin+0.5)*binWidth
        y = pads[0].GetBottomMargin()-0.009
        text.DrawLatex(x,y,binLabels[ibin])
        
    # draw the "Pre-fit background" text
    text.SetTextAlign(13)
    text.SetTextFont(42)
    text.SetTextAngle(0)
    text.SetTextSize(0.04)
    text.DrawLatex(left+0.02,1-top-0.01, "ETH/SNT comparison: {0}".format(bkg_processes[iBkgd-1]))

    # draw the HT bin  in upper middle
    text.SetTextAlign(21)
    text.SetTextFont(62)
    text.SetTextAngle(0)
    text.SetTextSize(0.035)
    text.DrawLatex(left+(1-right-left)*0.5, 1-top-0.01-0.04, utils.GetHTtitle(ht_reg))

    # Draw the CMS and luminosity text
    ppmUtils.DrawCmsText(pads[0],text="CMS Preliminary",textSize=0.038)
    ppmUtils.DrawLumiText(pads[0],lumi=utils.lumi,textSize=0.038)

    # draw the j/bj region labels
    ibin = 0
    for ijbj,jbj_reg in enumerate(jbj_regs):
        xcenter = left + binWidth*(ibin+(len(mt2bins[ijbj])-1)*0.5)
        lines = utils.GetJBJtitle(jbj_reg)
        text.SetTextAlign(23)
        text.SetTextFont(62)
        text.SetTextSize(0.030)
        # in the last region, move the text left a bit to avoid overlap with tick marks
        if ijbj==len(jbj_regs)-1:
            text.SetTextAlign(13)
            xcenter = left + binWidth*ibin + 0.007
            xcenter = max(xcenter, 1-right-0.25)
        y = bot+(1-top-bot)*0.85
        if xcenter > 1-right-0.19:
            y = 0.67
        text.DrawLatex(xcenter,y,lines[0])
        text.DrawLatex(xcenter,y-text.GetTextSize()-0.001,lines[1])

        ibin += len(mt2bins[ijbj])-1

    #draw the lines separating j-bj region
    line = ROOT.TLine()
    line.SetNDC(1)
    line.SetLineStyle(2)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kBlack)
    ibin = 0
    for i in range(len(jbj_regs)-1):
        ibin += len(mt2bins[i])-1
        x = left+binWidth*ibin
        line.DrawLineNDC(x,bot,x,bot+(1-top-bot)*0.85)        
    
    # legend
    leg = ROOT.TLegend(1-right-0.175,1-top-0.21,1-right-0.02,1-top-0.04)
    leg.SetBorderSize(1)
    leg.AddEntry(h_snt,"SNT")
    leg.AddEntry(h_eth,"ETH")
    leg.Draw()


    ####################
    #### RATIO PLOT ####
    ####################
    
    pads[1].cd()
    h_ratio = h_snt.Clone("h_ratio") #h_ratio is just a dummy histogram to draw axes correctly
    h_ratio.Divide(h_eth)
    for ibin in range(1,h_ratio.GetNbinsX()+1):
        h_ratio.SetBinError(ibin,0.0001)
    h_ratio.GetYaxis().SetRangeUser(0.5,1.5)
    h_ratio.GetYaxis().SetNdivisions(502)
    h_ratio.GetYaxis().SetTitle("SNT/ETH")
    h_ratio.GetYaxis().SetTitleSize(0.16)
    h_ratio.GetYaxis().SetTitleOffset(0.18)
    h_ratio.GetYaxis().SetLabelSize(0.13)
    h_ratio.GetYaxis().CenterTitle()
    h_ratio.GetYaxis().SetTickLength(0.02)
    h_ratio.GetXaxis().SetLabelSize(0)
    h_ratio.GetXaxis().SetTitle("")
    h_ratio.GetXaxis().SetNdivisions(nBinsTotal,0,0)
    h_ratio.GetXaxis().SetTickSize(0.06)
    
    h_ratio.Draw("PE")

    # draw line at 1
    line = ROOT.TLine()
    line.SetLineStyle(1)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kGray+2)
    line.DrawLine(0,1,nBinsTotal,1)

    #draw the lines separating j-bj region
    line.SetNDC(1)
    line.SetLineStyle(2)
    line.SetLineWidth(1)
    line.SetLineColor(ROOT.kBlack)
    ibin = 0
    for i in range(len(jbj_regs)-1):
        ibin += len(mt2bins[i])-1
        line.DrawLine(ibin,0,ibin,2)        
    
    h_ratio.Draw("SAME PE")
    h_ratio.Draw("SAME AXIS")

    name = "eth_snt_compare_{0}_{1}".format(bkg_processes[iBkgd-1], ht_reg)
    try:
        os.makedirs(outdir)
    except:
        pass
    c.SaveAs(os.path.join(outdir,name+".pdf"))
    c.SaveAs(os.path.join(outdir,name+".png"))
    
    h_snt.Delete()
    h_eth.Delete()
    h_ratio.Delete()

    
