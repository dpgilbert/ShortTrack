import ROOT, math

#snt_file = ROOT.TFile("/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_12p9fb/data_Run2016_sync.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/zinv_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/gjets_dr0p05_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_noBtagSF/zinv_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_noBtagSF/gjets_dr0p05_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/data_Run2016.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_skim_periodsCDEFGH/data_Run2016.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_skim_periodsCDEFGH_infcheck/data_Run2016B.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_unblind_BCDpartG/data_Run2016.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_dypt100_30/data_Run2016.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_dypt100_30/lostlepFromCRs.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_dypt100_30/zinvFromDY.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_dypt100_30_nottbarisrweight/lostlepFromCRs.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-12_json_271036-284044_23Sep2016ReReco_36p46fb_dy7j1bmerge/zinvFromDY.root")'
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-09_7Dec16/zinvFromDY.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-09_7Dec16/lostlepFromCRs.root")
#snt_file = ROOT.TFile("../../MT2looper/output/V00-08-09_5Dec16/data_Run2016.root")

#eth_file = ROOT.TFile("eth_estimates/gamma_data.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noBtagSF.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noSF_SnTMC.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noSF_SnTMC_fixedIso.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_llepCR_2016_RunCtoH_eth.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_zllCR_2016_RunCtoH_eth.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_of_zllCR_2016_RunCtoH_eth.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_gammaCR_2016_RunCtoH_eth.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_llepCR_2016_RunB_eth.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_zllCR_2016_RunB_eth.root")
#eth_file = ROOT.TFile("eth_sync_Nov2016/data_of_zllCR_2016_RunB_eth.root")
#eth_file = ROOT.TFile("gamma_mc_afterLoopOnSnT.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noSF.root")
#eth_file = ROOT.TFile("gamma_data_allData.root")
#eth_file = ROOT.TFile("eth_sync_dec2016/analysisYields_16p9ifb.root")
#eth_file = ROOT.TFile("eth_sync_dec2016/llepEstimate_36p5ifb.root")
#eth_file = ROOT.TFile("eth_sync_dec2016/zinvFromZll_36p5ifb.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/zinvFromZll.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/zinvFromZll_updatedShape.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/llepEstimate.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/llepEstimate_fixedISR.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/llepEstimate_fixed.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/data_forZinvEst.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/data_forZinvEst_08Dec.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/data_of_forZinvEst.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/zinvFromZll_noBtagSF_new.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/zinvFromZll_36p5ifb_08Dec.root")
#eth_file = ROOT.TFile("eth_sync_dec2016_snttrees/llepEstimate_noBtagSF.root")

#snt_label = "crsl"
#snt_label = "crdy"
#snt_label = "crgj"
#snt_label = "sr"
#eth_label = "gammaCR" 
#eth_label = "Zinv"
#eth_label = "llepCR"
#eth_label = "llepEstimate"
#eth_label = "llep_est_integral"
#eth_label = "ZinvEstimateFromZll_hybrid"
#eth_label = "zllData_forHybrid"
#eth_label = "data"
#eth_label = "data_of"
#eth_label = "prompt_integral"   # MC prompt 
#eth_label = "gamma_est_integral" # data in combined file
#eth_label = "gammaCR" # data in individual file

#snt_label = "sr"
#eth_label = "Zinv"


# Working combos:
snt_folder = "../../MT2looper/output/full2016unblind/"
eth_folder = "20Jan16eth"
#type = "data"
type = "ZinvEst"
#type = "LLepEst"
#type = "LLepCR"
#type = "ZinvCR"
#type = "ZinvCROF"

print "Looking at: " + type

if (type == "ZinvEst") :
    snt_file = ROOT.TFile(snt_folder+"/zinvFromDY.root")
    eth_file = ROOT.TFile(eth_folder+"/zinvFromZll_Jan20.root")
    snt_label = "sr"
    eth_label = "ZinvEstimateFromZll_hybrid"

if (type == "data") :
    snt_file = ROOT.TFile(snt_folder+"/data_Run2016.root")
    eth_file = ROOT.TFile(eth_folder+"/dataSR_Jan20.root")
    snt_label = "sr"
    eth_label = "data"

