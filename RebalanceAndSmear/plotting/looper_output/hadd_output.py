#! /usr/bin/env python
## syntax: python hadd_output.py <directory with smearlooper output>
## hadds together histograms of like samples. Keeps ext and non-ext separate
## makes a directory in this looper_output directory to keep the results

import ROOT
import os
import sys
import glob

test=True

indir = sys.argv[1]
basedir = indir.strip("/").split("/")[-1]

try:
  os.makedirs(basedir)
except:
  pass

list_of_subdirs = [indir]
list_of_files = []

batch_size = 50

idir = 0
while idir < len(list_of_subdirs):
  print "looping on dir: {0}".format(list_of_subdirs[idir])
  list_of_files = []    
  for fname in os.listdir(list_of_subdirs[idir]):      
    path = os.path.join(list_of_subdirs[idir],fname)
    if os.path.isdir(path):
      print "Appending directory {0}".format(path)
    list_of_subdirs.append(path) if os.path.isdir(path) else list_of_files.append(path)            
  if len(list_of_files):
    outpath = os.path.join(basedir,list_of_subdirs[idir].split("/"+basedir)[-1].strip("/"))                                       
    try:
      print "Making directory: {0}".format(outpath)
      if not test:
        os.makedirs(outpath)
    except:
      pass
    try:
      print "Preparing to hadd {0} files".format(len(list_of_files))
      if len(list_of_files) < batch_size:
        if not test:
          os.system("hadd " + os.path.join(outpath,"merged.root") + " " + " ".join(list_of_files))
      else:
        ##
        ## divide them up into sets of batch_size to be more manageable
        ##
        nProcessedTotal = 0                
        nProcessedThis = 0
        while nProcessedTotal < len(list_of_files):
          print "Merging files {0}-{1} into merged_{2}.root".format(nProcessedTotal,min(nProcessedTotal+batch_size-1,len(list_of_files)),int(round(nProcessedTotal/batch_size)))
          last_processed = nProcessedTotal+batch_size if (nProcessedTotal+batch_size < len(list_of_files)) else len(list_of_files)
          if not test:
            os.system("hadd " + os.path.join(outpath,"merged_{0}.root".format(int(round(nProcessedTotal/batch_size)))) + " " + " ".join(list_of_files[nProcessedTotal:last_processed]))
          nProcessedTotal+=batch_size
        tmp_list_of_files = []
        sum_file_sizes = 0
        for f in glob.glob(os.path.join(outpath,"merged_*.root")):
          tmp_list_of_files.append(f)
          sum_file_sizes += os.stat(f).st_size
              
        if len(tmp_list_of_files) and (sum_file_sizes/1024/1024) < 10000:
          print "Combining output of {0} files and cleaning up".format(len(tmp_list_of_files))
          if not test:
            os.system("hadd " + os.path.join(outpath,"merged.root") + " " + " ".join(tmp_list_of_files))
            for f in tmp_list_of_files:
              os.remove(f)                    
    except:
      pass
  idir+=1
