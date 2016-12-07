#! /usr/bin/env python
## syntax: python hadd_output.py <directory with smearlooper output>
## hadds together histograms of like samples. Keeps ext and non-ext separate
## makes a directory in this looper_output directory to keep the results

import ROOT
import os
import sys
import glob

indir = sys.argv[1]
basedir = indir.strip("/").split("/")[-1]

try:
    os.makedirs(basedir)
except:
    pass

# samples = ["dyjetsll", "gjets_ht", "ttsl", "ttdl", "ttz", "ttw", "ttg", "singletop", "wjets_ht", "zinv_ht", "qcd_ht"]
# samples = ["dyjetsll", "gjets_ht", "ttsl", "ttdl", "ttz", "ttw", "ttg", "singletop", "wjets_ht", "zinv_ht"]
samples = ["qcd_ht"]

for samp in samples:
    ext_files = []
    nonext_files = []
    for file in glob.glob(os.path.join(indir, samp+"*.root")):
        if "ext1" in file:
            ext_files.append(file)
        else:
            nonext_files.append(file)

    
    if len(ext_files) != 0:
        os.system("hadd " + os.path.join(basedir,samp+"_ext.root") + " " + " ".join(ext_files))

    if len(nonext_files) != 0:
        os.system("hadd " + os.path.join(basedir,samp+"_nonext.root") + " " + " ".join(nonext_files))