if (type == "LLepEst") :
    snt_file = ROOT.TFile(snt_folder+"/lostlepFromCRs.root")
    eth_file = ROOT.TFile(eth_folder+"/llepEstimate_Jan20.root")
    snt_label = "sr"
    eth_label = "llepEstimate"

if (type == "LLepCR") :
    snt_file = ROOT.TFile(snt_folder+"/data_Run2016.root")
    eth_file = ROOT.TFile(eth_folder+"/llepCR_Jan20.root")
    snt_label = "crsl"
    eth_label = "llepCR"

if (type == "ZinvCR") :
    snt_file = ROOT.TFile(snt_folder+"/data_Run2016.root")
    eth_file = ROOT.TFile(eth_folder+"/zllCR_Jan20.root")
    snt_label = "crdy"
    eth_label = "data"

if (type == "ZinvCROF") :
    snt_file = ROOT.TFile(snt_folder+"/data_Run2016.root")
    eth_file = ROOT.TFile(eth_folder+"/zllCR_OF_Jan20.root")
    snt_label = "crdy"
    eth_label = "data_of"


eth_yields = {}
snt_yields = {}
eth_total = 0.
snt_total = 0.

# multijet regions
for ht_reg in ["VL","L","M","H","UH"]:
    for top_reg in range(1,16):
        if (ht_reg == "VL" and top_reg >= 4 and top_reg <= 11):
            continue
        elif (ht_reg != "VL" and top_reg >= 12):
            continue
        
        sntname = "{0}{1}{2}".format(snt_label,top_reg,ht_reg)
        
        if ht_reg=="VL": ht_name = "HT250to450"
        if ht_reg=="L" : ht_name = "HT450to575"
        if ht_reg=="M" : ht_name = "HT575to1000"
        if ht_reg=="H" : ht_name = "HT1000to1500"
        if ht_reg=="UH": ht_name = "HT1500toInf"

        if top_reg in [1,2,3]: j_name = "j2to3"
        if top_reg in [4,5,6]: j_name = "j4to6"
        if top_reg in [7,8,9,11]: j_name = "j7toInf"
        if top_reg in [10]: j_name = "j2to6"
        if top_reg in [12,13,14]: j_name = "j4toInf"
        if top_reg in [15]: j_name = "j2toInf"

        if top_reg in [1,4,7,12]: b_name = "b0"
        if top_reg in [2,5,8,13]: b_name = "b1"
        if top_reg in [3,6,9,14]: b_name = "b2"
        if top_reg in [10,11,15]: b_name = "b3toInf"

        ethname = "{0}_{1}_{2}".format(ht_name,j_name,b_name)
        #print eth_label
        #print ethname
        try:
#            sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))   # int
#            sntyield = (snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))    # float
#            sntyield = (snt_file.Get("{0}/hybridEstimate".format(sntname)).Integral(0,-1))    # float
#            sntyield = (snt_file.Get("{0}/CRyieldCard".format(sntname)).Integral(0,1))    # float
#            sntyield = (snt_file.Get("{0}/h_mt2binsCRyieldEM".format(sntname)).Integral(0,-1))    # float
#            sntyield = (snt_file.Get("{0}/h_mt2binsemu".format(sntname)).Integral(0,-1))    # float
#            sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).GetEntries())    # int, entries
            if (type == "data" or type == "LLepCR" or type == "ZinvCR"):
                sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))   # int
            elif (type == "LLepEst"):
                sntyield = (snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))    # float
            elif (type == "ZinvEst"):
                sntyield = (snt_file.Get("{0}/hybridEstimate".format(sntname)).Integral(0,-1))    # float
            elif (type == "ZinvCROF"):
                sntyield = int(snt_file.Get("{0}/h_mt2binsemu".format(sntname)).Integral(0,-1))   # int
        except:
            sntyield = 0
#        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,-1))   # int
#        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,1))   # int, first bin only
#        print "{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)
#        ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral())   # float
#        ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,1))   # float, first bin only
#        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).GetEntries())   # int, entries
        
        if (type == "data" or type == "LLepCR" or type == "ZinvCR" or type == "ZinvCROF"):
            ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,-1))   # int
        elif (type == "ZinvEst" or type == "LLepEst"):
            ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,-1))   # float


        snt_yields[ethname] = sntyield
        eth_yields[ethname] = ethyield
        if not math.isnan(sntyield): snt_total += sntyield
        if not math.isnan(ethyield): eth_total += ethyield

