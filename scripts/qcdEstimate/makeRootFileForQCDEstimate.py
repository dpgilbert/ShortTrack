#!/bin/python

import os
import sys
import ROOT

fyields_mc = "/home/users/fgolf/mt2/current/MT2Analysis//MT2looper/output/ichep_40ifb/qcd_ht.root"
fyields_data = "/home/users/fgolf/mt2/current/MT2Analysis//MT2looper/output/ichep_40ifb/data_Run2016B.root"
outdir = "output/V00-08-08_nojson_12p9fb/"
fhists = outdir+"/qcdHistos.root"
outf1 = outdir+"/qcdEstimateData.root"
#outf2 = outdir+"/qcdEstimateMonojet.root"

finyields_mc = ROOT.TFile(fyields_mc)
finyields_data = ROOT.TFile(fyields_data)
finhists = ROOT.TFile(fhists)
fout1 = ROOT.TFile(outf1,"RECREATE")
#fout2 = ROOT.TFile(os.path(outf2),"RECREATE")

#
# make output directories
#
fout1.mkdir("qcdEstimate")
fout1.mkdir("qcdEstimateMC")
fout1.mkdir("nCR")
fout1.mkdir("nCRmc")
fout1.mkdir("r_effective")
fout1.mkdir("r_hat_mc")
fout1.mkdir("f_jets_mc")
fout1.mkdir("r_hat_data")
fout1.mkdir("f_jets_data")
fout1.mkdir("f_jets_data_noPS")
fout1.mkdir("qcdPurity")
fout1.mkdir("r_effectiveMC")
fout1.mkdir("r_systFit")

#
# build list of all the regions
#
dirlist = list(finyields_mc.GetListOfKeys())

ht_names = ["ht200to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf"]
nj_names = ["j2to3","j4to6","j7toInf"]

for iht in ht_names:
    fj_name_mc = "fj_"+iht+"/h_qcd_fj"
    fj_name_data = "fj_"+iht+"/h_datasub_fj"

    hfj_mc = finhists.Get(fj_name_mc).Clone("yield_f_jets_mc_"+iht.replace("ht","HT")+"_j2toInf_b0toInf")
    hfj_data = finhists.Get(fj_name_data).Clone("yield_f_jets_data_"+iht.replace("ht","HT")+"_j2toInf_b0toInf")
    
    tdir = fout1.mkdir("f_jets_mc/"+iht.replace("ht","HT")+"_j2toInf_b0toInf")
    fout1.cd("f_jets_mc/"+iht.replace("ht","HT")+"_j2toInf_b0toInf")
    hfj_mc.Write()

    tdir = fout1.mkdir("f_jets_data/"+iht.replace("ht","HT")+"_j2toInf_b0toInf")
    fout1.cd("f_jets_data/"+iht.replace("ht","HT")+"_j2toInf_b0toInf")
    hfj_data.Write()

for inj in nj_names:
    rb_name_mc = "rb_"+inj+"/h_qcd_rb"
    rb_name_data = "rb_"+inj+"/h_datasub_rb"

    hrb_mc = finhists.Get(rb_name_mc).Clone("yield_r_hat_mc_HT200toInf+"+inj+"_b0toInf")
    hrb_data = finhists.Get(rb_name_data).Clone("yield_r_hat_data_HT200toInf+"+inj+"_b0toInf")

    tdir = fout1.mkdir("r_hat_mc/HT200toInf+"+inj+"_b0toInf")
    fout1.cd("r_hat_mc/HT200toInf+"+inj+"_b0toInf")
    hrb_mc.Write()

    tdir = fout1.mkdir("r_hat_data/HT200toInf+"+inj+"_b0toInf")
    fout1.cd("r_hat_data/HT200toInf+"+inj+"_b0toInf")
    hrb_data.Write()

