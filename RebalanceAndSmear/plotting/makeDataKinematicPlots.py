import ROOT
import os
import pyRootPlotMaker as ppm
import numpy as np

username = os.environ["USER"]

ROOT.gROOT.SetBatch(1)

dir = "looper_output/v10/data/"
dir_noRS = "looper_output/v10/data_noRS/"
dir_mc = "looper_output/v10/"

lumi = 36.5

samples = ["zinv_ht", "top", "wjets_ht"]

frs = ROOT.TFile(os.path.join(dir,"merged_hists.root"))
fnrs = ROOT.TFile(os.path.join(dir_noRS,"merged_hists_JetHT.root"))
fmc = [ROOT.TFile(os.path.join(dir_mc,s+".root")) for s in samples]

vars = ["met","ht","mt2","nJet30","nBJet20","J0pt","J1pt","deltaPhiMin","diffMetMhtOverMet"]
names = ["E_{T}^{miss}","H_{T}","M_{T2}","nJet30","nBJet20","p_{T}(jet1)","p_{T}(jet2)","#Delta#phi_{min}","|met - mht|/met"]
units = ["GeV","GeV","GeV",None,None,"GeV","GeV",None,None]
rebin = [2,2,2,1,1,2,2,1,2]

for tdir in ["ht450to1000","ht1000toInf","ht250to1000"]:

  os.system("mkdir -p /home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/tmp/{1}".format(username,tdir))
  os.system("cp ~/scripts/index.php /home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/tmp/{1}/".format(username,tdir))
  
  for ivar,var in enumerate(vars):
    
    hrs = frs.Get("{0}/h_{1}".format(tdir, var))
    hnrs = fnrs.Get("{0}/h_{1}".format(tdir, var))

    hrs.Rebin(rebin[ivar])
    hnrs.Rebin(rebin[ivar])    

    h_mc_vec = [f.Get("{0}/h_{1}".format(tdir,var)) for f in fmc]
    for h in h_mc_vec:      
      h.Scale(lumi)
      h.Rebin(rebin[ivar])
    
    for ext in [".pdf",".png",".root"]:
      saveAs = "/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/tmp/{1}/{1}_{2}{3}".format(username,tdir,var,ext)
      ppm.plotDataMC([hrs]+h_mc_vec[::-1], ["RS from data", "Z+jets", "Top", "W+jets"], h_data=hnrs, dataTitle="data", saveAs=saveAs, 
                     xAxisTitle=names[ivar], xAxisUnit=units[ivar])
    
    if "nJet30" in var or "nBJet20" in var:
      newbins = np.array([0.,2.,4.,7.,15.]) if var=="nJet30" else np.array([0.,1.,2.,3.,6.])
      tmp_hrs = hrs.Rebin(4,"tmp_hrs",newbins)
      tmp_hnrs = hnrs.Rebin(4,"tmp_hnrs",newbins)
      tmp_h_mc_vec = [h.Rebin(4,"tmp_"+h.GetName(),newbins) for h in h_mc_vec]
    
      for ext in [".pdf",".png",".root"]:
        saveAs = "/home/users/{0}/public_html/mt2/RebalanceAndSmear/data_tests2/tmp/{1}/{1}_{2}{3}".format(username,tdir,var+"_rebin",ext)
        ppm.plotDataMC([tmp_hrs]+tmp_h_mc_vec[::-1], ["RS from data", "Z+jets", "Top", "W+jets"], h_data=tmp_hnrs, dataTitle="data", saveAs=saveAs, 
                       xAxisTitle=names[ivar], xAxisUnit=units[ivar])
