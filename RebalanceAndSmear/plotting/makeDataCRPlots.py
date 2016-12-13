import ROOT
import os
import pyRootPlotMaker as ppm
import numpy as np
import random

def GetTotalHistogram(h, fid, h_name, cr, ht_regs, isDataRS=False):
    # h is an already made TH1D with the correct binning
    h.Reset()
    for ht_reg in ht_regs:
        if isDataRS:
            if ht_reg=="L":  scale = 1.0/4
            if ht_reg=="M":  scale = 1.0/4
            if ht_reg=="H":  scale = 1.0/2
            if ht_reg=="UH": scale = 1.0/2
        else:
            scale = 1.0
        for top_reg in range(1,12):
            # print cr, top_reg, ht_reg, h_name
            try:
                h.Add(fid.Get("{0}{1}{2}/h_{3}".format(cr, top_reg, ht_reg, h_name)), scale)
            except:
                pass

ROOT.gROOT.SetBatch(1)

indir_MC = "looper_output/RebalanceAndSmear_V00-08-12_noRS"
indir_RS = "looper_output/RebalanceAndSmear_V00-08-12_data"
# indir_RS = "looper_output/RebalanceAndSmear_V00-08-12_fixedBtag_v2"
indir_data = "looper_output/RebalanceAndSmear_V00-08-12_data_noRS"

outdir = "/home/users/bemarsh/public_html/mt2/RebalanceAndSmear/data_tests/ICHEP"
RSfromMC = False

lumi = 12.9

data_name = "data_Run2016"
samples = ["zinv_ht","top","wjets_ht"]

data_file = ROOT.TFile(os.path.join(indir_data,data_name+".root"))
mc_files = [ROOT.TFile(os.path.join(indir_MC,s+"_nonext.root")) for s in samples]
if RSfromMC:
    rs_file = [ROOT.TFile(os.path.join(indir_RS,"qcd_ht_nonext.root")),
               ROOT.TFile(os.path.join(indir_RS,"qcd_ht_ext.root"))]
else:
    rs_file = ROOT.TFile(os.path.join(indir_RS,data_name+".root"))
    
vars = ["ht", "met", "mt2", "nJet30", "nBJet20"]
rebin = [2, 2, 2, 1, 1]

xRangeUser = [(1000,3000), (0,1000), (200,500), None, None]
units = ["GeV","GeV","GeV",None,None]
titles = ["H_{T}","MET","M_{T2}","N-jet","N-bjet"]

# dirs = ["crRSInvertDPhiInclusiveHT1000toInf", "crRSMT2SideBandInclusiveHT1000toInf", "crRSInvertDPhiInclusiveHT450to1000", "crRSMT2SideBandInclusiveHT450to1000"]
dirs = [("crRSInvertDPhi", ["L","M"]), ("crRSInvertDPhi", ["H","UH"]), ("crRSMT2SideBand", ["L","M"]), ("crRSMT2SideBand", ["H","UH"])]
# dirs = [("crRSInvertDPhi", ["H","UH"])]
for cr, ht_regs in dirs:
    if ht_regs==["L","M"]:
        ht_name="InclusiveHT450to1000"
    if ht_regs==["H","UH"]:
        ht_name="InclusiveHT1000toInf"
    try:
        os.makedirs(os.path.join(outdir,cr+ht_name))
        os.system("cp ~/scripts/index.php "+os.path.join(outdir,cr+ht_name))
    except:
        pass
    if "InvertDPhi" in cr:
        xRangeUser[2] = (200,500)
    if "MT2SideBand" in cr:
        xRangeUser[2] = (100,200)
    if "HT450to1000" in ht_name:
        xRangeUser[0] = (450,1000)
    if "HT1000toInf" in ht_name:
        xRangeUser[0] = (1000,3000)
    

    for ivar,var in enumerate(vars):
        h_data = data_file.Get("srbase/h_{0}".format(var)).Clone()
        h_data.Reset()
        GetTotalHistogram(h_data, data_file, var, cr, ht_regs, False)
        h_mc_vec = [f.Get("srbase/h_{0}".format(var)).Clone() for f in mc_files]
        for h,f in zip(h_mc_vec, mc_files):
            GetTotalHistogram(h, f, var, cr, ht_regs, False)
            h.Scale(lumi)
            h.Rebin(rebin[ivar])
        if RSfromMC:
            h_RS_nonext = rs_file[0].Get("srbase/h_{0}".format(var)).Clone()
            h_RS_ext = rs_file[0].Get("srbase/h_{0}".format(var)).Clone()
            GetTotalHistogram(h_RS_nonext, rs_file[0], var, cr, ht_regs, False)
            GetTotalHistogram(h_RS, rs_file[0], var, cr, ht_regs, False)
            h_RS = h_RS_nonext
            h_RS.Scale(1-0.686)
            h_RS.Add(h_RS_ext, 0.686)
            h_RS.Scale(lumi)
        else:
            h_RS = rs_file.Get("srbase/h_{0}".format(var)).Clone()
            GetTotalHistogram(h_RS, rs_file, var, cr, ht_regs, True)

        h_data.Rebin(rebin[ivar])
        h_RS.Rebin(rebin[ivar])

        if var=="nJet30":
            print "HI"
            bins = np.array([0.,2.,4.,7.,15.])
            h_data = h_data.Rebin(4, str(random.random()), bins)
            h_RS = h_RS.Rebin(4, str(random.random()), bins)
            for i,h in enumerate(h_mc_vec):
                h_mc_vec[i] = h.Rebin(4, str(random.random()), bins)
                
            
        saveAs = outdir+"/{0}/{1}".format(cr+ht_name,var)
        exts = [".pdf",".png"]
        for ext in exts:
            ppm.plotDataMC([h_RS]+h_mc_vec[::-1], ["RS QCD Pred", "Z+jets", "Top", "W+jets"], h_data=h_data,
                           saveAs=saveAs+ext, lumi=lumi, xAxisTitle=titles[ivar], doMT2Colors=True, xRangeUser=xRangeUser[ivar],
                           doOverflow=False, xAxisUnit=units[ivar])




