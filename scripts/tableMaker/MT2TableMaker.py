import os
import ROOT
import MT2TableUtils as utils

def assembleFullTable(listOfGuts, ncols):
    text  = "\\begin{table}[!ht]\n"
    text += "\\caption{}\n"
    text += "\\centering\n"
    text += "\\begin{tabular}{r" + "|c"*ncols + "}\n"
    text += "\\hline\n"
    
    for s in listOfGuts:
        text += s

    text += "\\hline \n"
    text += "\end{tabular}\n"
    text += "\end{table}\n"

    return text

def printTableGuts(samples, binNames, title, yields, errors, maxCols=None, doTotalSM=True):

    text = ""

    nbins = len(binNames)
    if maxCols==None:
        maxCols = 99999
    ncols = min(maxCols, nbins)
    
    text += "\\hline \n"
    text += "\\multicolumn{{{0}}}{{c}}{{{1}}} \\\\ \n".format(ncols+1, title)
    text += "\\hline \n"

    ndivs = int(float(nbins)/ncols - 0.00001) + 1
    for idiv in range(ndivs):
        text += "\\hline \n Sample"
        for ic in range(min(ncols, nbins-idiv*ncols)):
            text += " & "+binNames[idiv*ncols + ic]
        text += "\\\\ \n"
        text += "\\hline \\hline \n"
        for ir in range(len(samples)):
            text += utils.GetSampleName(samples[ir])
            for ic in range(min(ncols, nbins-idiv*ncols)):
                if yields[ir][idiv*ncols+ic] < 10:
                    ystr = "{0:.2f}".format(yields[ir][idiv*ncols+ic])
                    estr = "{0:.2f}".format(errors[ir][idiv*ncols+ic])
                else:
                    ystr = "{0:.1f}".format(yields[ir][idiv*ncols+ic])
                    estr = "{0:.1f}".format(errors[ir][idiv*ncols+ic])                    
                text += " & " + ystr + " $\pm$ " + estr
            text += "\\\\ \n"
        if doTotalSM:
            text += "\\hline \n Total SM"
            for ic in range(min(ncols, nbins-idiv*ncols)):
                tyld = sum([y[idiv*ncols+ic] for y in yields])
                terr = ROOT.TMath.Sqrt(sum([e[idiv*ncols+ic]**2 for e in errors]))
                if tyld < 10:
                    ystr = "{0:.2f}".format(tyld)
                    estr = "{0:.2f}".format(terr)
                else:
                    ystr = "{0:.1f}".format(tyld)
                    estr = "{0:.1f}".format(terr)
                text += " & " + ystr + " $\pm$ " + estr                
            text += "\\\\ \n"
        text += "\\hline \n"


    return text
    
def makeTableFromHistBins(indir, samples, dirnames, hname, bins=None, binNames=None, maxCols=None):
    
    if type(dirnames)!=type([]):
        dirnames = [dirnames]

    # get a test histo to count the number of bins
    fid = ROOT.TFile(os.path.join(indir,samples[0]+".root"))
    h = fid.Get(dirnames[0] + "/" + hname)
    ntotbins = h.GetNbinsX()
    fid.Close()
    
    if bins == None:
        bins = [(list(range(1,ntotbins+1)) + [-1]) for dn in dirnames]

    if binNames == None:
        binNames = []
        for i in range(len(bins)):
            binNames.append([])
            for j in range(len(bins[i])-1):
                binNames[i].append(utils.GetBinName(dirnames[i], j+1))

    if maxCols==None:
        maxCols = 99999
    
    listOfGuts = []
    for idir in range(len(dirnames)):
        nbins = min(ntotbins, len(bins[idir])-1)
        ncols = min(maxCols, nbins)
        yields = [[0 for i in range(nbins)] for j in range(len(samples))]
        errors = [[0 for i in range(nbins)] for j in range(len(samples))]
        for isamp in range(len(samples)):
            fid = ROOT.TFile(os.path.join(indir,samples[isamp]+".root"))
            h = fid.Get(dirnames[idir] + "/" + hname)
            for ib in range(nbins):
                err = ROOT.Double()
                b1 = bins[idir][ib]
                b2 = bins[idir][ib+1]
                if b2!=-1:
                    b2 -= 1
                try:
                    yld = h.IntegralAndError(b1,b2,err)
                    yields[isamp][ib] = yld
                    errors[isamp][ib] = err
                except AttributeError:
                    yields[isamp][ib] = 0
                    errors[isamp][ib] = 0                    
            fid.Close()

        listOfGuts.append(printTableGuts(samples, binNames[idir], utils.GetCRName(dirnames[idir]), yields, errors, maxCols=maxCols))
                       

    return assembleFullTable(listOfGuts, ncols)

def makeTableFromHTregs(indir, samples, ht_regs, dirnames, binNames=None, maxCols=None):
    if binNames == None:
        binNames = []
        for i in range(len(dirnames)):
            binNames.append(utils.GetBinName(dirnames[i], i+1))

    if maxCols==None:
        maxCols = 99999

    listOfGuts = []
    for reg in ht_regs:
        nbins = len(dirnames)
        ncols = min(maxCols, nbins)
        yields = [[0 for i in range(nbins)] for j in range(len(samples))]
        errors = [[0 for i in range(nbins)] for j in range(len(samples))]
        for isamp in range(len(samples)):
            fid = ROOT.TFile(os.path.join(indir,samples[isamp]+".root"))
            for idir in range(len(dirnames)):
                h = fid.Get(dirnames[idir]+reg+ "/h_Events_w")
                err = ROOT.Double()
                try:
                    yld = h.IntegralAndError(0,-1,err)
                except AttributeError:
                    yld = 0
                    err = 0
                yields[isamp][idir] = yld
                errors[isamp][idir] = err

        listOfGuts.append(printTableGuts(samples, binNames, utils.GetCRName(reg), yields, errors, maxCols=maxCols))

    return assembleFullTable(listOfGuts, ncols)

