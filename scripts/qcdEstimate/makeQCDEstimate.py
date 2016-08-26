#! /usr/bin/env python

import os
import sys
import ROOT

indir    = open('inputs.txt').readlines()[0].strip()
MT2indir    = open('inputs.txt').readlines()[1].strip()
tag = indir.split("/")[-1]
outdir = "output/"+tag

data_sample = "data_Run2016B"

ht_reg_names = ["ht200to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf"]
nj_reg_names = ["j2to3","j4to6","j7toInf"]
top_reg_names = ["j2to3_0b","j2to3_1b","j2to3_2b","j4to6_0b","j4to6_1b","j4to6_2b",
                 "j7toInf_0b","j7toInf_1b","j7toInf_2b","j2to6_b3toInf","j7toInf_b3toInf"]

fout = ROOT.TFile(os.path.join(outdir,"qcdEstimate.root"),"RECREATE")

fj_data_dir = fout.mkdir("fj_data")
fj_mc_dir = fout.mkdir("fj_mc")
for iht,ht_reg in enumerate(ht_reg_names):
    print "Getting fj in HT region",ht_reg

    fin = ROOT.TFile(os.path.join(outdir, "qcdHistos.root"))
    fj_mc = fin.Get("fj_{0}/h_qcd_fj".format(ht_reg)).Clone("h_fj_mc_{0}_j0toInf_b0toInf".format(ht_reg))
    fj_data = fin.Get("fj_{0}/h_datasub_fj".format(ht_reg)).Clone("h_fj_data_{0}_j0toInf_b0toInf".format(ht_reg))
    fj_mc.SetDirectory(0)
    fj_data.SetDirectory(0)
    fin.Close()
    
    fj_data_dir.cd()
    fj_data.Write()

    fj_mc_dir.cd()
    fj_mc.Write()

rb_data_dir = fout.mkdir("rb_data")
rb_mc_dir = fout.mkdir("rb_mc")
for inj,nj_reg in enumerate(nj_reg_names):
    print "Getting rb in NJ region",nj_reg

    fin = ROOT.TFile(os.path.join(outdir, "qcdHistos.root"))
    rb_mc = fin.Get("rb_{0}/h_qcd_rb".format(nj_reg)).Clone("h_rb_mc_ht200toInf_{0}_b0toInf".format(nj_reg))
    rb_data = fin.Get("rb_{0}/h_datasub_rb".format(nj_reg)).Clone("h_rb_data_ht200toInf_{0}_b0toInf".format(nj_reg))
    rb_mc.SetDirectory(0)
    rb_data.SetDirectory(0)
    fin.Close()
    
    rb_data_dir.cd()
    rb_data.Write()

    rb_mc_dir.cd()
    rb_mc.Write()

