import os
import numpy as np
import ROOT
import pyRootPlotMaker as ppm
import MT2PlotUtils as utils
import MT2PlotDefs as pd

def MT2PlotMaker(rootdir, samples, data, dirname, plots, output_dir=".", exts=["pdf"]):
    # rootdir contains output of MT2Looper, samples are names of the .root files,
    # data is the name of the data file, dirname is the directory within the root file
    # to extract plots from, plots are a list of plot definitions from MT2PlotDefs
    # exts is a list of file extensions to produce
    # note that dirname can be a '+' separated string of directories to add together

    h_bkg_vecs = [[] for x in plots]
    h_data = []

    dirnames = [s.strip() for s in dirname.split("+")]

    ## get background histograms
    for isamp in range(len(samples)):

        # get the root file for the given sample. This assumes that frag/fake photons come from qcd_ht.root
        if samples[isamp] in ["fragphoton","fakephoton"]:
            fn = os.path.join(rootdir,"qcd_ht.root")
        else:
            fn = os.path.join(rootdir,samples[isamp]+".root")        
        fid = ROOT.TFile(fn)

        for iplot in range(len(plots)):
            vn = plots[iplot][0]

            # fix the var name for fake photons
            if samples[isamp]=="fakephoton":
                if vn.find("Loose")==-1 and vn.find("AllIso")==-1:
                    vn += "Fake"
                else:
                    vn = vn.replace("Loose","FakeLoose")
                    vn = vn.replace("AllIso","FakeAllIso")

            h_bkg_vecs[iplot].append( fid.Get(dirnames[0]+"/h_"+vn) )
            if type(h_bkg_vecs[iplot][-1])==type(ROOT.TObject()):
                raise Exception("No {0}/h_{1} histogram for sample {2}!".format(dirnames[0], vn, samples[isamp]))
            h_bkg_vecs[iplot][-1].SetDirectory(0)

            # handle the case with more than one directory
            for idir in range(1, len(dirnames)):
                h_bkg_vecs[iplot][-1].Add(fid.Get(dirnames[idir]+"/h_"+vn))

        fid.Close()

    ## get data histograms
    data_file = os.path.join(rootdir, data+".root")
    fid = ROOT.TFile(data_file)
    for pl in plots:
        vn = pl[0]
        h_data.append( fid.Get(dirnames[0]+"/h_"+vn) )
        if type(h_data[-1])==type(ROOT.TObject()):
            raise Exception("No {0}/h_{1} histogram for {2}!".format(dirname, vn, data))
        h_data[-1].SetDirectory(0)
        # handle the case with more than one directory
        for idir in range(1, len(dirnames)):
            h_data[-1].Add(fid.Get(dirnames[idir]+"/h_"+vn))
    fid.Close()

    # make the output directory if it doesn't exist
    if not os.path.isdir(os.path.join(output_dir,dirname)):
        os.makedirs(os.path.join(output_dir,dirname))

    # make all of the plots
    for i in range(len(plots)):
        vn = plots[i][0]
        userMin,userMax = None,None
        if plots[i][3]!=None:
            userMin = plots[i][3][0]
            userMax = plots[i][3][1]
        if len(plots[i]) >= 5:
            utils.Rebin(h_bkg_vecs[i],h_data[i], plots[i][4])
        doOverflow = True
        if len(plots[i]) >= 6:
            doOverflow = plots[i][5]
        markerSize=0.8
        title = utils.GetCRName(dirname)
        xAxisTitle = utils.GetVarName(vn)
        unit = utils.GetUnit(vn)
        subtitles = utils.GetSubtitles(dirname)
        subLegText = ["MC scaled by {datamcsf}","# Data events: {ndata}"]
        sns = [utils.GetSampleName(s) for s in samples]
        for ext in exts:
            saveAs = os.path.join(output_dir,dirname,"{0}_{1}.{2}".format(dirname,vn,ext))
            ppm.plotDataMC(h_bkg_vecs[i], sns, h_data[i], doPause=False, xAxisTitle=xAxisTitle, lumi=pd.lumi, lumiUnit=pd.lumiUnit,
                           title=title, subtitles=subtitles, xRangeUser=plots[i][2], isLog=plots[i][1], saveAs=saveAs, 
                           scaleMCtoData=True, xAxisUnit=unit, userMin=userMin, userMax=userMax, doSort=False, 
                           doMT2Colors=True, markerSize=markerSize, titleSize=0.035, subtitleSize=0.03,
                           subLegText=subLegText, doBkgError=True, doOverflow=doOverflow)
            


