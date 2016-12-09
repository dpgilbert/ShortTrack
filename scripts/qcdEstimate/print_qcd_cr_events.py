#! /usr/bin/env python

import os
import sys
import ROOT

    
def main(args):    

    fin = ROOT.TFile(args[1])
    dirlist = list(fin.Get("nCR").GetListOfKeys())
    
    for idir in dirlist:
        
        print "{0:15s}\t".format(idir),
        
            h = fin.Get("rb_{0}/h_nbjets".format(idir))

            for xbin in range(1, h.GetNbinsX()+1):
                if xbin-1 < 3:                    
                    print '{0:.1f}\t'.format(h.GetBinContent(xbin)),
                elif xbin-1 == 3:
                    print '{0:.1f}\t'.format(h.Integral(xbin,h.GetNbinsX()+1))
                    
            fin.Close()

            print "\n\n"
            
if __name__ == "__main__":
    main(sys.argv)
