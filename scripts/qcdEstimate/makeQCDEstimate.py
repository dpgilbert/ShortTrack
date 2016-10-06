#! /usr/bin/env python

import os
import sys
import ROOT
import numpy as np

indir    = open('inputs.txt').readlines()[0].strip()
MT2indir    = open('inputs.txt').readlines()[1].strip()
tag = indir.split("/")[-1]
outdir = "output/"+tag

data_sample = "data_Run2016"
qcd_sample = "qcd_ht"
nonqcd_samples = ["top","wjets_ht","zinv_ht","dyjetsll_ht"]

ht_reg_names = ["ht200to450","ht450to575","ht575to1000","ht1000to1500","ht1500toInf"]
nj_reg_names = ["j2to3","j4to6","j7toInf"]
top_reg_names = ["j2to3_0b","j2to3_1b","j2to3_2b","j4to6_0b","j4to6_1b","j4to6_2b",
                 "j7toInf_0b","j7toInf_1b","j7toInf_2b","j2to6_b3toInf","j7toInf_b3toInf"]

fout = ROOT.TFile(os.path.join(outdir,"qcdEstimate.root"),"RECREATE")

fj_data_dir = fout.mkdir("f_jets_data")
fj_mc_dir = fout.mkdir("f_jets_mc")
for iht,ht_reg in enumerate(ht_reg_names):
    print "Getting fj in HT region",ht_reg

    fin = ROOT.TFile(os.path.join(outdir, "qcdHistos.root"))
    fj_mc = fin.Get("fj_{0}/h_qcd_fj".format(ht_reg)).Clone("yield_f_jets_mc_{0}_j2toInf_b0toInf".format(ht_reg.replace("ht","HT")))
    fj_data = fin.Get("fj_{0}/h_datasub_fj".format(ht_reg)).Clone("yield_f_jets_data_{0}_j2toInf_b0toInf".format(ht_reg.replace("ht","HT")))
    fj_mc.SetDirectory(0)
    fj_data.SetDirectory(0)
    fin.Close()
    
    d = fj_data_dir.mkdir("{0}_j2toInf_b0toInf".format(ht_reg.replace("ht","HT")))
    d.cd()
    fj_data.Write()

    d = fj_mc_dir.mkdir("{0}_j2toInf_b0toInf".format(ht_reg.replace("ht","HT")))    
    d.cd()
    fj_mc.Write()

rb_data_dir = fout.mkdir("r_hat_data")
rb_mc_dir = fout.mkdir("r_hat_mc")
for inj,nj_reg in enumerate(nj_reg_names):
    print "Getting rb in NJ region",nj_reg

    fin = ROOT.TFile(os.path.join(outdir, "qcdHistos.root"))
    rb_mc = fin.Get("rb_{0}/h_qcd_rb".format(nj_reg)).Clone("yield_r_hat_mc_HT200toInf_{0}_b0toInf".format(nj_reg))
    rb_data = fin.Get("rb_{0}/h_datasub_rb".format(nj_reg)).Clone("yield_r_hat_data_HT200toInf_{0}_b0toInf".format(nj_reg))
    rb_mc.SetDirectory(0)
    rb_data.SetDirectory(0)
    fin.Close()
    
    d = rb_data_dir.mkdir("HT200toInf_{0}_b0toInf".format(nj_reg))
    d.cd()
    rb_data.Write()

    d = rb_mc_dir.mkdir("HT200toInf_{0}_b0toInf".format(nj_reg))
    d.cd()
    rb_mc.Write()

