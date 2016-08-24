import ROOT

lumi = 7.7

## returns a list of yields, where the first is the data observation
## and the rest are background yields from the processes specified
## in bkg_names
def GetYieldsFromDatacard(datacard_fname, bkg_names):
    lines = open(datacard_fname).readlines()
    
    names = []
    rates = []
    obs = 0
    for line in lines:
        if line.startswith("observation"):
            obs = int(float(line.split()[1]))
        if line.startswith("process") and len(names)==0:
            names = line.split()[2:]
        if line.startswith("rate"):
            rates = [float(x) for x in line.split()[2:]]

    bkg_rates = []
    for name in bkg_names:
        try:
            ind = names.index(name)
        except:
            raise RuntimeError("provided list of backgrounds does not match those in datacard!")
        bkg_rates.append(rates[ind])

    return [obs]+bkg_rates

## returns a list of 2-tuples, one for each background.
## numbers are the upper and lower relative uncertainties
## for each background process
def GetUncertaintiesFromDatacard(datacard_fname, bkg_names):
    lines = open(datacard_fname).readlines()

    tot_err_up = [0 for i in bkg_names]
    tot_err_down = [0 for i in bkg_names]
    pastRates = 0
    for line in lines:
        if line.startswith("rate"):
            pastRates=True
        if not pastRates:
            continue

        procInd = -1
        for i in range(len(bkg_names)):
            if line.startswith(bkg_names[i]):
                procInd = i
                break

        if procInd == -1:
            continue
        
        # given a list of strings, return the first that is a valid number (expecting only 1)
        def getNum(lst):
            x = None
            for s in lst:
                try:
                    xt = float(s.split("/")[0])
                    x = xt
                except:
                    continue
                if x != None:
                    return x
            raise RuntimeError("Could not find valid number in datacard: "+str(lst))

        sp = line.split()
        errType = sp[1]
        if errType == "lnN":
            unc = getNum(sp[2:])
            tot_err_up[procInd]   += abs(1-unc)**2
            tot_err_down[procInd] += abs(1-unc)**2
        if errType == "gmN":
            N = int(sp[2])
            alpha = 1 - 0.6827
            xfact = getNum(sp[3:])
            tot_err_up[procInd]   += 0 if N==0 else (ROOT.Math.gamma_quantile_c(alpha/2, N+1, 1) / N - 1)**2
            tot_err_down[procInd] += 0 if N==0 else (1 - ROOT.Math.gamma_quantile(alpha/2, N, 1) / N)**2

    tot_err_up = [ROOT.TMath.Sqrt(x) for x in tot_err_up]
    tot_err_down = [ROOT.TMath.Sqrt(x) for x in tot_err_down]
    return zip(tot_err_up, tot_err_down)


def GetJBJregions(ht_reg):
    if ht_reg != "monojet":
        return ["j2to3_b0","j2to3_b1","j2to3_b2","j4to6_b0","j4to6_b1","j4to6_b2","j7toInf_b0","j7toInf_b1","j7toInf_b2","j2to6_b3toInf","j7toInf_b3toInf"]
    else:
        return ["j1_b0","j1_b1toInf"]

def GetMT2bins(ht_reg):
    mt2bins = []
    jbj_regs = GetJBJregions(ht_reg)

    for i,jbj_reg in enumerate(jbj_regs):
        bins = []
        if ht_reg == "HT200to450":
            if 0 <= i <= 5:            bins = [200,300,400]
            if 6 <= i <= 8 or i==10:   bins = [200]
            if i==9:                   bins = [200,300]
        if ht_reg == "HT450to575":
            if 0 <= i <= 5:            bins = [200,300,400,500]
            if i==6 or i==8 or i==10:  bins = [200]
            if i==7 or i==9:           bins = [200,300]
        if ht_reg == "HT575to1000":
            if 0 <= i <= 1 or i==3:    bins = [200,300,400,600,800]
            if i==2 or 4 <= i <= 5:    bins = [200,300,400,600]
            if 6 <= i <= 10:           bins = [200,300,400]
        if ht_reg == "HT1000to1500":
            if i==0 or i==3:           bins = [200,400,600,800,1000]
            if i==1 or i==4:           bins = [200,400,600,800]
            if i==2 or 8 <= i <= 10:   bins = [200,400]
            if 5 <= i <= 7:            bins = [200,400,600]
        if ht_reg == "HT1500toInf":
            if i==0 or i==3:           bins = [200,400,600,800,1000]
            if i==1 or i==4 or i==5:   bins = [200,400,600]
            if i==2:                   bins = [200]
            if 6 <= i <= 10:           bins = [200,400]
        # for monojet, mt2bins are actually ht bins
        if ht_reg == "monojet":
            if i==0: bins = [200,250,350,450,575,700,1000]
            if i==1: bins = [200,250,350,450,575]

        if bins==[]:
            raise RuntimeError("Couldn't get MT2 binning for region {0} {1}".format(ht_reg,jbj_reg))
        
        bins.append(-1)
        mt2bins.append(bins)

    return mt2bins

def GetHTtitle(ht_reg):
    if ht_reg == "HT200to450": return "200 < H_{T} < 450 GeV"
    if ht_reg == "HT450to575": return "450 < H_{T} < 575 GeV"
    if ht_reg == "HT575to1000": return "575 < H_{T} < 1000 GeV"
    if ht_reg == "HT1000to1500": return "1000 < H_{T} < 1500 GeV"
    if ht_reg == "HT1500toInf": return "H_{T} > 1500 GeV"
    if ht_reg == "monojet": return "Monojet Region"

    return ht_reg

def GetJBJtitle(jbj_reg):
    lines = ["",""]
    jreg = jbj_reg.split("_")[0]
    if jreg=="j2to3":   lines[0] = "2-3j"
    elif jreg=="j4to6": lines[0] = "4-6j"
    elif jreg=="j2to6": lines[0] = "2-6j"
    elif jreg=="j7toInf": lines[0] = "#geq7j"
    elif jreg=="j1": lines[0] = "1j"
    else: lines[0] = jreg

    bjreg = jbj_reg.split("_")[1]
    if bjreg=="b0": lines[1] = "0b"
    elif bjreg=="b1": lines[1] = "1b"
    elif bjreg=="b2": lines[1] = "2b"
    elif bjreg=="b3toInf": lines[1] = "#geq3b"
    elif bjreg=="b1toInf": lines[1] = "#geq1b"
    else: lines[1] = bjreg

    return lines

def GetMT2name(left, right):
    s = "m{0}to{1}".format(left,right)
    s = s.replace("-1","Inf")
    return s

def GetMT2label(left, right):
    if right != -1:
        return "[{0}, {1}]".format(left,right)
    else:
        return "> {0}".format(left)

def GetLegendName(proc):
    if proc=="zinv": return "Z#rightarrow#nu#bar{#nu}"
    if proc=="llep": return "Lost Lepton"
    if proc=="qcd": return "Multijet"
    
    return proc

