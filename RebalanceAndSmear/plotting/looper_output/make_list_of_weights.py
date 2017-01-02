import sys
import os
import subprocess
import ROOT as r

list_of_ext_files = []
list_of_non_ext_files =[]
list_of_files = []
ext_file_weights = {}
non_ext_file_weights = {}

indir = sys.argv[1]

out = subprocess.Popen("find {0} -type f".format(indir),shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
(stdout,stderr) = out.communicate()
list_of_files = stdout.decode().split()
for f in list_of_files:
    if "smearbaby" not in f: continue
    if "ext" in f:
        list_of_ext_files.append(f)
    else:
        list_of_non_ext_files.append(f)

for f in list_of_ext_files:
    f = r.TFile(f)
    for event in f.mt2:
        sid = event.evt_id
        w = event.evt_scale1fb
        ext_file_weights[sid] = w
        break
for f in list_of_non_ext_files:    
    f = r.TFile(f)
    for event in f.mt2:
        sid = event.evt_id
        w = event.evt_scale1fb
        non_ext_file_weights[sid] = w
        break

print ext_file_weights
print non_ext_file_weights