# Get rphi, CR yields and make estimate
nCR_dir = fout.mkdir("nCR")
rphi_data_dir = fout.mkdir("r_effective_data")
rphi_mc_dir = fout.mkdir("r_effective_mc")
qcd_estimate_dir = fout.mkdir("qcdEstimate")
qcd_estimate_mc_dir = fout.mkdir("qcdEstimate_mc")
for iht,ht_reg in enumerate(ht_reg_names):
    for itop,top_reg in enumerate(top_reg_names):
        nj_reg = top_reg.split('_')[0]
        nb_reg = top_reg.split('_')[1]
        htAbbrev = ["VL","L","M","H","UH"][iht]
        
        print "Getting CR yields and forming QCD estimate in region",str(itop+1)+htAbbrev
        
        fin = ROOT.TFile(os.path.join(MT2indir, data_sample+".root"))
        h_mt2bins = fin.Get("crqcd{0}{1}/h_mt2bins".format(itop+1,htAbbrev))
        h_mt2bins.SetName("h_mt2bins_{0}_{1}".format(ht_reg,top_reg))
        h_mt2bins.SetDirectory(0)
        fin.Close()
        
        h_rphi_mc = h_mt2bins.Clone("h_rphi_mc_{0}_{1}".format(ht_reg, top_reg))        
        h_rphi_data = h_mt2bins.Clone("h_rphi_data_{0}_{1}".format(ht_reg, top_reg))
        
        fin = ROOT.TFile(os.path.join(outdir, "qcdHistos.root"))
        fit_mc = fin.Get("rphi_{0}/fit_mc".format(ht_reg))
        fit_data = fin.Get("rphi_{0}/fit_mc".format(ht_reg))
        for i in range(1,h_mt2bins.GetNbinsX()+1):
            h_rphi_mc.SetBinContent(i,fit_mc.Eval(h_mt2bins.GetBinLowEdge(i)))
            h_rphi_data.SetBinContent(i,fit_data.Eval(h_mt2bins.GetBinLowEdge(i)))
            h_rphi_mc.SetBinError(i,0)
            h_rphi_data.SetBinError(i,0)
        fin.Close()

        nCR_dir.cd()
        h_mt2bins.Write()

        rphi_data_dir.cd()
        h_rphi_data.Write()

        rphi_mc_dir.cd()
        h_rphi_mc.Write()

        ## FORM ESTIMATE
        temp_nj_reg = nj_reg
        if temp_nj_reg=="j2to6": temp_nj_reg="j2to3"
        h_rb_data = rb_data_dir.Get("h_rb_data_ht200toInf_{0}_b0toInf".format(temp_nj_reg))
        h_rb_mc = rb_mc_dir.Get("h_rb_mc_ht200toInf_{0}_b0toInf".format(temp_nj_reg))
        h_fj_data = fj_data_dir.Get("h_fj_data_{0}_j0toInf_b0toInf".format(ht_reg))
        h_fj_mc = fj_mc_dir.Get("h_fj_mc_{0}_j0toInf_b0toInf".format(ht_reg))

        if nj_reg == "j2to3":
            fj_data = h_fj_data.GetBinContent(1)
            fj_mc = h_fj_mc.GetBinContent(1)
        elif nj_reg == "j4to6":
            fj_data = h_fj_data.GetBinContent(2)
            fj_mc = h_fj_mc.GetBinContent(2)
        elif nj_reg == "j7toInf":
            fj_data = h_fj_data.GetBinContent(3)
            fj_mc = h_fj_mc.GetBinContent(3)
        elif nj_reg == "j2to6":
            fj_data = h_fj_data.Integral(1,2)
            fj_mc = h_fj_mc.Integral(1,2)
        else:
            raise RuntimeError("Unknown NJ region {0}!!".format(nj_reg))

        if nb_reg == "0b":
            rb_data = h_rb_data.GetBinContent(1)
            rb_mc = h_rb_mc.GetBinContent(1)
        elif nb_reg == "1b":
            rb_data = h_rb_data.GetBinContent(2)
            rb_mc = h_rb_mc.GetBinContent(2)
        elif nb_reg == "2b":
            rb_data = h_rb_data.GetBinContent(3)
            rb_mc = h_rb_mc.GetBinContent(3)
        elif nb_reg == "b3toInf":
            rb_data = h_rb_data.GetBinContent(4)
            rb_mc = h_rb_mc.GetBinContent(4)
        else:
            raise RuntimeError("Unknown NB region {0}!!".format(nb_reg))

        qcdEstimate_data = h_mt2bins.Clone("qcdEstimate_{0}_{1}".format(ht_reg,top_reg))
        qcdEstimate_mc   = h_mt2bins.Clone("qcdEstimate_mc_{0}_{1}".format(ht_reg,top_reg))

        qcdEstimate_data.Multiply(h_rphi_data)
        qcdEstimate_mc.Multiply(h_rphi_mc)
        qcdEstimate_data.Scale(rb_data * fj_data)
        qcdEstimate_mc.Scale(rb_mc * fj_mc)
        
        qcd_estimate_dir.cd()
        qcdEstimate_data.Write()
        
        qcd_estimate_mc_dir.cd()
        qcdEstimate_mc.Write()

fout.Close()
        
