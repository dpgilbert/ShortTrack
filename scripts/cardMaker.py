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
    nbjets_LOW = h_nbjets_LOW.GetBinContent(1)
    nbjets_HI = h_nbjets_HI.GetBinContent(1)
    njets_LOW_name = directory + "/h_njets_LOW"
    njets_HI_name = directory + "/h_njets_HI"
    h_njets_LOW = f_sig.Get(njets_LOW_name)
    h_njets_HI = f_sig.Get(njets_HI_name)
    njets_LOW = h_njets_LOW.GetBinContent(1)
    njets_HI = h_njets_HI.GetBinContent(1)

    # If we're doing a scan over mass points, we need to extract any 2D histogram from the all-inclusive 
    # 3D histogram to read MT2 boundaries.
    # If we've got only a single mass point, no projection procedure is needed (and attempting it will fail).
    if (doScan):
        h_sigscan = f_sig.Get(fullhistnameScan)
        h_sig = h_sigscan.ProjectionX("h_sig",0,0,0,0) # bin 0 is guaranteed to exist, so it's suitable as a dummy projection
    else:
        h_sig = f_sig.Get(fullhistname)

    mt2_LOW = h_sig.GetBinLowEdge(imt2)
    mt2_HI = mt2_LOW + h_sig.GetBinWidth(imt2)
    # The uppermost bin actually extends to infinity, which we represent with -1 for now.
    # TODO: Update these hardcoded values to their new values if they should change
    if (mt2_HI == 1800 or mt2_HI == 1500): mt2_HI = -1

    nbjets_HI_mod = nbjets_HI
    njets_HI_mod = njets_HI
    # The upper limit (if it's finite) is exclusive, but the descriptive string is inclusive. *_mod will be used to make strings....
    if (nbjets_HI != -1): nbjets_HI_mod -= 1
    if (njets_HI != -1): njets_HI_mod -= 1

    ht_str = "HT{0}to{1}".format(str(ht_LOW),str(ht_HI))
    if (nbjets_HI != nbjets_HI_mod):
        bjet_str = "b{0}to{1}".format(str(nbjets_LOW),str(nbjets_HI_mod))
    else:
        bjet_str = "b{0}".format(str(nbjets_LOW))
    if (njets_HI != njets_HI_mod):
        jet_str = "j{0}to{1}".format(str(njets_LOW),str(njets_HI_mod))
    else:
        jet_str = "j{0}".format(str(njets_LOW))
    mt2_str = "m{0}to{1}".format(str(mt2_LOW),str(mt2_HI))

    ht_str.replace("-1","Inf")
    bjet_str.replace("-1","Inf")
    jet_str.replace("-1","Inf")
    mt2_str.replace("-1","Inf")

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
        ht_LOW_crsl = h_ht_LOW_crsl.GetBinContent(1)
        ht_HI_crsl = h_ht_HI_crsl.GetBinContent(1)

        h_nbjets_LOW_crsl = f_lostlep.Get(nbjets_LOW_name)
        h_nbjets_HI_crsl = f_lostlep.Get(nbjets_HI_name)
        nbjets_LOW_crsl = h_nbjets_LOW_crsl.GetBinContent(1)
        nbjets_HI_crsl = h_nbjets_HI_crsl.GetBinContent(1)

        h_njets_LOW_crsl = f_lostlep.Get(njets_LOW_name)
        h_njets_HI_crsl = f_lostlep.Get(njets_HI_name)
        njets_LOW_crsl = h_njets_LOW_crsl.GetBinContent(1)
        njets_HI_crsl = h_njets_HI_crsl.GetBinContent(1)

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

    ht_str_crsl.replace("-1","Inf")
    bjet_str_crsl.replace("-1","Inf")
    jet_str_crsl.replace("-1","Inf")

    return "template"

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
