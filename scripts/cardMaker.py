import ROOT
import re
from sys import argv,exit

verbose = False
suppressZeroBins = False
suppressZeroTRs = True
doSuperSignalRegions = False
dummy_alpha = 1
uncorrelatedZGratio = False
fourNuisancesPerBinZGratio = True
integratedZinvEstimate = True
doDummySignalSyst = False
subtractSignalContam = True
doZinvFromDY = True
decorrelatedLostlepNuisances = False
doSimpleLostlepNuisances = False
printTable = False
suppressUHmt2bin = True

last_zinv_ratio = 0.5
last_lostlep_transfer = 2.0
last_zinvDY_transfer = 2.0

if len(argv) < 3:
    print "Usage: {0} [signal file name] [input directory] [output directory] [doScan = True] [doData = True]".format(argv[0])
    exit(1)

signal = argv[1]
indir = argv[2]
outdir = argv[3]
doScan = True
doData = True
if len(argv) > 4:
    if argv[4] == "0" or argv[4].lower() == "false": doScan = False
    if len(argv) > 5:
        if argv[5] == "0" or argv[4].lower() == "false": doData = False

bmark = ROOT.TBenchmark()
bmark.Start("benchmark")

f_lostlep = ROOT.TFile("{0}/lostlepFromCRs.root".format(indir))
f_zinv = ROOT.TFile("{0}/zinvFromGJ.root".format(indir))
f_zinvDY = ROOT.TFile("{0}/zinvFromDY.root".format(indir))
f_zgratio = ROOT.TFile("{0}/doubleRatio.root".format(indir))
f_purity = ROOT.TFile("{0}/purity.root".format(indir))
f_qcd = ROOT.TFile("{0}/qcdFromCRs.root".format(indir))
f_sig = ROOT.TFile("{0}/{1}.root".format(indir,signal))
if (doData): f_data = ROOT.TFile("{0}/data_Run2016.root".format(indir))

if f_lostlep.IsZombie():
    print "lostlepFromCRs.root does not exist\n"
    exit(1)
if f_zinv.IsZombie():
    print "zinvFromGJ.root does not exist\n"
    exit(1)
if f_zinvDY.IsZombie():
    print "zinvFromDY.root does not exist\n"
    exit(1)
if f_zgratio.IsZombie():
    print "doubleRatio.root does not exist\n"
    exit(1)
if f_purity.IsZombie(): 
    print "purity.root does not exist\n"
    exit(1)
if f_qcd.IsZombie():
    print "qcdFromCRs.root does not exist\n"
    exit(1)
if f_sig.IsZombie():
    print "{0}.root does not exist\n".format(signal)
    exit(1)
if (doData and f_data.IsZombie()):
    print "data_Run2016.root does not exist\n"
    print "Perhaps you wanted to set doData = False or forgot to hadd?"
    exit(1)

