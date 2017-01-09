import sys
import os

test = sys.argv[2] if len(sys.argv) > 2 else False

eras = ['Run2016B','Run2016C','Run2016D','Run2016E','Run2016F','Run2016G','Run2016H']

indir = sys.argv[1]

for era in eras:
  list_of_dirs = []
  for dirname in os.listdir(indir):
    if not os.path.isdir(os.path.join(indir,dirname)): continue
    if era not in dirname: continue
    list_of_dirs.append(os.path.join(indir,dirname))
  list_of_files = []
  for dirname in list_of_dirs:
    for fname in os.listdir(dirname):
      if os.path.isfile(os.path.join(dirname,fname)):
        list_of_files.append(os.path.join(dirname,fname))
  cmd = "hadd data_{0}.root ".format(era) + " ".join(list_of_files)
  print cmd
  if not test:
    os.system(cmd)
    
