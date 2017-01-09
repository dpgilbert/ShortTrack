import ROOT
import os
import pyRootPlotMaker as ppm
import numpy as np

username = os.environ["USER"]

ROOT.gROOT.SetBatch(1)

dir = "looper_output/v10/qcd/"
dir_noRS = "looper_output/v10/qcd_noRS/"

inputs = ['merged_hists.root','merged_hists_qcd_ht1000to1500.root','merged_hists_qcd_ht1500to2000.root','merged_hists_qcd_ht2000toInf.root','merged_hists_qcd_ht300to500.root','merged_hists_qcd_ht500to700.root','merged_hists_qcd_ht700to1000.root']

for inp in inputs:

  frse = ROOT.TFile(os.path.join(dir,inp))
  fnrse = ROOT.TFile(os.path.join(dir_noRS,inp))

  os.system("mkdir -p /home/users/{0}/public_html/mt2/RebalanceAndSmear/MCtests2/tmp/{1}".format(username,inp.replace('.root','')))
  os.system("cp ~/scripts/index.php /home/users/{0}/public_html/mt2/RebalanceAndSmear/MCtests2/tmp/{1}/".format(username,inp.replace('.root','')))
  
  vars = ["met","ht","mt2","nJet30","nBJet20","J0pt","J1pt","deltaPhiMin","diffMetMhtOverMet"]
  names = ["E_{T}^{miss}","H_{T}","M_{T2}","nJet30","nBJet20","p_{T}(jet1)","p_{T}(jet2)","#Delta#phi_{min}","|met - mht|/met"]
  units = ["GeV","GeV","GeV",None,None,"GeV","GeV",None,None]
  rebin = [2,2,2,1,1,2,2,1,2]
  
  for tdir in ["ht450to1000","ht1000toInf","ht250to1000"]:
#  for tdir in ["ht450to1000","ht1000toInf"]:    

    os.system("mkdir -p /home/users/{0}/public_html/mt2/RebalanceAndSmear/MCtests2/tmp/{1}/{2}".format(username,inp.replace('.root',''),tdir))
    os.system("cp ~/scripts/index.php /home/users/{0}/public_html/mt2/RebalanceAndSmear/MCtests2/tmp/{1}/{2}/".format(username,inp.replace('.root',''),tdir))
    
    for ivar,var in enumerate(vars):
      
      hrs = frse.Get("{0}/h_{1}".format(tdir, var))
      hnrs = fnrse.Get("{0}/h_{1}".format(tdir, var))
  
      hrs.Rebin(rebin[ivar])
      hnrs.Rebin(rebin[ivar])
  
      for ext in [".pdf",".png",".root"]:
          saveAs = "/home/users/{0}/public_html/mt2/RebalanceAndSmear/MCtests2/tmp/{4}/{1}/{1}_{2}{3}".format(username,tdir,var,ext,inp.replace('.root',''))
          ppm.plotDataMC([hnrs], ["QCD MC"], h_data=hrs, dataTitle="R&S from MC", saveAs=saveAs, 
                         xAxisTitle=names[ivar], xAxisUnit=units[ivar])
      
      if "nJet30" in var or "nBJet20" in var:
        newbins = np.array([0.,2.,4.,7.,15.]) if var=="nJet30" else np.array([0.,1.,2.,3.,6.])
        tmp_hrs = hrs.Rebin(4,"tmp_hrs",newbins)
        tmp_hnrs = hnrs.Rebin(4,"tmp_hnrs",newbins)
      
        for ext in [".pdf",".png",".root"]:
          saveAs = "/home/users/{0}/public_html/mt2/RebalanceAndSmear/MCtests2/tmp/{4}/{1}/{1}_{2}{3}".format(username,tdir,var+"_rebin",ext,inp.replace('.root',''))
          ppm.plotDataMC([tmp_hnrs], ["QCD MC"], h_data=tmp_hrs, dataTitle="R&S from MC", saveAs=saveAs, 
                         xAxisTitle=names[ivar], xAxisUnit=units[ivar])
