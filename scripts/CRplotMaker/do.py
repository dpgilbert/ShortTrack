#! /usr/bin/env python
import sys
import ROOT
# run in batch mode to suppress plot windows
ROOT.gROOT.SetBatch(1)

from MT2PlotMaker import *
import MT2PlotDefs as pd

pd.lumi = 36.8
pd.lumiUnit = "fb"

#input_dir = "/home/users/olivito/mt2_80x/MT2Analysis/MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_dypt100_30/"
#input_dir = "/home/users/gzevi/MT2/MT2Analysis80X/MT2Analysis/MT2looper/output/V00-08-09_15Dec16/"
input_dir = "/home/users/gzevi/MT2/MT2Analysis80X/MT2Analysis/MT2looper/output/V00-08-09_16Dec16/"
## 31.24/fb
#input_dir = "/home/users/mderdzinski/summer2016/current_master_mt2/MT2looper/output/31p24_oldMC_nov7"
## 20.1/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_20p1fb"
## 12.9/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_12p9fb"
## 7.65/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-05_nojson_skim_base_mt2gt200_ZinvV4_7p65fb"
## 5.9/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-04_MiniAODv2_nojson_skim_base_mt2gt200_ZinvV4_5p9fb"
## 4.0/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4_3p99fb"
## 2.6/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-02_nojson_skim_base_mt2gt200_ZinvV4_2p6fb"
## 2.1/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-02_json_Cert_271036-274421_skim_base_mt2gt200_ZinvV4"
## 804/pb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-01_json_Cert_271036-274240_skim_base_mt2gt200_ZinvV5"
## 589/pb
#input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-00_json_Cert_271036-273730_skim_base_mt2gt200_ZinvV4"

output_dir = "crplots/crplots_36p5fb_16Dec16"

exts = ["pdf","png"]

# removed lepton
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crrlbase", pd.rl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crrlbaseJ", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crrlbaseIncl", pd.ht_njet_plots, output_dir, exts)

# single lepton
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbase", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslwjets", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["top", "wjets_ht", "qcd_ht"], "data_Run2016", "crslttbar", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslelbase", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslmubase", pd.sl_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseJ", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseVL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseM", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseH", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseUH", pd.mt2_only, output_dir, exts)

# photon+jets
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbase", pd.gj_plots, output_dir, exts)
 
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseJ", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseIncl", pd.gamma_vars_only, output_dir, exts)
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseIncl", pd.ht_njet_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseVL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseM", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseH", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["gjets_dr0p05_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseUH", pd.mt2_only, output_dir, exts)

# z->ll
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybase", pd.dy_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseJ", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseIncl", pd.ht_njet_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["dyjetsll_ht", "ww", "top"], "data_Run2016", "crdybaseInclLowPtOF", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "ww", "top"], "data_Run2016", "crdybaseInclLowPtSF", pd.ht_njet_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseJ", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseVL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseM", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseH", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["dyjetsll_ht", "top"], "data_Run2016", "crdybaseUH", pd.mt2_only, output_dir, exts)

# Lost Lepton MT2 plots, normalized in each HT, nj, nb bin
#makeNormalizedLostLep(input_dir, ["wjets_ht", "top"], "data_Run2016", output_dir, exts, ht_regs=["VL","L","M","H","UH"])



# # Lost Lepton Hybrid MT2 plots
makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["VL","L","M","H","UH"])
# makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["L","M","H","UH"])
# makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["M","H","UH"])
# makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["H","UH"])
makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["VL"])
makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["L"])
makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["M"])
makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["H"])
makeLostLepHybrid(input_dir, ["lostlepFromCRs"], "data_Run2016", output_dir, exts, ht_regs=["UH"])

# QCD monojet
MT2PlotMaker(input_dir, ["qcd_ht", "wjets_ht", "zinv_ht"], "data_Run2016", "crqcdbaseJ", pd.qcdJ_plots, output_dir, exts)



# # example of using no data
# MT2PlotMaker(input_dir, ["top","wjets_ht","qcd_ht"], None, "srbaseM", pd.mt2_only, output_dir, exts)