# Get rphi, CR yields, qcdPurity and make estimate
nCR_dir = fout.mkdir("nCR")
purity_dir = fout.mkdir("qcdPurity")
r_systfit_dir = fout.mkdir("r_systFit")
rphi_data_dir = fout.mkdir("r_effective")
rphi_mc_dir = fout.mkdir("r_effectiveMC")
qcd_estimate_dir = fout.mkdir("qcdEstimate")
qcd_estimate_mc_dir = fout.mkdir("qcdEstimate_mc")
for iht,ht_reg in enumerate(ht_reg_names):
    for itop,top_reg in enumerate(top_reg_names):
        nj_reg = top_reg.split('_')[0]
        nb_reg = top_reg.split('_')[1]
        htAbbrev = ["VL","L","M","H","UH"][iht]
        
        print "Getting CR yields and forming QCD estimate in region",str(itop+1)+htAbbrev
        
        #purity calc
        fqcd = ROOT.TFile(os.path.join(MT2indir, qcd_sample+".root"))
        h_purity = fqcd.Get("crqcd{0}{1}/h_mt2bins".format(itop+1,htAbbrev)).Clone("yield_qcdPurity_{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        h_denom = h_purity.Clone("h_denom")
        for nqcd in nonqcd_samples:
            fnqcd = ROOT.TFile(os.path.join(MT2indir, nqcd+".root"))
            h_denom.Add(fnqcd.Get("crqcd{0}{1}/h_mt2bins".format(itop+1,htAbbrev)))
            fnqcd.Close()
        h_purity.Divide(h_denom)

        # get nCR
        fin = ROOT.TFile(os.path.join(MT2indir, data_sample+".root"))
        h_mt2bins = fin.Get("crqcd{0}{1}/h_mt2bins".format(itop+1,htAbbrev))
        h_mt2bins.SetName("yield_nCR_{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        h_mt2bins.SetDirectory(0)
        
        # do r_effective calculation
        h_rphi_mc = h_mt2bins.Clone("yield_r_effectiveMC_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))        
        h_rphi_data = h_mt2bins.Clone("yield_r_effective_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))
        h_rphi_mc_systUp = h_mt2bins.Clone("yield_systUp_r_effectiveMC_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))        
        h_rphi_data_systUp = h_mt2bins.Clone("yield_systUp_r_effective_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))
        h_rphi_mc_systDown = h_mt2bins.Clone("yield_systDown_r_effectiveMC_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))        
        h_rphi_data_systDown = h_mt2bins.Clone("yield_systDown_r_effective_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))
        h_r_systFit = h_mt2bins.Clone("yield_r_systFit_{0}_{1}".format(ht_reg.replace("ht","HT"), top_reg))

        for i in range(1,h_mt2bins.GetNbinsX()+1):
            dname = "crqcd{0}{1}".format(itop+1,htAbbrev)

            sumRphi = fin.Get(dname+"/h_sumRphi").GetBinContent(i)
            sumRphi_err = fin.Get(dname+"/h_sumRphi").GetBinError(i)
            sumRphi_up = fin.Get(dname+"/h_sumRphi_systUp").GetBinContent(i)
            sumRphi_down = fin.Get(dname+"/h_sumRphi_systDown").GetBinContent(i)

            sumRphi_mc = fin.Get(dname+"/h_sumRphi_mc").GetBinContent(i)
            sumRphi_mc_err = fin.Get(dname+"/h_sumRphi_mc").GetBinError(i)
            sumRphi_mc_up = fin.Get(dname+"/h_sumRphi_mc_systUp").GetBinContent(i)
            sumRphi_mc_down = fin.Get(dname+"/h_sumRphi_mc_systDown").GetBinContent(i)
            
            nCR = fin.Get(dname+"/h_mt2bins").GetBinContent(i)
            # all of the sumRphis are 0 anyway, so prevent divide by 0 errors
            if nCR==0:
                nCR = 1
            
            h_rphi_mc.SetBinContent(i, sumRphi_mc/nCR)
            h_rphi_data.SetBinContent(i, sumRphi/nCR)
            h_rphi_mc_systUp.SetBinContent(i, sumRphi_mc_up/nCR)
            h_rphi_data_systUp.SetBinContent(i, sumRphi_up/nCR)
            h_rphi_mc_systUp.SetBinError(i,0)
            h_rphi_data_systUp.SetBinError(i,0)
            h_rphi_mc_systDown.SetBinContent(i, sumRphi_mc_down/nCR)
            h_rphi_data_systDown.SetBinContent(i, sumRphi_down/nCR)
            h_rphi_mc_systDown.SetBinError(i,0)
            h_rphi_data_systDown.SetBinError(i,0)

            maxdiff = max(abs(sumRphi-sumRphi_up),abs(sumRphi-sumRphi_down))
            relerr = maxdiff/sumRphi if sumRphi>0 else 0

            h_r_systFit.SetBinContent(i, relerr)
            h_r_systFit.SetBinError(i, 0)

            err = np.sqrt((sumRphi_err/nCR)**2+ (relerr*sumRphi/nCR)**2)
            h_rphi_data.SetBinError(i,err)

            err = np.sqrt((sumRphi_mc_err/nCR)**2+ (relerr*sumRphi_mc/nCR)**2)
            h_rphi_mc.SetBinError(i,err)

#        fin.Close()

        d = purity_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        h_purity.Write()

        d = nCR_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        h_mt2bins.Write()

        d = rphi_data_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        h_rphi_data.Write()
        h_rphi_data_systUp.Write()
        h_rphi_data_systDown.Write()

        d = rphi_mc_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        h_rphi_mc.Write()
        h_rphi_mc_systUp.Write()
        h_rphi_mc_systDown.Write()

        d = r_systfit_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        h_r_systFit.Write()

        ## FORM ESTIMATE
        temp_nj_reg = nj_reg
        if temp_nj_reg=="j2to6": temp_nj_reg="j2to3"
        h_rb_data = rb_data_dir.Get("HT200toInf_{0}_b0toInf/yield_r_hat_data_HT200toInf_{0}_b0toInf".format(temp_nj_reg))
        h_rb_mc = rb_mc_dir.Get("HT200toInf_{0}_b0toInf/yield_r_hat_mc_HT200toInf_{0}_b0toInf".format(temp_nj_reg))
        h_fj_data = fj_data_dir.Get("{0}_j2toInf_b0toInf/yield_f_jets_data_{0}_j2toInf_b0toInf".format(ht_reg.replace("ht","HT")))
        h_fj_mc = fj_mc_dir.Get("{0}_j2toInf_b0toInf/yield_f_jets_mc_{0}_j2toInf_b0toInf".format(ht_reg.replace("ht","HT")))

        ibin = 0
        if nj_reg == "j2to3":
            fj_data = h_fj_data.GetBinContent(1)
            fj_mc = h_fj_mc.GetBinContent(1)
            fj_data_relerr = h_fj_data.GetBinError(1)/fj_data
            fj_mc_relerr = h_fj_mc.GetBinError(1)/fj_mc
        elif nj_reg == "j4to6":
            fj_data = h_fj_data.GetBinContent(2)
            fj_mc = h_fj_mc.GetBinContent(2)
            fj_data_relerr = h_fj_data.GetBinError(2)/fj_data
            fj_mc_relerr = h_fj_mc.GetBinError(2)/fj_mc
        elif nj_reg == "j7toInf":
            fj_data = h_fj_data.GetBinContent(3)
            fj_mc = h_fj_mc.GetBinContent(3)
            fj_data_relerr = h_fj_data.GetBinError(3)/fj_data
            fj_mc_relerr = h_fj_mc.GetBinError(3)/fj_mc
        elif nj_reg == "j2to6":
            fj_data_relerr = ROOT.Double(0)
            fj_mc_relerr = ROOT.Double(0)
            fj_data = h_fj_data.IntegralAndError(1,2,fj_data_relerr)
            fj_mc = h_fj_mc.IntegralAndError(1,2,fj_mc_relerr)
            fj_data_relerr /= fj_data
            fj_mc_relerr /= fj_mc
        else:
            raise RuntimeError("Unknown NJ region {0}!!".format(nj_reg))


        if nb_reg == "0b":
            rb_data = h_rb_data.GetBinContent(1)
            rb_mc = h_rb_mc.GetBinContent(1)
            rb_data_err = h_rb_data.GetBinError(1)/rb_data
            rb_mc_err = h_rb_mc.GetBinError(1)/rb_mc
        elif nb_reg == "1b":
            rb_data = h_rb_data.GetBinContent(2)
            rb_mc = h_rb_mc.GetBinContent(2)
            rb_data_err = h_rb_data.GetBinError(2)/rb_data
            rb_mc_err = h_rb_mc.GetBinError(2)/rb_mc
        elif nb_reg == "2b":
            rb_data = h_rb_data.GetBinContent(3)
            rb_mc = h_rb_mc.GetBinContent(3)
            rb_data_err = h_rb_data.GetBinError(3)/rb_data
            rb_mc_err = h_rb_mc.GetBinError(3)/rb_mc
        elif nb_reg == "b3toInf":
            rb_data = h_rb_data.GetBinContent(4)
            rb_mc = h_rb_mc.GetBinContent(4)
            rb_data_err = h_rb_data.GetBinError(4)/rb_data
            rb_mc_err = h_rb_mc.GetBinError(4)/rb_mc
        else:
            raise RuntimeError("Unknown NB region {0}!!".format(nb_reg))

        qcdEstimate_data = h_mt2bins.Clone("yield_qcdEstimate_{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        qcdEstimate_mc   = h_mt2bins.Clone("yield_qcdEstimate_mc_{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))

        qcdEstimate_data.Multiply(h_rphi_data)
        qcdEstimate_mc.Multiply(h_rphi_mc)
        qcdEstimate_data.Scale(rb_data * fj_data)
        qcdEstimate_mc.Scale(rb_mc * fj_mc)

        for ibin in range(1,qcdEstimate_data.GetNbinsX()+1):
            val = qcdEstimate_data.GetBinContent(i)
            err = qcdEstimate_data.GetBinError(i)
            qcdEstimate_data.SetBinError(i, np.sqrt((err/val)**2+fj_data_relerr**2+rb_data_err**2)*val if val!=0 else 0)
            val = qcdEstimate_mc.GetBinContent(i)
            err = qcdEstimate_mc.GetBinError(i)
            qcdEstimate_mc.SetBinError(i, np.sqrt((err/val)**2+fj_mc_relerr**2+rb_mc_err**2)*val if val!=0 else 0)
        
        d = qcd_estimate_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        qcdEstimate_data.Write()
        
        d = qcd_estimate_mc_dir.mkdir("{0}_{1}".format(ht_reg.replace("ht","HT"),top_reg))
        d.cd()
        qcdEstimate_mc.Write()

fout.Close()
        
