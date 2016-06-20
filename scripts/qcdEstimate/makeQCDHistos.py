#! /usr/bin/env python
## run this on the output of QCDLooper to produce barebones histograms of rphi, fj, rb
## output is stored in directories in a root file. Pass this to makeQCDPlots to make
## pretty plots

import os
import sys
import numpy as np
import ROOT

indir = "/home/users/bemarsh/analysis/mt2/current/MT2Analysis/QCDLooper/output/test_prescale"
outdir = "output/test_prescale"

try:
    os.makedirs(outdir)
except:
    pass

qcd_sample = "qcd_ht"
nqcd_samples = ["wjets_ht","ttsl", "dyjetsll_incl","ttdl", "zinv_ht"]
data_sample = "data_Run2016B"

ht_reg_names = ["ht200to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf"]
nj_reg_names = ["j2to3","j4to6","j7toInf"]

fout = ROOT.TFile(os.path.join(outdir,"QCDhistos.root"),"RECREATE")

## r_phi
for reg in ht_reg_names:
    print "Doing r_phi for region",reg
    
    dirname = "rphi_"+reg
    tdir = fout.mkdir(dirname)

    ## get num/denom histos
    
    # QCD MC
    fin = ROOT.TFile(os.path.join(indir,qcd_sample+".root"))
    try:
        h_qcd_num = fin.Get("rphi_{0}/h_mt2_num".format(reg))
        h_qcd_den = fin.Get("rphi_{0}/h_mt2_den".format(reg))
        h_qcd_num.SetDirectory(0)    
        h_qcd_den.SetDirectory(0)
    except TypeError:
        raise Exception("ERROR: could not get histos from qcd file!!")
    fin.Close()

    # Data
    fin = ROOT.TFile(os.path.join(indir,data_sample+".root"))
    try:
        h_data_num = fin.Get("rphi_{0}/h_mt2_num".format(reg))
        h_data_den = fin.Get("rphi_{0}/h_mt2_den".format(reg))
        h_data_num.SetDirectory(0)
        h_data_den.SetDirectory(0)
    except TypeError:
        raise Exception("ERROR: could not get histos from data file!!")
    fin.Close()

    # MCall and non-qcd-subtracted data
    h_mcall_num = ROOT.TH1D()
    h_mcall_den = ROOT.TH1D()
    h_datasub_num = ROOT.TH1D()
    h_datasub_den = ROOT.TH1D()
    h_qcd_num.Copy(h_mcall_num)
    h_qcd_den.Copy(h_mcall_den)
    h_data_num.Copy(h_datasub_num)
    h_data_den.Copy(h_datasub_den)
    for s in nqcd_samples:
        fin = ROOT.TFile(os.path.join(indir,s+".root"))
        try:
            h_mcall_num.Add(fin.Get("rphi_{0}/h_mt2_num".format(reg)))
            h_mcall_den.Add(fin.Get("rphi_{0}/h_mt2_den".format(reg)))
            h_datasub_num.Add(fin.Get("rphi_{0}/h_mt2_num".format(reg)),-1)
            h_datasub_den.Add(fin.Get("rphi_{0}/h_mt2_den".format(reg)),-1)
        except TypeError:
            ## sometimes some bkg process wont have any events, generates error
            print "WARNING: could not get {0} rphi hists for sample {1}".format(reg,s)
        fin.Close()
   
    #rebin using the following bin edges    
    mt2bins = [50,55,60,65,70,75,80,90,100,120,200,300,450,800,1500]
    mt2bins = np.array(mt2bins, dtype=float)
    
    h_qcd_num = h_qcd_num.Rebin(mt2bins.size-1, "h_qcd_num", mt2bins)
    h_qcd_den = h_qcd_den.Rebin(mt2bins.size-1, "h_qcd_den", mt2bins)
    h_data_num = h_data_num.Rebin(mt2bins.size-1, "h_data_num", mt2bins)
    h_data_den = h_data_den.Rebin(mt2bins.size-1, "h_data_den", mt2bins)
    h_mcall_num = h_mcall_num.Rebin(mt2bins.size-1, "h_mcall_num", mt2bins)
    h_mcall_den = h_mcall_den.Rebin(mt2bins.size-1, "h_mcall_den", mt2bins)
    h_datasub_num = h_datasub_num.Rebin(mt2bins.size-1, "h_datasub_num", mt2bins)
    h_datasub_den = h_datasub_den.Rebin(mt2bins.size-1, "h_datasub_den", mt2bins)

    # compute rphi
    h_qcd_rphi = h_qcd_num.Clone("h_qcd_rphi")
    h_qcd_rphi.Divide(h_qcd_den)
    h_data_rphi = h_data_num.Clone("h_data_rphi")
    h_data_rphi.Divide(h_data_den)
    h_mcall_rphi = h_mcall_num.Clone("h_mcall_rphi")
    h_mcall_rphi.Divide(h_mcall_den)
    h_datasub_rphi = h_datasub_num.Clone("h_datasub_rphi")
    h_datasub_rphi.Divide(h_datasub_den)

    # write to directory
    tdir.cd()
    h_qcd_rphi.Write()
    h_data_rphi.Write()
    h_mcall_rphi.Write()
    h_datasub_rphi.Write()
    h_qcd_num.Write()
    h_data_num.Write()
    h_mcall_num.Write()
    h_datasub_num.Write()
    h_qcd_den.Write()
    h_data_den.Write()
    h_mcall_den.Write()
    h_datasub_den.Write()

