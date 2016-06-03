# MT2PlotUtils.py
# various plotting utilities

def GetCRName(cr):
    names = {"crrlbase": "Removed Lepton CR",
             "crgjbase": "#gamma+jets CR",
             "crdybase": "Z #rightarrow #font[12]{l}^{#plus}#font[12]{l}^{#minus} CR",
             "crslbase": "Single Lepton CR",
             "crslwjets": "Single Lepton CR (wjets)",
             "crslttbar": "Single Lepton CR (ttbar)",
             "crslelbase": "Single Lepton CR (els)",
             "crslmubase": "Single Lepton CR (mus)",
             }

    # use the above name if defined, otherwise use cr itself
    return names.get(cr,cr)

def GetSampleName(sample):
    names = {"wjets_ht": "W+Jets",
             "wjets_incl": "W+Jets",
             "dyjetsll_ht": "Z(#font[12]{ll})+Jets",
             "dyjetsll_incl": "Z(#font[12]{ll})+Jets",
             "top": "Top",
             "gjets_ht": "Prompt #gamma",
             "fakephoton": "Fake #gamma",
             "fragphoton": "Frag. #gamma",
             "qcd_ht": "QCD",
             }

    # use the above name if defined, otherwise use sample itself
    return names.get(sample,sample)

def GetVarName(var):
    names = {"ht": "H_{T}",
             "met": "E_{T}^{miss}",
             "mt2": "M_{T2}",
             "nJet30": "N(jet)",
             "nBJet20": "N(b jet)",
             "letppt": "P_{T}(lep)",
             "lepeta": "#eta(lep)",
             "nlepveto": "N(lep veto)",
             "zllmass": "m_{#font[12]{ll}}",
             "gammaPt": "P_{T}(#gamma)",
             "gammaEta": "#eta(#gamma)",
             }

    # use the above name if defined, otherwise use var itself
    return names.get(var,var)

def GetUnit(vn):
    noUnit = ["nJet","nBJet","eta","nlep","drMin","chiso"]
    for s in noUnit:
        if s in vn:
            return None

    return "GeV"

