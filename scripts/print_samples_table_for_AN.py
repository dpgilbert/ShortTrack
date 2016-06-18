import sys
sys.path.insert(0, "/home/users/fgolf/ntupling/NtupleTools/AutoTwopler/dashboard/")
import twiki, dis_client
import os, time, re

twiki_username = "FrankGolf"
twiki_name = "Run2Samples25ns80XminiAODv2"
if os.getenv("USER") == "namin":
    twiki_username = "namin"
elif os.getenv("USER") == "fgolf":
    twiki_username = "FrankGolf"

samples = twiki.get_samples(username=twiki_username, page=twiki_name, get_unmade=False, assigned_to="all")
print "Available keys:",samples[0].keys()

samples_to_ignore_mt2 = ["W2JetsToLNu", "W3JetsToLNu", "W4JetsToLNu", "WBJetsToLNu", "BGenFilter", "Zpt", "5to50", "10to50", "DYBJets", "DYBBJets", "TTJets_HT", "TTTo2L2Nu", "inclusiveDecays", "TTWJetsToQQ", "TTWJetsToLNu", "TTZToQQ", "TTZToLLNuNu", "TGJets", "tZq", "/WW", "/WZ", "/ZZ", "/VV", "WpWpJJ", "WGstar", "WGToLNuG", "ZGTo2LG", "WZZ", "WWW", "WWZ", "ZZZ", "QCD_Pt", "GenJets5", "bEnriched", "WminusH", "VHToNonbb"] 
                     

print ""

print "\\begin{table}[htp]"
print "\caption{}"
print "\centering"
print "\scriptsize"
print "\makebox[\\textwidth][c]{%"
print "\\begin{tabular}{l|c|c|c}"
print "\hline"
print "Sample & Cross Section & K-factor & Effective luminosity \\\\"
print "& [pb] & & [/fb]\\\\"
print "\hline"

for sample in samples:
    ignore = False
    for isample in samples_to_ignore_mt2:
        if isample in sample["dataset"]:
            ignore = True
            break
    if ignore:
        continue
    total_xsec = float(sample["xsec"]) * float(sample["kfact"]) * float(sample["efact"])
    eqlumi = float(sample["nevents_out"]) / total_xsec / 1000 ##convert to /fb
    kfact = sample["kfact"]
    if float(kfact) == 1:
        kfact = "-"
    dataset = sample["dataset"].replace("RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0","*").replace("_","\_")
    if eqlumi > 10:
        print dataset, "&", sample["xsec"], "&", kfact, "&", "%.0f" % eqlumi, "\\\\"
    elif eqlumi > 1:
        print dataset, "&", sample["xsec"], "&", kfact, "&", "%.1f" % eqlumi, "\\\\"
    else:
        print dataset, "&", sample["xsec"], "&", kfact, "&", "%.2f" % eqlumi, "\\\\"


print "\hline"
print "\end{tabular}}"
print "\end{table}"