# f_j
for reg in ht_reg_names:
    print "Doing f_j for region",reg

    dirname = "fj_"+reg
    tdir = fout.mkdir(dirname)

    # QCD MC
    fin = ROOT.TFile(os.path.join(indir,qcd_sample+".root"))
    h_qcd_fj = fin.Get("fj_{0}/h_njets".format(reg))
    h_qcd_fj.SetDirectory(0)
    if type(h_qcd_fj)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get {0} fj data from qcd sample!!".format(reg))
    fin.Close()

    # data
    fin = ROOT.TFile(os.path.join(indir,data_sample+".root"))
    h_data_fj = fin.Get("fj_{0}/h_njets".format(reg))
    h_data_fj.SetDirectory(0)
    if type(h_data_fj)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get {0} fj data from data sample!!".format(reg))
    fin.Close()

    h_qcd_fj.Scale(1.0/h_qcd_fj.Integral(0,-1))
    h_data_fj.Scale(1.0/h_data_fj.Integral(0,-1))

    njbins = [2,4,7,11]
    njbins = np.array(njbins, dtype=float)

    h_qcd_fj = h_qcd_fj.Rebin(njbins.size-1, "h_qcd_fj", njbins)
    h_data_fj = h_data_fj.Rebin(njbins.size-1, "h_data_fj", njbins)

    tdir.cd()
    h_qcd_fj.Write()
    h_data_fj.Write()

# r_b
for reg in nj_reg_names:
    print "Doing r_b for region",reg

    dirname = "rb_"+reg
    tdir = fout.mkdir(dirname)

    # QCD MC
    fin = ROOT.TFile(os.path.join(indir,qcd_sample+".root"))
    h_qcd_rb = fin.Get("rb_{0}/h_nbjets".format(reg))
    h_qcd_rb.SetDirectory(0)
    if type(h_qcd_rb)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get {0} rb data from qcd sample!!".format(reg))
    fin.Close()

    # data
    fin = ROOT.TFile(os.path.join(indir,data_sample+".root"))
    h_data_rb = fin.Get("rb_{0}/h_nbjets".format(reg))
    h_data_rb.SetDirectory(0)
    if type(h_data_rb)!=type(ROOT.TH1D()):
        raise Exception("ERROR: could not get {0} rb data from data sample!!".format(reg))
    fin.Close()

    h_qcd_rb.Scale(1.0/h_qcd_rb.Integral(0,-1))
    h_data_rb.Scale(1.0/h_data_rb.Integral(0,-1))

    nbjbins = [0,1,2,3,6]
    nbjbins = np.array(nbjbins, dtype=float)

    h_qcd_rb = h_qcd_rb.Rebin(nbjbins.size-1, "h_qcd_rb", nbjbins)
    h_data_rb = h_data_rb.Rebin(nbjbins.size-1, "h_data_rb", nbjbins)

    tdir.cd()
    h_qcd_rb.Write()
    h_data_rb.Write()


fout.Close()



