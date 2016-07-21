import ROOT
import os
import ResultPlotUtils as utils
import ppmUtils

ROOT.gROOT.SetBatch(1)

def MakePlot(ht_reg, datacard_dir, datacard_name, outdir, userMax=None):
    jbj_regs = utils.GetJBJregions(ht_reg)
    
    #list of lists, one per jbj region, of low edges of MT2 bins
    mt2bins = utils.GetMT2bins(ht_reg)

    nBinsTotal = sum([len(bins)-1 for bins in mt2bins])
    bkg_processes = ["zinv","llep","qcd"]
    nBkgs = len(bkg_processes)

    ## setup histograms
    h_data = ROOT.TH1D("h_data","",nBinsTotal,0,nBinsTotal)
    h_bkg_vec = []
    for i,proc in enumerate(bkg_processes):
        h_bkg_vec.append(ROOT.TH1D("h_"+proc,"",nBinsTotal,0,nBinsTotal))

    ## fill histograms
    ibin = 0
    binLabels = []
    for ijbj, jbj_reg in enumerate(jbj_regs):
        for imt2 in range(len(mt2bins[ijbj])-1):
            ibin += 1
            mt2left = mt2bins[ijbj][imt2]
            mt2right = mt2bins[ijbj][imt2+1]
            mt2name = utils.GetMT2name(mt2left,mt2right)
            datacard_name2 = datacard_name.format(ht_reg,jbj_reg,mt2name)
            yields = utils.GetYieldsFromDatacard(os.path.join(datacard_dir,datacard_name2),bkg_processes)

            h_data.SetBinContent(ibin, yields[0])
            for j in range(1,nBkgs+1):
                h_bkg_vec[j-1].SetBinContent(ibin, yields[j])

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
    
    # get a graph using proper asymmetric poissonian errors
    g_data = ROOT.TGraphAsymmErrors()
    ppmUtils.ConvertToPoissonGraph(h_data, g_data, drawZeros=True)
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
    for ibin in range(nBinsTotal):
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
    ppmUtils.DrawLumiText(pads[0],lumi=7.7,textSize=0.038)

    # draw the j/bj region labels
    ibin = 0
    for ijbj,jbj_reg in enumerate(jbj_regs):
        xcenter = left + binWidth*(ibin+(len(mt2bins[ijbj])-1)*0.5)
        y = bot+(1-top-bot)*0.85
        if xcenter > 1-right-0.19:
            y = 0.67
        lines = utils.GetJBJtitle(jbj_reg)
        text.SetTextAlign(23)
        text.SetTextFont(62)
        text.SetTextSize(0.030)
        # in the last region, move the text left a bit to avoid overlap with tick marks
        if ijbj==len(jbj_regs)-1:
            text.SetTextAlign(13)
            xcenter = left + binWidth*ibin + 0.007
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
    ppmUtils.GetPoissonRatioGraph(h_pred, h_data, g_ratio, drawZeros=False, useMCErr=False)
    h_ratio.GetYaxis().SetRangeUser(0,2)
    h_ratio.GetYaxis().SetNdivisions(505)
    h_ratio.GetYaxis().SetTitle("Data/Pred.")
    h_ratio.GetYaxis().SetTitleSize(0.16)
    h_ratio.GetYaxis().SetTitleOffset(0.18)
    h_ratio.GetYaxis().SetLabelSize(0.13)
    h_ratio.GetYaxis().CenterTitle()
    h_ratio.GetXaxis().SetLabelSize(0)
    h_ratio.GetXaxis().SetTitle("")
    h_ratio.GetXaxis().SetNdivisions(nBinsTotal,0,0)
    h_ratio.GetXaxis().SetTickSize(0.06)
    g_ratio.SetMarkerStyle(20)
    g_ratio.SetMarkerSize(1.0)
    g_ratio.SetLineWidth(1)

    h_ratio.Draw()
    g_ratio.Draw("SAME P")

    # draw line at 1
    line = ROOT.TLine()
    line.SetLineStyle(2)
    line.SetLineWidth(2)
    line.SetLineColor(ROOT.kGray+2)
    line.DrawLine(0,1,nBinsTotal,1)
    
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
    
