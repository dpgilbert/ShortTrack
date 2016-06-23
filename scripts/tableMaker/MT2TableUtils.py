
def GetCRName(cr):
    if cr[-3:]=="J0B":
        return "Monojet regions, 0b"
    if cr[-3:]=="J1B":
        return "Monojet regions, 1b"

    if cr=="VL":
        return "$200 < H_T < 450$ GeV, $E_T^{miss} > 200$ GeV"
    if cr=="L":
        return "$450 < H_T < 575$ GeV, $E_T^{miss} > 200$ GeV"
    if cr=="M":
        return "$575 < H_T < 1000$ GeV, $E_T^{miss} > 200$ GeV"
    if cr=="H":
        return "$1000 < H_T < 1500$ GeV, $E_T^{miss} > 200$ GeV"
    if cr=="UH":
        return "$H_T > 1500$ GeV, $E_T^{miss} > 200$ GeV"

def GetSampleName(s):
    names = {"wjets_ht": "W+Jets",
             "top": "Top",
             "qcd_ht": "QCD",
             }

    return names.get(s,s)

def GetBinName(cr, bin):
    if cr == "crslbaseJ0B":
        names = {1: "$200 < p_{T}^\\text{jet1} < 250$",
                 2: "$250 < p_{T}^\\text{jet1} < 350$",
                 3: "$350 < p_{T}^\\text{jet1} < 450$",
                 4: "$450 < p_{T}^\\text{jet1} < 575$",
                 5: "$575 < p_{T}^\\text{jet1} < 700$",
                 6: "$700 < p_{T}^\\text{jet1} < 1000$",
                 7:       "$p_{T}^\\text{jet1} > 1000$",
                 }
    elif cr == "crslbaseJ1B":
        names = {1: "$200 < p_{T}^\\text{jet1} < 250$",
                 2: "$250 < p_{T}^\\text{jet1} < 350$",
                 3: "$350 < p_{T}^\\text{jet1} < 450$",
                 4: "$450 < p_{T}^\\text{jet1} < 575$",
                 5:       "$p_{T}^\\text{jet1} > 575$",
                 }
    elif cr.find("crsl")==0:
        names = {1: "2-3j, 0b",
                 2: "2-3j, 1b",
                 3: "2-3j, 2b",
                 4: "4-6j, 0b",
                 5: "4-6j, 1b",
                 6: "4-6j, 2b",
                 7: "$\\geq$7j, 0b",
                 8: "$\\geq$7j, 1-2b",
                 9: "2-6j, $\\ge3$b",
                 }
    else:
        names = {}

    return names.get(bin,'bin'+str(bin))
