import ROOT as r
import sys
import os
import getopt
    

def mergeQCDEstimateFiles(eth_file,snt_file,out_file):
    feth = r.TFile(eth_file)
    fsnt = r.TFile(snt_file)
    fout = r.TFile(out_file,'RECREATE')

    eth_topdirs = [d.GetName() for d in feth.GetListOfKeys()]
    snt_topdirs = [d.GetName() for d in fsnt.GetListOfKeys()]
    
    for d in snt_topdirs:
        print d
        fout.mkdir(d)
        eth_subdirs = []
        snt_subdirs = []
        if d in eth_topdirs:
            feth.cd(d)
            eth_subdirs = [sub.GetName() for sub in feth.CurrentDirectory().GetListOfKeys()]
        if d in snt_topdirs:
            fsnt.cd(d)
            snt_subdirs = [sub.GetName() for sub in fsnt.CurrentDirectory().GetListOfKeys()]
        if not eth_subdirs: continue
        for sdir in snt_subdirs:            
            fout.mkdir(os.path.join(d,sdir))
            if sdir in eth_subdirs:
                feth.cd(os.path.join(d,sdir))
                eth_hists = [hist.GetName() for hist in feth.CurrentDirectory().GetListOfKeys()]
                for hist in eth_hists:
                    h = feth.Get("{0}/{1}/{2}".format(d,sdir,hist)).Clone()
                    h.SetDirectory(0)
                    fout.cd(os.path.join(d,sdir))
                    h.Write()
            else:
                fsnt.cd(os.path.join(d,sdir))
                snt_hists = [hist.GetName() for hist in fsnt.CurrentDirectory().GetListOfKeys()]
                for hist in snt_hists:
                    h = fsnt.Get("{0}/{1}/{2}".format(d,sdir,hist)).Clone()
                    h.SetDirectory(0)
                    fout.cd(os.path.join(d,sdir))
                    h.Write()
    fout.Close()
    fsnt.Close()
    feth.Close()

if __name__ == "__main__":
    eth_file = ''
    snt_file = ''
    out_file = ''
    try:
        opts, args = getopt.getopt(sys.argv[1:],"",["eth=","snt=","out="])
        print opts
        print args
    except getopt.GetoptError:
        print 'mergeQCDEstimateFiles.py --eth <eth_file> --snt <snt_file> --out <out_file>'
        sys.exit(2)    
    for opt,arg in opts:
        if opt == "--eth":
            eth_file = arg
        elif opt == "--snt":
            snt_file = arg
        elif opt == "--out":
            out_file = arg
    if not eth_file or not snt_file or not out_file:
        print 'incorrect arguments provided'
        print 'mergeQCDEstimateFiles.py --eth <eth_file> --snt <snt_file> --out <out_file>'
        sys.exit(2)    
    print 'Combining qcd estimate files: {0}, {1} into output file {2}'.format(eth_file, snt_file, out_file)    
    mergeQCDEstimateFiles(eth_file,snt_file,out_file)
