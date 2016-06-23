import os
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
                           doMT2Colors=True, markerSize=markerSize, titleSize=0.035, subtitleSize=0.025,
                           subLegText=subLegText, doBkgError=True, doOverflow=doOverflow)
            