for mj_reg in range(1,8)+range(11,16):
    sntname = "{0}{1}J".format(snt_label, mj_reg)

    if mj_reg in [1,2,3,4,5,6,7]: jbj_name = "j1_b0"
    else:                         jbj_name = "j1_b1toInf"

    if mj_reg in [1,11]: ht_name = "HT250to350"
    if mj_reg in [2,12]: ht_name = "HT350to450"
    if mj_reg in [3,13]: ht_name = "HT450to575"
    if mj_reg in [4,14]: ht_name = "HT575to700"
    if mj_reg in [5]:    ht_name = "HT700to1000"
    if mj_reg in [6]:    ht_name = "HT1000to1200"
    if mj_reg in [7]:    ht_name = "HT1200toInf"
    if mj_reg in [15]:   ht_name = "HT700toInf"

    ethname = "{0}_{1}".format(ht_name,jbj_name)

    try:
       # print eth_label
       # print ethname
#    sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))                           # int
#        sntyield = (snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))                               # float
#        sntyield = (snt_file.Get("{0}/hybridEstimate".format(sntname)).Integral(0,-1))                               # float
#        sntyield = (snt_file.Get("{0}/CRyieldCard".format(sntname)).Integral(0,1))                               # float
#        sntyield = (snt_file.Get("{0}/h_mt2binsCRyieldEM".format(sntname)).Integral(0,-1))                               # float
#        sntyield = (snt_file.Get("{0}/h_mt2binsemu".format(sntname)).Integral(0,-1))                               # float
#    sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).GetEntries())                             # int, entries
        if (type == "data" or type == "LLepCR" or type == "ZinvCR"):
            sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))   # int
        elif (type == "ZinvEst"):
            sntyield = (snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))    # float
        elif (type == "ZinvEst"):
            sntyield = (snt_file.Get("{0}/hybridEstimate".format(sntname)).Integral(0,-1))    # float
        elif (type == "ZinvCROF"):
            sntyield = int(snt_file.Get("{0}/h_mt2binsemu".format(sntname)).Integral(0,-1))   # int
    except:
        sntyield = 0.
#    ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,-1))         # int    
#    ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,1))         # int, first bin only    
#    ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,-1))             # float
#    ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral())             # float, first bin only
#    ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).GetEntries())             # int, entries, first bin only
    

    if (type == "data" or type == "LLepCR" or type == "ZinvCR" or type == "ZinvCROF"):
        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral())   # int
        #ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).GetEntries())             # int, entries, first bin only

    elif (type == "ZinvEst" or type == "LLepEst"):
        ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,-1))             # float

    snt_yields[ethname] = sntyield
    eth_yields[ethname] = ethyield
    if not math.isnan(sntyield): snt_total += sntyield
    if not math.isnan(ethyield): eth_total += ethyield
    

ratios = {}
for key in eth_yields.keys():

    if(eth_yields[key] == 0):
        if(snt_yields[key] == 0):
            r = 1
        else:
            r = 999
    else:
        r = float(snt_yields[key]) / eth_yields[key]
    ratios[key] = r

ratios_sorted = sorted(ratios.items(), key=lambda x: x[1], reverse=True)

print "{0:30s} {1:>14s} {2:>14s} {3:>8s}".format("Region", "SNT yield", "ETH yield", "Ratio")
print "--------------------------------------------------------------------------------"
for key,r in ratios_sorted: 
    if (type == "data"):
        print "{0:30s} {1:14d} {2:14d} {3:8.5f}".format(key,snt_yields[key],eth_yields[key],r)         # int
    else:
        print "{0:30s} {1:14f} {2:14f} {3:8.5f}".format(key,snt_yields[key],eth_yields[key],r)         # float

print "--------------------------------------------------------------------------------"
print "{0:30s} {1:14f} {2:14f} {3:8.5f}".format("total events",snt_total,eth_total,snt_total/eth_total)