def makeTemplate(directory,imt2):
    if verbose: print "Forming template for region {0}, bin {1}".format(directory,imt2)
    dir_sig = f_sig.Get(directory)
    dir_lostlep = f_lostlep.Get(directory)
    dir_zinv = f_zinv.Get(directory)
    dir_zinvDY = f_zinvDY.Get(directory)
    dir_zgratio = f_zgratio.Get(directory)
    dir_purity = f_purity.Get(directory)
    dir_qcd = f_qcd.Get(directory)

    if (dir_sig is None):
        print "dir {0} does not exist in f_sig. Aborting...".format(directory)
        exit(1)

    fullhistname = "{0}/h_mt2bins".format(directory)
    fullhistnameGenMet = "{0}_genmet".format(fullhistname)
    fullhistnameBtagsfHeavy = "{0}_btagsf_heavy_UP".format(fullhistname)
    fullhistnameBtagsfLight = "{0}_btagsf_light_UP".format(fullhistname)
    fullhistnameLepeff = "{0}_lepeff_UP".format(fullhistname)
    fullhistnameIsr = "{0}_isr_UP".format(fullhistname)
    fullhistnameScan = "{0}_sigscan".format(fullhistname)
    fullhistnameScanGenMet = "{0}_sigscan_genmet".format(fullhistname)
    fullhistnameScanBtagsfHeavy = "{0}_sigscan_btagsf_heavy_UP".format(fullhistname)
    fullhistnameScanBtagsfLight = "{0}_sigscan_btagsf_light_UP".format(fullhistname)
    fullhistnameScanLepeff = "{0}_sigscan_lepeff_UP".format(fullhistname)
    fullhistnameScanIsr = "{0}_sigscan_isr_UP".format(fullhistname)
    fullhistnameStat = "{0}Stat".format(fullhistname)
    fullhistnameMCStat = "{0}MCStat".format(fullhistname)
    fullhistnameCRyield = "{0}CRyield".format(fullhistname)
    fullhistnameCRyieldDatacard = "{0}CRyieldDatacard".format(fullhistname)
    fullhistnameLastbinHybrid = "{0}/h_lastbinHybrid".format(directory)
    fullhistnameMCExtrap = "{0}/h_mt2binsMCExtrap".format(directory)
    fullhistnameRatio = "{0}Ratio".format(fullhistname)
    fullhistnameRatioInt = "{0}RatioInt".format(fullhistname)
    fullhistnamePurity = "{0}/h_mt2binspurityFailSieieData".format(directory)
    fullhistnamePurityInt = "{0}/h_mt2binspurityIntFailSieieData".format(directory)
    fullhistnameAlpha = "{0}Alpha".format(fullhistname)
    fullhistnameFJRB = "{0}FJRBsyst".format(fullhistname)
    fullhistnameFitStat = "{0}FitStat".format(fullhistname)
    fullhistnameFitSyst = "{0}FitSyst".format(fullhistname)
    crsl_directory = directory.replace("sr","crsl")
    fullhistnameCRSL = "{0}/h_mt2bins".format(crsl_directory)
    fullhistnameCRSLGenMet = "{0}/h_mt2bins_genmet".format(crsl_directory)
    fullhistnameCRSLScan = "{0}_sigscan".format(fullhistnameCRSL)
    fullhistnameCRSLScanGenMet = "{0}_sigscan_genmet".format(fullhistnameCRSL)

    isSignalWithLeptons = signal.find("T1tttt") != -1 or signal.find("T2tt") != -1
    
    n_lostlep = 0.0
    n_lostlep_cr = 0.0
    lostlep_alpha = 0.0
    lostlep_alpha_topological = 0.0
    lostlep_alpa_lepeff_UP = 0.0
    lostlep_alpa_lepeff_DN = 0.0
    lostlep_alpha_btagsf_heavy_UP = 0.0
    lostlep_alpha_btagsf_heavy_DN = 0.0
    lostlep_alpha_btagsf_light_UP = 0.0
    lostlep_alpha_btagsf_light_DN = 0.0
    lostlep_alpha_tau1p_UP = 0.0
    lostlep_alpha_tau1p_DN = 0.0
    lostlep_alpha_tau3p_UP = 0.0
    lostlep_alpha_tau3p_DN = 0.0
    lostlep_alpha_renorm_UP = 0.0
    lostlep_alpha_renorm_DN = 0.0
    lostlep_MCExtrap = 0.0
    err_lostlep_mcstat = 0.0
    n_zinv = 0.0
    n_zinv_cr = 0.0
    err_zinv_mcstat = 0.0
    zinv_ratio_zg = 0.0
    zinv_purity = 0.0
    err_zinv_purity = 0.0
    n_qcd = 0.0
    n_qcd_cr = 0.0
    qcd_alpha = 0.0
    err_qcd_alpha = 0.0
    err_qcd_fjrb = 0.0
    err_qcd_fitstat = 0.0
    err_qcd_fitsyst = 0.0
    n_bkg = 0.0
    n_data = 0.0
    lostlep_lastbin_hybrid = 1
    n_zinvDY = 0.0
    n_zinvDY_cr = 0.0
    zinvDY_alpha = 0.0
    zinvDY_alpha_topological = 0.0
    zinvDY_purity = 0.0
    err_zinvDY_mcstat = 0.0
    err_zinvDY_purity = 0.0
    usingInclusiveTemplates = True
    zinvDY_lastbin_hybrid = 1

    # Get various boundaries from histograms in the signal file
    ht_LOW_name = directory + "/h_ht_LOW"
    ht_HI_name = directory + "/h_ht_HI"
    h_ht_LOW = f_sig.Get(ht_LOW_name)
    h_ht_HI = f_sig.Get(ht_HI_name)
    ht_LOW = int(h_ht_LOW.GetBinContent(1))
    ht_HI = int(h_ht_HI.GetBinContent(1))
    nbjets_LOW_name = directory + "/h_nbjets_LOW"
    nbjets_HI_name = directory + "/h_nbjets_HI"
    h_nbjets_LOW = f_sig.Get(nbjets_LOW_name)
    h_nbjets_HI = f_sig.Get(nbjets_HI_name)
    nbjets_LOW = int(h_nbjets_LOW.GetBinContent(1))
    nbjets_HI = int(h_nbjets_HI.GetBinContent(1))
    njets_LOW_name = directory + "/h_njets_LOW"
    njets_HI_name = directory + "/h_njets_HI"
    h_njets_LOW = f_sig.Get(njets_LOW_name)
    h_njets_HI = f_sig.Get(njets_HI_name)
    njets_LOW = int(h_njets_LOW.GetBinContent(1))
    njets_HI = int(h_njets_HI.GetBinContent(1))

    # If we're doing a scan over mass points, we need to extract any 2D histogram from the all-inclusive 
    # 3D histogram to read MT2 boundaries.
    # If we've got only a single mass point, no projection procedure is needed (and attempting it will fail).
    if (doScan):
        h_sigscan = f_sig.Get(fullhistnameScan)
        h_sig = h_sigscan.ProjectionX("h_sig",0,0,0,0) # bin 0 is guaranteed to exist, so it's suitable as a dummy projection
    else:
        h_sig = f_sig.Get(fullhistname)

    mt2_LOW = int(h_sig.GetBinLowEdge(imt2))
    mt2_HI = mt2_LOW + int(h_sig.GetBinWidth(imt2))
    # The uppermost bin actually extends to infinity, which we represent with -1 for now.
    # TODO: Update these hardcoded values to their new values if they should change
    if (mt2_HI == 1800 or mt2_HI == 1500): mt2_HI = -1

    nbjets_HI_mod = nbjets_HI
    njets_HI_mod = njets_HI
    # The upper limit (if it's finite) is exclusive, but the descriptive string is inclusive. *_mod will be used to make strings....
    if (nbjets_HI != -1): nbjets_HI_mod -= 1
    if (njets_HI != -1): njets_HI_mod -= 1

    ht_str = "HT{0}to{1}".format(str(ht_LOW),str(ht_HI))
    if (nbjets_LOW != nbjets_HI_mod):
        bjet_str = "b{0}to{1}".format(str(nbjets_LOW),str(nbjets_HI_mod))
    else:
        bjet_str = "b{0}".format(str(nbjets_LOW))
    if (njets_LOW != njets_HI_mod):
        jet_str = "j{0}to{1}".format(str(njets_LOW),str(njets_HI_mod))
    else:
        jet_str = "j{0}".format(str(njets_LOW))
    mt2_str = "m{0}to{1}".format(str(mt2_LOW),str(mt2_HI))

    ht_str = ht_str.replace("-1","Inf")
    bjet_str = bjet_str.replace("-1","Inf")
    jet_str = jet_str.replace("-1","Inf")
    mt2_str = mt2_str.replace("-1","Inf")

    topologicalR = "{0}_{1}_{2}".format(ht_str,jet_str,bjet_str)
    channel = "{0}_{1}".format(topologicalR,mt2_str)

    ht_LOW_crsl = 0
    ht_HI_crsl = 0
    nbjets_LOW_crsl = 0
    nbjets_HI_crsl = 0
    njets_LOW_crsl = 0
    njets_HI_crsl = 0
    if (dir_lostlep is not None):
        h_ht_LOW_crsl = f_lostlep.Get(ht_LOW_name)
        h_ht_HI_crsl = f_lostlep.Get(ht_HI_name)
        ht_LOW_crsl = int(h_ht_LOW_crsl.GetBinContent(1))
        ht_HI_crsl = int(h_ht_HI_crsl.GetBinContent(1))

        h_nbjets_LOW_crsl = f_lostlep.Get(nbjets_LOW_name)
        h_nbjets_HI_crsl = f_lostlep.Get(nbjets_HI_name)
        nbjets_LOW_crsl = int(h_nbjets_LOW_crsl.GetBinContent(1))
        nbjets_HI_crsl = int(h_nbjets_HI_crsl.GetBinContent(1))

        h_njets_LOW_crsl = f_lostlep.Get(njets_LOW_name)
        h_njets_HI_crsl = f_lostlep.Get(njets_HI_name)
        njets_LOW_crsl = int(h_njets_LOW_crsl.GetBinContent(1))
        njets_HI_crsl = int(h_njets_HI_crsl.GetBinContent(1))

    nbjets_HI_crsl_mod = nbjets_HI_crsl
    njets_HI_crsl_mod = njets_HI_crsl
    if (nbjets_HI_crsl != -1): nbjets_HI_crsl_mod -= 1
    if (njets_HI_crsl != -1): njets_HI_crsl_mod -= 1

    ht_str_crsl = "HT{0}to{1}".format(str(ht_LOW_crsl),str(ht_HI_crsl))
    if (nbjets_HI_crsl != nbjets_HI_crsl_mod):
        bjet_str_crsl = "b{0}to{1}".format(str(nbjets_LOW_crsl),str(nbjets_HI_crsl_mod))
    else:
        bjet_str_crsl = "b{0}".format(str(nbjets_LOW_crsl))
    if (njets_HI_crsl != njets_HI_crsl_mod):
        jet_str_crsl = "j{0}to{1}".format(str(njets_LOW_crsl),str(njets_HI_crsl_mod))
    else:
        jet_str_crsl = "j{0}".format(str(njets_LOW_crsl))

    ht_str_crsl = ht_str_crsl.replace("-1","Inf")
    bjet_str_crsl = bjet_str_crsl.replace("-1","Inf")
    jet_str_crsl = jet_str_crsl.replace("-1","Inf")

    if (dir_lostlep is not None):
        h_lostlep = f_lostlep.Get(fullhistname)
        if (h_lostlep is not None): n_lostlep = h_lostlep.GetBinContent(imt2)
        h_lostlep_mcstat = f_lostlep.Get(fullhistnameMCStat)
        if (h_lostlep_mcstat is not None):
            mcstat_content = h_lostlep_mcstat.GetBinContent(imt2)
            if (mcstat_content != 0): 
                err_lostlep_mcstat = h_lostlep_mcstat.GetBinError(imt2) / mcstat_content       
        h_lostlep_cryield = f_lostlep.Get(fullhistnameCRyieldDatacard)
        if (h_lostlep_cryield is not None):
            n_lostlep_cr = round(h_lostlep_cryield.GetBinContent(imt2))
        h_lostlep_alpha = f_lostlep.Get(fullhistnameAlpha)
        if (h_lostlep_alpha is not None):
            lostlep_alpha = h_lostlep_alpha.GetBinContent(imt2)
            lostlep_alpha_topological = h_lostlep_alpha.Integral(0,-1)
        h_lostlep_alpha_lepeff_UP = f_lostlep.Get(fullhistnameAlpha+"_lepeff_UP")
        if (h_lostlep_alpha_lepeff_UP is not None):
            lostlep_alpha_lepeff_UP = h_lostlep_alpha_lepeff_UP.Integral(0,-1)
        h_lostlep_alpha_lepeff_DN = f_lostlep.Get(fullhistnameAlpha+"_lepeff_DN")
        if (h_lostlep_alpha_lepeff_DN is not None):
            lostlep_alpha_lepeff_DN = h_lostlep_alpha_lepeff_DN.Integral(0,-1)
        h_lostlep_alpha_btagsf_heavy_UP = f_lostlep.Get(fullhistnameAlpha+"_btagsf_heavy_UP")
        if (h_lostlep_alpha_btagsf_heavy_UP is not None):
            lostlep_alpha_btagsf_heavy_UP = h_lostlep_alpha_btagsf_heavy_UP.Integral(0,-1)
        h_lostlep_alpha_btagsf_heavy_DN = f_lostlep.Get(fullhistnameAlpha+"_btagsf_heavy_DN")
        if (h_lostlep_alpha_btagsf_heavy_DN is not None):
            lostlep_alpha_btagsf_heavy_DN = h_lostlep_alpha_btagsf_heavy_DN.Integral(0,-1)
        h_lostlep_alpha_btagsf_light_UP = f_lostlep.Get(fullhistnameAlpha+"_btagsf_light_UP")
        if (h_lostlep_alpha_btagsf_light_UP is not None):
            lostlep_alpha_btagsf_light_UP = h_lostlep_alpha_btagsf_light_UP.Integral(0,-1)
        h_lostlep_alpha_btagsf_light_DN = f_lostlep.Get(fullhistnameAlpha+"_btagsf_light_DN")
        if (h_lostlep_alpha_btagsf_light_DN is not None):
            lostlep_alpha_btagsf_light_DN = h_lostlep_alpha_btagsf_light_DN.Integral(0,-1)
        h_lostlep_alpha_tau1p_UP = f_lostlep.Get(fullhistnameAlpha+"_tau1p_UP")
        if (h_lostlep_alpha_tau1p_UP is not None):
            lostlep_alpha_tau1p_UP = h_lostlep_alpha_tau1p_UP.Integral(0,-1)
        h_lostlep_alpha_tau1p_DN = f_lostlep.Get(fullhistnameAlpha+"_tau1p_DN")
        if (h_lostlep_alpha_tau1p_DN is not None):
            lostlep_alpha_tau1p_DN = h_lostlep_alpha_tau1p_DN.Integral(0,-1)
        h_lostlep_alpha_tau3p_UP = f_lostlep.Get(fullhistnameAlpha+"_tau3p_UP")
        if (h_lostlep_alpha_tau3p_UP is not None):
            lostlep_alpha_tau3p_UP = h_lostlep_alpha_tau3p_UP.Integral(0,-1)
        h_lostlep_alpha_tau3p_DN = f_lostlep.Get(fullhistnameAlpha+"_tau3p_DN")
        if (h_lostlep_alpha_tau3p_DN is not None):
            lostlep_alpha_tau3p_DN = h_lostlep_alpha_tau3p_DN.Integral(0,-1)
        h_lostlep_alpha_renorm_UP = f_lostlep.Get(fullhistnameAlpha+"_renorm_UP")
        if (h_lostlep_alpha_renorm_UP is not None):
            lostlep_alpha_renorm_UP = h_lostlep_alpha_renorm_UP.Integral(0,-1)
        h_lostlep_alpha_renorm_DN = f_lostlep.Get(fullhistnameAlpha+"_renorm_DN")
        if (h_lostlep_alpha_renorm_DN is not None):
            lostlep_alpha_renorm_DN = h_lostlep_alpha_renorm_DN.Integral(0,-1)
        h_lostlep_lastbin_hybrid = f_lostlep.Get(fullhistnameLastbinHybrid)
        if (h_lostlep_lastbin_hybrid is not None):
            lostlep_lastbin_hybrid = h_lostlep_lastbin_hybrid.GetBinContent(1)
        h_lostlep_MCExtrap = f_lostlep.Get(fullhistnameMCExtrap)
        if (h_lostlep_MCExtrap is not None):
            lostlep_MCExtrap = h_lostlep_MCExtrap.GetBinContent(imt2)

    # lepeff
    lostlep_alpha_lepeff_ERRup = abs(1.0-lostlep_alpha_lepeff_UP/lostlep_alpha_topological)
    lostlep_alpha_lepeff_ERRdn = abs(1.0-lostlep_alpha_lepeff_DN/lostlep_alpha_topological)
    lostlep_alpha_lepeff_ERR = max(lostlep_alpha_lepeff_ERRup, lostlep_alpha_lepeff_ERRdn)

    # btagsf
    lostlep_alpha_btagsf_heavy_ERRup = abs(1.0-lostlep_alpha_btagsf_heavy_UP/lostlep_alpha_topological)
    lostlep_alpha_btagsf_heavy_ERRdn = abs(1.0-lostlep_alpha_btagsf_heavy_DN/lostlep_alpha_topological)
    lostlep_alpha_btagsf_heavy_ERR = max(lostlep_alpha_btagsf_heavy_ERRup, lostlep_alpha_btagsf_heavy_ERRdn)
    lostlep_alpha_btagsf_light_ERRup = abs(1.0-lostlep_alpha_btagsf_light_UP/lostlep_alpha_topological)
    lostlep_alpha_btagsf_light_ERRdn = abs(1.0-lostlep_alpha_btagsf_light_DN/lostlep_alpha_topological)
    lostlep_alpha_btagsf_light_ERR = max(lostlep_alpha_btagsf_light_ERRup, lostlep_alpha_btagsf_light_ERRdn)
    lostlep_alpha_btagsf_ERR = max(lostlep_alpha_btagsf_heavy_ERR, lostlep_alpha_btagsf_light_ERR)

    # tau
    lostlep_alpha_tau1p_ERRup = abs(1.0-lostlep_alpha_tau1p_UP/lostlep_alpha_topological)
    lostlep_alpha_tau1p_ERRdn = abs(1.0-lostlep_alpha_tau1p_DN/lostlep_alpha_topological)
    lostlep_alpha_tau1p_ERR = max(lostlep_alpha_tau1p_ERRup, lostlep_alpha_tau1p_ERRdn)
    lostlep_alpha_tau3p_ERRup = abs(1.0-lostlep_alpha_tau3p_UP/lostlep_alpha_topological)
    lostlep_alpha_tau3p_ERRdn = abs(1.0-lostlep_alpha_tau3p_DN/lostlep_alpha_topological)
    lostlep_alpha_tau3p_ERR = max(lostlep_alpha_tau3p_ERRup, lostlep_alpha_tau3p_ERRdn)
    lostlep_alpha_tau_ERR = max(lostlep_alpha_tau1p_ERR, lostlep_alpha_tau3p_ERR)

    # renorm
    lostlep_alpha_renorm_ERRup = abs(1.0-lostlep_alpha_renorm_UP/lostlep_alpha_topological)
    lostlep_alpha_renorm_ERRdn = abs(1.0-lostlep_alpha_renorm_DN/lostlep_alpha_topological)
    lostlep_alpha_renorm_ERR = max(lostlep_alpha_renorm_ERRup, lostlep_alpha_renorm_ERRdn)

    # MC extrapolation (MT2 shape)
    lostlep_shape_ERR = 0.0
    if (n_lostlep > 0.0): lostlep_shape_ERR = lostlep_MCExtrap / n_lostlep

    n_mt2bins = 1
    h_zinv = None
    if (dir_zinv is not None):
        h_zinv = f_zinv.Get(fullhistname)
        if (h_zinv is not None):
            n_zinv = h_zinv.GetBinContent(imt2)
            n_mt2bins = h_zinv.GetNbinsX()
        h_zinv_mcstat = None
        if (integratedZinvEstimate):
            h_zinv_mcstat = f_zinv.Get(fullhistnameRatioInt)
        else:
            h_zinv_mcstat = f_zinv.Get(fullhistnameRatio)
        if (h_zinv_mcstat is not None):
            mcstat_content = h_zinv_mcstat.GetBinContent(imt2)
            if (mcstat_content != 0):
                err_zinv_mcstat = h_zinv_mcstat.GetBinError(imt2) / mcstat_content
                zinv_ratio_zg = mcstat_content
                last_zinv_ratio = zinv_ratio_zg
        else:
            err_zinv_mcstat = 1.0
            zinv_ratio_zg = last_zinv_ratio
            
        # h_gjetyield = f_zinv.Get(fullhistnameCRyield)

    if (dir_purity is not None):
        h_zinv_cryield = f_purity.Get(fullhistname)
        if (h_zinv_cryield is not None):
            if (integratedZinvEstimate):
                n_zinv_cr = round(h_zinv_cryield.Integral(0,-1))
            else:
                n_zinv_cr = round(h_zinv_cryield.GetBinContent(imt2))

        h_zinv_purity = None
        
        zinv_purity = 1.0
        mt2bin_tmp = imt2
        if (integratedZinvEstimate):
            h_zinv_purity = f_purity.Get(fullhistnamePurityInt)
            mt2bin_tmp = 1
        else:
            h_zinv_purity = f_purity.Get(fullhistnamePurity)

        if (h_zinv_purity is not None):
            purity_content = h_zinv_purity.GetBinContent(mt2bin_tmp)
            if (purity_content != 0):
                zinv_purity = purity_content
                err_zinv_purity = h_zinv_purity.GetBinContent(mt2bin_tmp) / purity_content

    # Zinv from DY
    h_zinvDY = None
    if (dir_zinvDY is not None):
        h_zinvDY = f_zinvDY.Get(directory+"/hybridEstimate")
        purityCard = f_zinvDY.Get(directory+"/purityCard")
        ratioCard = f_zinvDY.Get(directory+"/ratioCard")
        h_zinvDY_cryield = f_zinvDY.Get(directory+"/CRyieldCard")
        if (h_zinvDY is not None):
            n_mt2bins = h_zinvDY.GetNbinsX()
            n_zinvDY = h_zinvDY.GetBinContent(imt2)
        if (ratioCard is not None):
            ratio_content = ratioCard.GetBinContent(imt2)
            if (ratio_content != 0):
                err_zinvDY_mcstat = ratioCard.GetBinError(imt2) / ratio_content
        if (h_zinvDY_cryield is not None):
            n_zinvDY_cr = round(h_zinvDY_cryield.GetBinContent(imt2))
            crYieldFirstBin = -1
            for ibin in range(1, h_zinvDY_cryield.GetNbinsX()):
                thiscrYield = round(h_zinvDY_cryield.GetBinContent(ibin))
                if (ibin == 1): crYieldFirstBin = thiscrYield
                if (crYieldFirstBin != thiscrYield):
                    usingInclusiveTemplates = False
                    break
        h_zinvDY_alpha = ratioCard.Clone()
        if (h_zinvDY_alpha is not None):
            if (purityCard is not None):
                zinvDY_purity = purityCard.GetBinContent(imt2)
                h_zinvDY_alpha.Multiply(purityCard)
                purity_content = purityCard.GetBinContent(imt2)
                if (purity_content > 0):
                    err_zinvDY_purity = purityCard.GetBinError(imt2) / purity_content
                else:
                    err_zinvDY_purity = 0
            zinvDY_alpha = h_zinvDY_alpha.GetBinContent(imt2)
            zinvDY_alpha_topological = h_zinvDY_alpha.Integral(0,-1)
        h_zinvDY_lastbin_hybrid = f_zinvDY.Get(fullhistnameLastbinHybrid)
        if (h_zinvDY_lastbin_hybrid is not None):
            zinvDY_lastbin_hybrid = h_zinvDY_lastbin_hybrid.GetBinContent(1)

    zllgamma_nj = 1.0
    zllgamma_nb = 1.0
    zllgamma_ht = 1.0
    zllgamma_ht2 = 1.0
    zllgamma_mt2 = 1.0
    notFound = ""
    if (fourNuisancesPerBinZGratio):
        h_zllgamma_nj = f_zgratio.Get("h_njbinsRatio")
        h_zllgamma_nb = f_zgratio.Get("h_nbjbinsRatio")
        h_zllgamma_ht = f_zgratio.Get("h_htbinsRatio")
        h_zllgamma_ht2 = f_zgratio.Get("h_htbins2Ratio")
        h_zllgamma_mt2 = f_zgratio.Get("h_mt2binsRatio")
        if (h_zllgamma_nj is None or h_zllgamma_nb is None or h_zllgamma_ht is None or h_zllgamma_ht2 is None or h_zllgamma_mt2 is None):
            print "Trying fourNuisancesPerBinZGratio, but could not find inclusive Zll/Gamma ratio plots for nuisance parameters"
            exit(1)
        bin_nj = h_zllgamma_nj.FindBin(njets_LOW + 0.5)
        bin_nb = h_zllgamma_nb.FindBin(nbjets_LOW + 0.5)
        if (nbjets_LOW >= 3): bin_nb = h_zllgamma_nb.FindBin(nbjets_LOW - 0.5)
        bin_ht = h_zllgamma_ht.FindBin(ht_LOW + 1)
        bin_ht2 = h_zllgamma_ht2.FindBin(ht_LOW + 1)
        bin_mt2 = h_zllgamma_mt2.FindBin(mt2_LOW + 1)
        zllgamma_nj = 1.0
        nj_content = h_zllgamma_nj.GetBinContent(bin_nj)
        if (nj_content > 0):
            zllgamma_nj = h_zllgamma_nj.GetBinError(bin_nj) / nj_content
        zllgamma_nb = 1.0
        nb_content = h_zllgamma_nb.GetBinContent(bin_nb)
        if (nb_content > 0):
            zllgamma_nb = h_zllgamma_nb.GetBinError(bin_nb) / nb_content
        if (nbjets_LOW >= 3): zllgamma_nb *= 2
        zllgamma_ht = 1.0
        ht_content = h_zllgamma_ht.GetBinContent(bin_ht)
        if (ht_content > 0):
            zllgamma_ht = h_zllgamma_ht.GetBinError(bin_ht) / ht_content
        zllgamma_ht2 = 1.0
        ht2_content = h_zllgamma_ht2.GetBinContent(bin_ht2)
        if (ht2_content > 0):
            zllgamma_ht2 = h_zllgamma_ht2.GetBinError(bin_ht2) / ht2_content
        zllgamma_mt2 = 1.0
        mt2_content = h_zllgamma_mt2.GetBinContent(bin_mt2)
        if (mt2_content > 0):
            zllgamma_mt2 = h_zllgamma_mt2.GetBinError(bin_mt2) / mt2_content
        
    if (dir_qcd is not None):
        h_qcd = f_qcd.Get(fullhistname)
        if (h_qcd is not None):
            n_qcd = h_qcd.GetBinContent(imt2)
        h_qcd_cryield = f_qcd.Get(fullhistnameCRyield)
        if (h_qcd_cryield is not None):
            n_qcd_cr = round(h_qcd_cryield.GetBinContent(imt2))
        h_qcd_alpha = f_qcd.Get(fullhistnameAlpha)
        if (h_qcd_alpha is not None):
            qcd_alpha = h_qcd_alpha.GetBinContent(imt2)
            err_qcd_alpha = h_qcd_alpha.GetBinError(imt2)
        h_qcd_fjrb = f_qcd.Get(fullhistnameFJRB)
        if (h_qcd_fjrb is not None):
            err_qcd_fjrb = h_qcd_fjrb.GetBinContent(imt2)
        h_qcd_fitstat = f_qcd.Get(fullhistnameFitStat)
        if (h_qcd_fitstat is not None):
            err_qcd_fitstat = h_qcd_fitstat.GetBinContent(imt2)
        h_qcd_fitsyst = f_qcd.Get(fullhistnameFitSyst)
        if (h_qcd_fitsyst is not None):
            err_qcd_fitsyst = h_qcd_fitsyst.GetBinContent(imt2)

    # Finalize Errors
    n_syst = 0
    lostlep_shape = lostlep_shape_ERR
    lostlep_mcstat = 1.0 + err_lostlep_mcstat
    lostlep_lepeff = 1.0 + lostlep_alpha_lepeff_ERR
    lostlep_mtcut = 1.03
    lostlep_taueff = 1.0 + lostlep_alpha_tau_ERR
    lostlep_btageff = 1.0 + lostlep_alpha_btagsf_ERR
    lostlep_jec = 1.02
    lostlep_renorm = 1.0 + lostlep_alpha_renorm_ERR
    lostlep_alphaerr = 1.10

    name_lostlep_shape = "llep_shape_{0}_{1}_{2}".format(ht_str_crsl,jet_str_crsl,bjet_str_crsl)
    name_lostlep_crstat = "llep_CRstat_{0}_{1}_{2}".format(ht_str_crsl,jet_str_crsl,bjet_str_crsl)
    name_lostlep_alphaerr = "llep_alpha_{0}_{1}_{2}".format(ht_str_crsl,jet_str_crsl,bjet_str_crsl)
    if (n_mt2bins > 1 and imt2 >= lostlep_lastbin_hybrid):
        n_syst += 1 # for lostlep shape
    else:
        name_lostlep_crstat = "llep_CRstat_{0}".format(channel)

    # nuisances decorrelated across all bins
    name_lostlep_mcstat = "llep_MCstat_{0}".format(channel)
    
    llep_corr_str = ""
    if (decorrelatedLostlepNuisances):
        llep_corr_str = "_" + channel
    # nuisances correlated across all bins unless explicitly decorrelated by above
    name_lostlep_lepeff = "lep_eff{0}".format(llep_corr_str)
    name_lostlep_taueff = "llep_taueff{0}".format(llep_corr_str)
    name_lostlep_btageff = "llep_btageff{0}".format(llep_corr_str)
    name_lostlep_jec = "jec{0}".format(llep_corr_str)
    name_lostlep_renorm = "llep_renorm{0}".format(llep_corr_str)
    name_lostlep_mtcut = "llep_mtcut{0}".format(llep_corr_str)

    if (doSimpleLostlepNuisances):
        name_lostlep_lepeff = "llep_lepeff_{0}_{1}_{2}".format(ht_str_crsl,jet_str_crsl,bjet_str_crsl)
        lostlep_lepeff = 1.12
        if (njets_LOW == 7 and nbjets_LOW >= 3):
            lostlep_alphaerr = 1.18

    if (n_lostlep_cr > 0.0):
        if (lostlep_alpha > 3.0):
            lostlep_alpha = 3.0
            n_lostlep = n_lostlep_cr * lostlep_alpha
    if (lostlep_alpha > 0.0):
        last_lostlep_transfer = lostlep_alpha
    elif (n_lostlep == 0):
        lostlep_alpha = 0
    else: 
        lostlep_alpha = last_lostlep_transfer
    if (doSimpleLostlepNuisances):
        n_syst += 4
    else:
        n_syst += 8

    perChannel = channel
    perTopoRegion = topologicalR

    zinv_suffix = perChannel
    if (integratedZinvEstimate):
        zinv_suffix = perTopoRegion

    name_zinv_crstat = "zinv_CRstat_{0}".format(zinv_suffix)
    name_zinv_alphaerr = "zinv_alphaErr_{0}".format(zinv_suffix)
    name_zinv_purityerr = "zinv_purity_{0}".format(zinv_suffix)
    name_zinv_puritysyst = "zinv_puritySyst_{0}".format(zinv_suffix)
    name_zinv_zgamma = "zinv_ZGratio"
    if (uncorrelatedZGratio):
        name_zinv_zgamma += "_{0}".format(channel)
    name_zinv_zgamma_nj = "zinv_ZGratio_{0}".format(jet_str)
    name_zinv_zgamma_nb = "zinv_ZGratio_{0}".format(bjet_str)
    name_zinv_zgamma_ht = "zinv_ZGratio_{0}".format(ht_str)
    name_zinv_zgamma_mt2 = "zinv_ZGratio_m{0}".format(mt2_LOW)
    name_zinv_doubleRatioOffset = "zinv_doubleRatioOffset"
    name_zinv_mcsyst = "zinv_MC_{0}".format(channel)
    name_zinv_shape = "zinv_shape_{0}".format(perTopoRegion)

    zinv_alpha = 1.0
    zinv_alphaerr = 1.0 + err_zinv_mcstat
    zinv_purityerr = 1.0 + err_zinv_purity
    zinv_puritysyst = 1.10
    zinv_doubleRatioOffset = 1.11
    zinv_zgamma = -1
    zinv_zgamma_nj = 1.0
    zinv_zgamma_nb = 1.0
    zinv_zgamma_ht = 1.0
    zinv_zgamma_mt2 = 1.0
    zinv_mcsyst = -1
    zinv_shape = 1.0

    name_zinvDY_alphaErr = "zinvDY_alphaErr_{0}".format(channel)
    name_zinvDY_purity = "zinvDY_purity_{0}".format(perTopoRegion)
    name_zinvDY_rsfof = "zinvDY_Rsfof"
    name_zinvDY_shape = "zinvDY_shape_{0}".format(perTopoRegion)
    name_zinvDY_crstat = "zinvDY_CRstat_{0}".format(perTopoRegion)
    if (imt2 < zinvDY_lastbin_hybrid and not usingInclusiveTemplates):
        name_zinvDY_crstat = "zinvDY_CRstat_{0}".format(channel)
    zinvDY_shape = 1.0
    zinvDY_puritystat = 1.0 + err_zinvDY_purity
    zinvDY_rsfof = 1.0 + (1-zinvDY_purity)*0.15
    zinvDY_alphaErr = 1.0 + err_zinvDY_mcstat
    zinvDY_lepeff = 1.05
    zinvDY_jec = 1.02
    if (ht_HI == 450):
        zinvDY_jec = 1.05

    if (n_zinvDY_cr > 0.0):
        if (zinvDY_alpha > 10.0):
            zinvDY_alpha = 10.0
            n_zinvDY = n_zinvDY_cr * zinvDY_alpha
    if (zinvDY_alpha > 0.0):
        last_zinvDY_transfer = zinvDY_alpha
    elif (n_zinvDY == 0):
        zinvDY_alpha = 0
    else:
        zinvDY_alpha = last_zinvDY_transfer
        
    if (zinvDY_alpha != n_zinvDY / n_zinvDY_cr and n_zinvDY_cr != 0):
        zinvDY_alpha = n_zinvDY / n_zinvDY_cr

    n_syst += 6

    last_bin_relerr_zinvDY = 0.4
    n_extrap_bins_zinvDY = n_mt2bins - zinvDY_lastbin_hybrid
    if (n_extrap_bins_zinvDY > 0 and imt2 >= zinvDY_lastbin_hybrid):
        if (imt2 == zinvDY_lastbin_hybrid and n_zinvDY > 0.0):
            increment = 0.0
            for ibin in range(zinvDY_lastbin_hybrid+1,h_zinvDY.GetNbinsX()):
                increment += last_bin_relerr_zinvDY / n_extrap_bins_zinvDY * (ibin - zinvDY_lastbin_hybrid) * h_zinvDY.GetBinContent(ibin)
            zinvDY_shape = 1.0 - increment / n_zinvDY
        else:
            zinvDY_shape = 1.0 + last_bin_relerr_zinvDY / (n_extrap_bins_zinvDY) * (imt2 -  zinvDY_lastbin_hybrid)
        n_syst += 1

    if (not integratedZinvEstimate and nbjets_LOW >= 2):
        zinv_mcsyst = 2.0
        n_syst += 1
    else:
        zinv_alpha = zinv_ratio_zg * zinv_purity * 0.92
        if (f_data):
            zinv_alpha *= 0.93
        if (zinv_alpha > 0.5):
            zinv_alpha = 0.5
        n_syst += 5
        if (integratedZinvEstimate and n_mt2bins > 1):
            if (imt2 == 1 and n_zinv > 0):
                increment = 0.0
                for ibin in range(2,h_zinv.GetNbinsX()):
                    increment += 0.4 / (n_mt2bins - 1) * (ibin - 1) * h_zinv.GetBinContent(ibin)
                zinv_shape = 1.0 - increment / n_zinv
            else:
                zinv_shape = 1.0 + 0.4 / (n_mt2bins - 1) * (imt2 - 1)
            n_syst += 1
        if (fourNuisancesPerBinZGratio):
            zinv_zgamma_nj  =  1.0 + zllgamma_nj     
            zinv_zgamma_nb  =  1.0 + zllgamma_nb
            zinv_zgamma_ht  =  1.0 + zllgamma_ht 
            if (njets_LOW == 1):
                zinv_zgamma_ht = 1.0 + zllgamma_ht2
            else:
                n_syst += 3
            if (not integratedZinvEstimate):
                zinv_zgamma_mt2 = 1.0 + zllgamma_mt2
                n_syst += 1
        else:
            zinv_zgamma = 1.20
            n_syst += 1
    
        n_zinv = n_zinv_cr * zinv_alpha

    qcd_crstat = qcd_alpha
    qcd_alphaerr = 1.0 + (err_qcd_alpha / qcd_alpha)
    qcd_fjrbsyst = 1.0 + err_qcd_fjrb
    qcd_fitstat = 1.0 + err_qcd_fitstat
    qcd_fitsyst = 1.0 + err_qcd_fitsyst

    name_qcd_crstat = "qcd_CRstat_{0}".format(perChannel)
    name_qcd_alphaerr = "qcd_alphaErr_{0}".format(perChannel)
    name_qcd_fjrbsyst = "qcd_FJRBsyst_{0}".format(perChannel)
    name_qcd_fitstat = "qcd_RPHIstat_{0}".format(ht_str)
    name_qcd_fitsyst = "qcd_RPHIsyst_{0}".format(ht_str)

    if (njets_LOW == 1): n_syst += 2
    else: n_syst += 4

    if (doZinvFromDY):
        n_bkg = n_lostlep+n_zinvDY+n_qcd
    else:
        n_bkg = n_lostlep_n_zinv+n_qcd

    if (n_bkg < 0.001): n_qcd = 0.01

    if (f_data is not None):
        h_data = f_data.Get(fullhistname)
        if (h_data is not None):
            n_data = h_data.GetBinContent(imt2)
    else:
        n_data = n_bkg

    uncorr_zinvDY = n_zinvDY_cr > 0 and n_zinvDY == 0
    uncorr_lostlep = n_lostlep_cr > 0 and n_lostlep == 0
    uncorr_qcd = n_qcd_cr > 0 and n_qcd == 0
    
    zero_zinvDY = zinvDY_alpha == 0.0 and n_zinvDY_cr == 0.0
    zero_lostlep = lostlep_alpha == 0.0 and n_lostlep_cr == 0.0
    zero_qcd = qcd_crstat == 0.0 and n_qcd_cr == 0.0

    # Allow ad hoc modification of counts and alphas by setting scale parameters
    # Set alphas = 0 to 2.0 (ad hoc) in certain cases
    scale = 1.0
    n_zinvDY_towrite = scale * n_zinvDY
    n_lostlep_towrite = scale * n_lostlep
    n_qcd_towrite = scale * n_qcd
    # If alpha is 0 then set it to 2.0 for now for comparison with old cards
    zinvDY_alpha_towrite = scale * zinvDY_alpha
    lostlep_alpha_towrite = scale * lostlep_alpha
    qcd_crstat_towrite = scale * qcd_crstat
    if (uncorr_zinvDY or zero_zinvDY): zinvDY_alpha_towrite = scale * 2.0
    if (uncorr_lostlep or zero_lostlep): lostlep_alpha_towrite = scale * 2.0
    if (uncorr_qcd or zero_qcd): qcd_alpha_towrite = scale * 2.0
    
    n_zinvDY_cr_towrite = n_zinvDY_cr
    if (uncorr_zinvDY):
        name_zinvDY_crstat += "_" + mt2_str
        n_zinvDY_cr_towrite = 0
    n_lostlep_cr_towrite = n_lostlep_cr
    if (uncorr_lostlep):
        name_lostlep_crstat += "_" + mt2_str
        n_lostlep_cr_towrite = 0
    n_qcd_cr_towrite = n_qcd_cr
    if (uncorr_qcd):
        name_qcd_crstat += "_" + mt2_str
        n_qcd_cr_towrite = 0

    template = "imax 1  number of channels\n"
    template += "jmax 3  number of backgrounds\n"
    template += "kmax *\n"
    template += "------------\n"
    template += "bin         {0}\n".format(channel)
    template += "observation {0:.3f}\n".format(n_data)
    template += "------------\n"
    template += "bin             {0}   {1}   {2}   {3}\n".format(channel,channel,channel,channel)
    template += "process          sig       zinv        llep      qcd\n"
    template += "process           0         1           2         3\n"
    if (doZinvFromDY):
        template += "rate            n_sig_cor_recogenaverage    {0:.3f}      {1:.3f}      {2:.3f}\n".format(n_zinvDY_towrite,n_lostlep_towrite,n_qcd_towrite)
    else:  
        template += "rate            n_sig_cor_recogenaverage    {0:.3f}      {1:.3f}      {2:.3f}\n".format(n_zinv,n_lostlep,n_qcd)
    template += "------------"

    if (doDummySignalSyst):
        template += "name_sig_syst                                lnN   sig_syst    -      -     - \n"
    else:
        template += "name_sig_lumi                    lnN    sig_lumi   -    -    - \n"
        template += "name_sig_pu                    lnN    sig_pu   -    -    - \n"
        template += "name_sig_mcstat     lnN    sig_mcstat   -    -    - \n"
        template += "name_sig_genmet                  lnU    sig_genmet   -    -    - \n"
        template += "name_sig_isr                  lnU    sig_isr   -    -    - \n"
        template += "name_sig_btagsf_heavy            lnN    sig_btagsf_heavy   -    -    - \n"
        template += "name_sig_btagsf_light            lnN    sig_btagsf_light   -    -    - \n"
        if (isSignalWithLeptons):
            template += "name_sig_lepeff               lnN    sig_lepeff   -    -    - \n"

            
    # Zinv systs
    if (doZinvFromDY):
        template += "{0}        gmN {1:.0f}    -   {2:.5f}   -   - \n".format(name_zinvDY_crstat,n_zinvDY_cr_towrite,zinvDY_alpha_towrite)
        template += "{0}        lnN    -   {1:.3f}   -   - \n".format(name_zinvDY_alphaErr,zinvDY_alphaErr)
        template += "{0}        lnN    -   {1:.3f}   -   - \n".format(name_zinvDY_purity,zinvDY_puritystat)        
        template += "{0}        lnN    -   {1:.3f}   -   - \n".format(name_zinvDY_rsfof,zinvDY_rsfof)
        if (n_extrap_bins_zinvDY > 0 and imt2 >= zinvDY_lastbin_hybrid):
            template += "{0}    lnN    -   {1:.3f}   -   - \n".format(name_zinvDY_shape,zinvDY_shape)
    else:
        print "Zinv currently only implemented for DY"
        exit(1)
    
    # lostlep systs
    if (doSimpleLostlepNuisances):
        template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_lepeff,lostlep_lepeff)
        template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_alphaerr,lostlep_alphaerr)
    else:
        template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_mtcut,lostlep_mtcut)
        template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_taueff,lostlep_taueff)
        template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_btageff,lostlep_btageff)
        if (not doZinvFromDY):
            print "Zinv currently only implemented for DY"
            exit(1)
        template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_renorm,lostlep_renorm)
    
    template += "{0}        gmN {1:.0f}    -    -    {2:.5f}     - \n".format(name_lostlep_crstat,n_lostlep_cr_towrite,lostlep_alpha_towrite)
    template += "{0}        lnN    -    -    {1:.3f}    - \n".format(name_lostlep_mcstat,lostlep_mcstat)
    if (n_mt2bins > 1 and imt2 >= lostlep_lastbin_hybrid):
        template += "{0}    lnN    -    -   {1:.3f}     - \n".format(name_lostlep_shape,lostlep_shape)
    
    if (doZinvFromDY):
        template += "{0}        lnN    -    {1:.3f}    {2:.3f}    - \n".format(name_lostlep_jec,zinvDY_jec,lostlep_jec)
        template += "{0}        lnN    -    {1:.3f}    {2:.3f}    - \n".format(name_lostlep_lepeff,zinvDY_lepeff,lostlep_lepeff)
    else:
        print "Zinv currently only implemented for DY"
        exit(1)
        
    # QCD systs
    template += "{0}        gmN {1:.0f}    -    -    -   {2:.5f}\n".format(name_qcd_crstat,n_qcd_cr_towrite,qcd_crstat_towrite)
    if (njets_LOW == 1): # Monojet
        template += "{0}        lnN    -    -    -   {1:.3f}\n".format(name_qcd_alphaerr,qcd_alphaerr)
    else:
        template += "{0}        lnN    -    -    -   {1:.3f}\n".format(name_qcd_fjrbsyst,qcd_fjrbsyst)
        template += "{0}        lnN    -    -    -   {1:.3f}\n".format(name_qcd_fitstat,qcd_fitstat)
        template += "{0}        lnN    -    -    -   {1:.3f}\n".format(name_qcd_fitsyst,qcd_fitsyst)
    
    return template

