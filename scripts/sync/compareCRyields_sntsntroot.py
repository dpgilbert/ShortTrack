import ROOT

#snt_file = ROOT.TFile("/home/users/bemarsh/analysis/mt2/current/MT2Analysis/MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_12p9fb/data_Run2016_sync.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/zinv_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/gjets_dr0p05_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_noBtagSF/zinv_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_noBtagSF/gjets_dr0p05_ht.root")
#snt_file = ROOT.TFile("../MT2looper/output/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/data_Run2016.root")
#snt_file = ROOT.TFile("../MT2looper/output/testSync/unknown_test_Zinv_80X.root") #20k
snt_file = ROOT.TFile("../MT2looper/output/testSync/zinv_ht800to1200ComplSkim.root") #full

#eth_file = ROOT.TFile("eth_estimates/gamma_data.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noBtagSF.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noSF_SnTMC.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noSF_SnTMC_fixedIso.root")
#eth_file = ROOT.TFile("gamma_mc_afterLoopOnSnT.root")
#eth_file = ROOT.TFile("zinvFromGamma_80X_noSF.root")
#eth_file = ROOT.TFile("gamma_data_allData.root")
#eth_file = ROOT.TFile("../MT2looper/output/testSync/ZJets_comparisonFile.root")
eth_file = ROOT.TFile("../MT2looper/output/testSync/ZJetsToNuNu_HT800to1200_post_skim_prune.root")

snt_label = "sr"
#eth_label = "gammaCR" 
#eth_label = "Zinv"
#eth_label = "prompt_integral"   # MC prompt 
#eth_label = "gamma_est_integral" # data in combined file
#eth_label = "gammaCR" # data in individual file

eth_label = "sr"

#snt_label = "sr"
#eth_label = "Zinv"

eth_yields = {}
snt_yields = {}
eth_total = 0.
snt_total = 0.

# multijet regions
for ht_reg in ["VL","L","M","H","UH"]:
    for top_reg in range(1,12):
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
        try:
#            sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))   # int
            sntyield = (snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))    # float
#            sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).GetEntries())    # int, entries
        except:
            sntyield = 0
#        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,-1))   # int
#        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,1))   # int, first bin only
#        ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,-1))   # float
#        ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).Integral(1,1))   # float, first bin only
#        ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label, ethname)).GetEntries())   # int, entries
#        ethyield = int(eth_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))   # int
        ethyield =    (eth_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))    # float
#            ethyield = int(eth_file.Get("{0}/h_mt2bins".format(sntname)).GetEntries())    # int, entries

        snt_yields[ethname] = sntyield
        eth_yields[ethname] = ethyield
        snt_total += sntyield
        eth_total += ethyield

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
        #sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))                           # int
        sntyield = (snt_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))                               # float
    except:
        sntyield = 0

#    sntyield = int(snt_file.Get("{0}/h_mt2bins".format(sntname)).GetEntries())                             # int, entries
#    ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,-1))         # int    
#    ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,1))         # int, first bin only    
#    ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,-1))             # float
#    ethyield = (eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).Integral(1,1))             # float, first bin only
#    ethyield = int(eth_file.Get("{0}/{1}/yield_{0}_{1}".format(eth_label,ethname)).GetEntries())             # int, entries, first bin only
    try:    
#        ethyield = int(eth_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))                           # int    
        ethyield = (eth_file.Get("{0}/h_mt2bins".format(sntname)).Integral(0,-1))                           # float    
    except:
        ethyield = 0

    snt_yields[ethname] = sntyield
    eth_yields[ethname] = ethyield
    snt_total += sntyield
    eth_total += ethyield
    

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

print "{0:30s} {1:>14s} {2:>14s} {3:>8s}".format("Region", "SNT CR yield", "ETH CR yield", "Ratio")
print "--------------------------------------------------------------------------------"
for key,r in ratios_sorted: 
#    print "{0:30s} {1:14d} {2:14d} {3:8.5f}".format(key,snt_yields[key],eth_yields[key],r)         # int
    print "{0:30s} {1:14f} {2:14f} {3:8.5f}".format(key,snt_yields[key],eth_yields[key],r)          # float

print "--------------------------------------------------------------------------------"
print "{0:30s} {1:14f} {2:14f} {3:8.5f}".format("total events",snt_total,eth_total,snt_total/eth_total)
