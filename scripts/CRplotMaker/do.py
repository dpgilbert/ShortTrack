#! /usr/bin/env python
import sys
import ROOT
# run in batch mode to suppress plot windows
ROOT.gROOT.SetBatch(1)

from MT2PlotMaker import *
import MT2PlotDefs as pd

pd.lumi = 20.1
pd.lumiUnit = "fb"

## 20.1/fb
input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_20p1fb"
## 12.9/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_12p9fb"
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-07_nojson_skim_base_mt2gt200_ZinvV6_12p9fb"
## 7.65/fb
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-05_nojson_skim_base_mt2gt200_ZinvV4_7p65fb"
# input_dir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-04_MiniAODv2_nojson_skim_base_mt2gt200_ZinvV4_7p65fb"
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

output_dir = "crplots/crplots_20p1fb"

exts = ["pdf","png"]

# removed lepton
MT2PlotMaker(input_dir, ["2015wjets_ht", "top", "2015qcd_ht"], "data_Run2016", "crrlbase", pd.rl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["2015wjets_ht", "top", "2015qcd_ht"], "data_Run2016", "crrlbaseJ", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["2015wjets_ht", "top", "2015qcd_ht"], "data_Run2016", "crrlbaseIncl", pd.ht_njet_plots, output_dir, exts)

# single lepton
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbase", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslwjets", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["top", "wjets_ht", "qcd_ht"], "data_Run2016", "crslttbar", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslelbase", pd.sl_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslmubase", pd.sl_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["wjets_ht", "top"], "data_Run2016", "crslbaseJ", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseVL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseM", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseH", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["wjets_ht", "top", "qcd_ht"], "data_Run2016", "crslbaseUH", pd.mt2_only, output_dir, exts)

# photon+jets
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbase", pd.gj_plots, output_dir, exts)
 
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseJ", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseIncl", pd.gamma_vars_only, output_dir, exts)
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseIncl", pd.ht_njet_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseVL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseL", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseM", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseH", pd.mt2_only, output_dir, exts)
MT2PlotMaker(input_dir, ["2015gjets_ht", "fragphoton", "fakephoton"], "data_Run2016", "crgjbaseUH", pd.mt2_only, output_dir, exts)

# z->ll
MT2PlotMaker(input_dir, ["2015dyjetsll_ht", "top"], "data_Run2016", "crdybase", pd.dy_plots, output_dir, exts)

MT2PlotMaker(input_dir, ["2015dyjetsll_ht", "top"], "data_Run2016", "crdybaseJ", pd.ht_njet_plots, output_dir, exts)
MT2PlotMaker(input_dir, ["2015dyjetsll_ht", "top"], "data_Run2016", "crdybaseIncl", pd.ht_njet_plots, output_dir, exts)

# QCD monojet
MT2PlotMaker(input_dir, ["qcd_ht", "wjets_ht", "2015zinv_ht"], "data_Run2016", "crqcdbaseJ", pd.qcdJ_plots, output_dir, exts)

# Lost Lepton MT2 plots, normalized in each HT, nj, nb bin
makeNormalizedLostLep(input_dir, ["wjets_ht", "top"], "data_Run2016", output_dir, exts, ht_regs=["VL","L","M","H","UH"])

# # example of using no data
# MT2PlotMaker(input_dir, ["top","wjets_ht","qcd_ht"], None, "srbaseM", pd.mt2_only, output_dir, exts)