for idir in dirlist:
    htname = ""
    njname = ""
    nbname = ""

    if idir.GetName().find("crqcd") < 0:
        continue
    if idir.GetName().find("base") >= 0:
        continue
    if idir.GetName()[-1] == "J":
        continue
    
    ht_hist = finyields_mc.Get(idir.GetName()+"/h_htbins")
    ht_maxbin = ht_hist.GetMaximumBin()
    ht_nbins = ht_hist.GetNbinsX()
    ht_low = "0"
    ht_high = "Inf"   
    if ht_maxbin < ht_nbins:
        ht_low = str(int(ht_hist.GetBinLowEdge(ht_maxbin)))
        ht_high = str(int(ht_hist.GetBinLowEdge(ht_maxbin)+ht_hist.GetBinWidth(ht_maxbin)))
    elif ht_maxbin == ht_nbins:
        ht_low = str(int(ht_hist.GetBinLowEdge(ht_maxbin)))
    
    nj_hist = finyields_mc.Get(idir.GetName()+"/h_njbins")
    nj_maxbin = nj_hist.GetMaximumBin()
    nj_nbins = nj_hist.GetNbinsX()
    nj_low = "0"
    nj_high = "Inf"   
    if nj_maxbin < nj_nbins:
        nj_low = str(int(nj_hist.GetBinLowEdge(nj_maxbin)))
        nj_high = str(int(nj_hist.GetBinLowEdge(nj_maxbin)+nj_hist.GetBinWidth(nj_maxbin))-1)
    elif nj_maxbin == nj_nbins:
        nj_low = str(int(nj_hist.GetBinLowEdge(nj_maxbin)))

    nb_hist = finyields_mc.Get(idir.GetName()+"/h_nbjbins")    
    nb_maxbin = nb_hist.GetMaximumBin()
    nb_nbins = nb_hist.GetNbinsX()
    nb_low = "0"
    nb_high = "Inf"   
    if nb_maxbin < nb_nbins:
        nb_low = str(int(nb_hist.GetBinLowEdge(nb_maxbin)))
        nb_high = str(int(nb_hist.GetBinLowEdge(nb_maxbin)+nb_hist.GetBinWidth(nb_maxbin))-1)
    elif nb_maxbin == nb_nbins:
        nb_low = str(int(nb_hist.GetBinLowEdge(nb_maxbin)))

    mt2_hist_mc = finyields_mc.Get(idir.GetName()+"/h_mt2bins")
    mt2_hist_data = finyields_data.Get(idir.GetName()+"/h_mt2bins")        
        
    odirname = "HT"+ht_low+"to"+ht_high+"_j"+nj_low+"to"+nj_high+"_b"+nb_low
    if int(float(nb_low)) == 3:
        odirname = odirname+"to"+nb_high

    hout_mc = finyields_mc.Get(idir.GetName()+"/h_mt2bins").Clone("yield_nCR_"+odirname)
    hout_data = finyields_mc.Get(idir.GetName()+"/h_mt2bins").Clone("yield_nCR_"+odirname)    
    if not fout1.GetDirectory("nCR/"+odirname):
        print "processing directory %s" % idir.GetName()    
        tdir = fout1.mkdir("nCR/"+odirname)
        fout1.cd("nCR/"+odirname)
        hout_data.Write()
    if not fout1.GetDirectory("nCRmc/"+odirname):
        tdir = fout1.mkdir("nCRmc/"+odirname)
        fout1.cd("nCRmc/"+odirname)
        hout_mc.Write()    

    rphi_name_mc = "rphi_ht"+ht_low+"to"+ht_high+"/fit_mc"
    rphi_name_data = "rphi_ht"+ht_low+"to"+ht_high+"/fit_data"    
        
    rphi_fit_mc = finhists.Get(rphi_name_mc)
    rphi_fit_data = finhists.Get(rphi_name_data)    
    
    #
    # for now, take rphi_eff from value at center of bin
    #
    rphi_eff_mc = finyields_mc.Get(idir.GetName()+"/h_mt2bins").Clone("yield_r_effectiveMC_"+odirname)
    rphi_eff_data = finyields_data.Get(idir.GetName()+"/h_mt2bins").Clone("yield_r_effective_"+odirname)    
    for i in range(1,rphi_eff_mc.GetNbinsX()+1):
        cbin = rphi_eff_mc.GetBinCenter(i)
        rphi_eff_mc.SetBinContent(i, rphi_fit_mc.Eval(cbin))
        rphi_eff_mc.SetBinError(i, 0)
        rphi_eff_data.SetBinContent(i, rphi_fit_data.Eval(cbin))
        rphi_eff_data.SetBinError(i, 0)

    if not fout1.GetDirectory("r_effective/"+odirname):
        tdir = fout1.mkdir("r_effective/"+odirname)
        fout1.cd("r_effective/"+odirname)
        rphi_eff_data.Write()

    if not fout1.GetDirectory("r_effectiveMC/"+odirname):        
        tdir = fout1.mkdir("r_effectiveMC/"+odirname)
        fout1.cd("r_effectiveMC/"+odirname)
        rphi_eff_mc.Write()

    #
    # now, let's put it together and make qcd estimate
    #
    hqcdest_mc = finyields_mc.Get(idir.GetName()+"/h_mt2bins").Clone("yield_qcdEstimate_"+odirname)
    hqcdest_data = finyields_data.Get(idir.GetName()+"/h_mt2bins").Clone("yield_qcdEstimate_"+odirname)    

    hqcdest_mc.Multiply(rphi_eff_mc)
    hqcdest_data.Multiply(rphi_eff_data)

    htname = "ht"+ht_low+"to"+ht_high
    njname = "j"+nj_low+"to"+nj_high    

    nj_idx = nj_names.index(njname)+1
    nb_idx = int(float(nb_low))+1
    
    fj_mc = finhists.Get("fj_"+htname+"/h_qcd_fj").GetBinContent(nj_idx)
    rb_mc = finhists.Get("rb_"+njname+"/h_qcd_rb").GetBinContent(nb_idx)

    fj_data = finhists.Get("fj_"+htname+"/h_datasub_fj").GetBinContent(nj_idx)
    rb_data = finhists.Get("rb_"+njname+"/h_datasub_rb").GetBinContent(nb_idx)

    hqcdest_mc.Scale(fj_mc*rb_mc)
    hqcdest_data.Scale(fj_data*rb_data)

    if not fout1.GetDirectory("qcdEstimate/"+odirname):
        tdir = fout1.mkdir("qcdEstimate/"+odirname)
        fout1.cd("qcdEstimate/"+odirname)
        hqcdest_data.Write()

    if not fout1.GetDirectory("qcdEstimateMC/"+odirname):
        tdir = fout1.mkdir("qcdEstimateMC/"+odirname)
        fout1.cd("qcdEstimateMC/"+odirname)
        hqcdest_mc.Write()
    
fout1.Close()
