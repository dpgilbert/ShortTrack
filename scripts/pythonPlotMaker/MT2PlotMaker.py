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
    data_file = os.path.join(rootdir, data+".root")

    h_bkg_vecs = [[] for x in plots]
    h_data = []

    ## get background histograms
    for i in range(len(samples)):

        # get the root file for the given sample. This assumes that frag/fake photons come from qcd_ht.root
        if samples[i] in ["fragphoton","fakephoton"]:
            fn = os.path.join(rootdir,"qcd_ht.root")
        else:
            fn = os.path.join(rootdir,samples[i]+".root")        
        fid = ROOT.TFile(fn)

        for j in range(len(plots)):
            vn = plots[j][0]

            # fix the var name for fake photons
            if samples[i]=="fakephoton":
                if vn.find("Loose")==-1:
                    vn += "Fake"
                else:
                    vn = vn.replace("Loose","FakeLoose")

            h_bkg_vecs[j].append( fid.Get(dirname+"/h_"+vn) )
            if type(h_bkg_vecs[j][-1])==type(ROOT.TObject()):
                raise Exception("No {0}/h_{1} histogram for sample {2}!".format(dirname, vn, samples[i]))

            h_bkg_vecs[j][-1].SetDirectory(0)

        fid.Close()

    ## get data histograms
    fid = ROOT.TFile(data_file)
    for pl in plots:
        vn = pl[0]
        h_data.append( fid.Get(dirname+"/h_"+vn) )
        if type(h_data[-1])==type(ROOT.TObject()):
            raise Exception("No {0}/h_{1} histogram for {2}!".format(dirname, vn, data))
        h_data[-1].SetDirectory(0)
    fid.Close()

    # make the output director if it doesn't exist
    if not os.path.isdir(os.path.join(output_dir,dirname)):
        os.makedirs(os.path.join(output_dir,dirname))

    # make all of the plots
    for i in range(len(plots)):
        vn = plots[i][0]
        userMin,userMax = None,None
        if plots[i][3]!=None:
            userMin = plots[i][3][0]
            userMax = plots[i][3][1]
        markerSize=0.7
        title = utils.GetCRName(dirname)
        xAxisTitle = utils.GetVarName(vn)
        unit = utils.GetUnit(vn)
        subtitles = ["H_{T} > 200 GeV","M_{T2} > 200 GeV", "#geq 2j"]
        subLegText = ["MC scaled by {datamcsf}","# Data events: {ndata}"]
        sns = [utils.GetSampleName(s) for s in samples]
        for ext in exts:
            saveAs = os.path.join(output_dir,dirname,"{0}_{1}.{2}".format(dirname,vn,ext))
            ppm.plotDataMC(h_bkg_vecs[i], sns, h_data[i], doPause=False, xAxisTitle=xAxisTitle, lumi=pd.lumi, lumiUnit=pd.lumiUnit,
                           title=title, subtitles=subtitles, xRangeUser=plots[i][2], isLog=plots[i][1], saveAs=saveAs, 
                           scaleMCtoData=True, xAxisUnit=unit, userMin=userMin, userMax=userMax, doSort=False, 
                           doMT2Colors=True, markerSize=markerSize, titleSize=0.035, subtitleSize=0.025,
                           subLegText=subLegText)
            