def makeCard(tmp,signal,outdir,im1=-1,im2=-1):
    outfile = open("{0}/{1}_{2}_{3}.txt".format(outdir,signal,im1,im2),"w")

    n_sig = 0.0
    n_sig_cor = 0.0
    n_sig_TR = 0.0
    n_sig_crsl = 0.0
    n_sig_crsl_genmet = 0.0
    err_sig_mcstat = 0.0
    err_sig_mcstat_rel = 0.0
    n_sig_genmet = 0.0
    n_sig_cor_genment = 0.0
    n_sig_recogenaverage = 0.0
    n_sig_cor_recogenaverage = 0.0
    n_sig_btagsf_heavy_UP = 0.0
    n_sig_btagsf_light_UP = 0.0
    n_sig_lepeff_UP = 0.0
    n_sig_isr_UP = 0.0
    err_sig_recogenaverage = 0.0

    tmp.replace("placeholder","real_val")
    outfile.write(tmp)
    outfile.close()
    return True

signal_points = []
iterator = f_sig.GetListOfKeys()
keep = "sr"
skip = "srbase"
for key in iterator:
    directory = key.GetTitle()
    print directory
    if directory.find(skip) >= 0: continue
    if directory.find(keep) >= 0:
        sr_number = int(re.findall("\d+",directory)[0]) # Search for the first integer of any length in the directory string
        if (sr_number < 20 and doSuperSignalRegions): continue
        elif (sr_number >= 20 and not doSuperSignalRegions): continue
        #print "Expected a directory corresponding to a numbered signal region, e.g. sr1VL, but could not find a number in directory {0}, aborting...".format(directory)
        #    exit(1)
        n_mt2_name = directory + "/h_n_mt2bins"
        h_n_mt2bins = f_sig.Get(n_mt2_name)
        n_mt2_bins = int(h_n_mt2bins.GetBinContent(1))
        for imt2 in range(1,n_mt2_bins+1):
            template = makeTemplate(directory,imt2)
            if (doScan):
                y_binwidth = 25
                y_max = 1600
                if signal.find("T2cc") != -1:
                    y_binwidth = 5
                    y_max = 800
                for im1 in range(0,2301,25):
                    for im2 in range(0,y_max+1,y_binwidth):
                        if (suppressUHmt2bin and directory.find("UH") != -1 and imt2 == 1): continue
                        success = makeCard(template,signal,outdir,im1,im2)
                        if success:
                            signal_points.append( (im1,im2) )
            else:
                makeCard(template,signal,outdir)

    if (doScan):
        points_file = open("{0}/points_{1}.txt".format(outdir,signal),"w")
        points_file.write("--------------------------------------------\n")
        points_file.write("- saw nonzero signal entries for the following points: \n")
        for mass_point in signal_points:
            points_file.write("{0}_{1}_{2}\n".format(signal,mass_point[0],mass_point[1]))
        points_file.close()

bmark.Stop("benchmark")    
print "\n"
print "------------------------------"
print "CPU Time:   {0:.1f} s\n".format(bmark.GetCpuTime("benchmark"))
print "Real Time:  {0:.1f} s\n".format(bmark.GetRealTime("benchmark"))
print "\n"