def makeNormalizedLostLep(indir, samples, data, outdir='.', exts=['png','pdf']):
    ## makes plots of lost lepton CR (crsl) yields after MC is normalized in
    ## each HT, Njet, Nbjet bin separately. Makes 0b, 1b, inclusive plots
    ## and puts in directory called "lostlep"

    fmc = [ROOT.TFile(os.path.join(indir,s+".root"), "READ") for s in samples]
    fdata = ROOT.TFile(os.path.join(indir,data+".root"), "READ")

    regions_0b = ["1","4","7"]
    regions_ge1b = ["2","3","5","6","8","10"]
    regions_incl = regions_0b + regions_ge1b
    
    regions = [regions_0b, regions_ge1b, regions_incl]
    region_names = ["0b","ge1b","incl"]

    mt2bins = [200, 300, 400, 500, 600, 800, 1000, 1500]
    mt2bins = np.array(mt2bins, dtype=float)

    bkg_names = [utils.GetSampleName(s) for s in samples]

    try:
        os.makedirs(os.path.join(outdir, "lostlep"))
    except:
        pass

    #loop over sets of regions (0b, >=1b, inclusive)
    for iregs,regs in enumerate(regions):
        h_mt2_mc_cr_vec = [None for s in samples]
        h_mt2_data_cr = None
        # loop over set of SRs within the given region
        for isr, sr in enumerate(regs):
            for ht_reg in ["VL","L","M","H","UH"]:
                
                # get the data/mc CR yields
                mc_cr_yield = 0.0
                data_cr_yield = 0.0
                for i in range(0,len(fmc)):
                    mc_cr_yield += fmc[i].Get("crsl{0}{1}/h_mt2".format(sr,ht_reg)).Integral(0,-1)
                # sometimes we get 0 data events in a CR bin, so handle appropriately
                try:
                    data_cr_yield += fdata.Get("crsl{0}{1}/h_mt2".format(sr,ht_reg)).Integral(0,-1)
                except AttributeError:
                    pass

                scalefactor = data_cr_yield/mc_cr_yield

                # form the appropriately scaled histograms
                for i in range(len(fmc)):
                    if h_mt2_mc_cr_vec[i] == None:
                        h_mt2_mc_cr_vec[i] = fmc[i].Get("crsl{0}{1}/h_mt2".format(sr,ht_reg)).Clone("h_mt2_mc_cr_"+str(i))
                        h_mt2_mc_cr_vec[i].Scale(scalefactor)
                    else:
                        h_mt2_mc_cr_vec[i].Add(fmc[i].Get("crsl{0}{1}/h_mt2".format(sr,ht_reg)), scalefactor)

                # again, somtimes 0 events in data CR
                try:
                    if h_mt2_data_cr == None:
                        h_mt2_data_cr = fdata.Get("crsl{0}{1}/h_mt2".format(sr,ht_reg)).Clone("h_mt2_data_cr")
                    else:
                        h_mt2_data_cr.Add(fdata.Get("crsl{0}{1}/h_mt2".format(sr,ht_reg)))
                except (TypeError, AttributeError):
                    pass

        h_mt2bins_mc_vec = [h.Rebin(mt2bins.size-1, "h_mt2bins_mc_"+str(i), mt2bins) for h in h_mt2_mc_cr_vec]
        h_mt2bins_data = h_mt2_data_cr.Rebin(mt2bins.size-1, "h_mt2bins_data", mt2bins)

        subtitles = ["#geq 2j, 1 lepton", "M_{T2} > 200 GeV"]
        if iregs==0:
            subtitles[0] = "#geq 2j, 0b, 1 lepton"
        if iregs==1:
            subtitles[0] = "#geq 2j, #geq 1b, 1 lepton"
            
        for ext in exts:
            saveAs = os.path.join(outdir, "lostlep", "lostlep_{0}_mt2bins.{1}".format(region_names[iregs],ext))
            ppm.plotDataMC(h_mt2bins_mc_vec, bkg_names, h_mt2bins_data, doPause=False, xAxisTitle="M_{T2}",
                           lumi=pd.lumi, lumiUnit=pd.lumiUnit, title=None, subtitles=subtitles, isLog=True,
                           saveAs=saveAs, scaleMCtoData=False, xAxisUnit="GeV", doSort=False, doMT2Colors=True,
                           markerSize=1.0, subtitleSize=0.040, doBkgError=True, doOverflow=False,
                           cmsTextSize=0.040)

    for i in range(len(fmc)):
        fmc[i].Close()
    fdata.Close()

