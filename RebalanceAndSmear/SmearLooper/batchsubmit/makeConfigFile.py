import glob
import os
import subprocess
import fnmatch
import re


#samples = ["datav2_data_Run2016[B-H]_(HTMHT|JetHT|MET)"]
#samples = ["datav2_data_Run2016[B-H]_(HTMHT|MET)"]
#samples = ["datav2_data_Run2016[B-H]_JetHT"]
#samples = ["datav2_data_Run2016[B-H]_MET"]
#samples = ["qcd_ht", "zinv_ht", "wjets_ht", "ttdl", "ttsl", "singletop", "ttw", "ttz"]
#samples = ["zinv_ht", "wjets_ht", "ttdl", "ttsl", "singletop", "ttw", "ttz"]
#samples = ["data_rereco_data_Run2016[B-G]_(HTMHT|JetHT|MET)", "datav2_data_Run2016H_(HTMHT|JetHT|MET)"]
#samples = ["data_rereco_data_Run2016[B-G]_JetHT", "datav2_data_Run2016H_JetHT"]
#samples = ["data_rereco_data_Run2016[E-G]_(HTMHT|MET)", "datav2_data_Run2016H_(HTMHT|MET)"]
#samples = ["data_rereco_data_Run2016[E-G]_JetHT", "datav2_data_Run2016H_JetHT"]
#samples = ["data_rereco_data_Run2016[B-D]_JetHT"]
#samples = ["data_rereco_data_Run2016C_MET"]
#samples = ["data_rereco_data_Run2016C_HTMHT"]
#samples = ["data_rereco_data_Run2016[B-D]_(JetHT|HTMHT|MET)"]
samples = ["qcd_ht"]
#samples = ["data_rereco_data_Run2016[B-G]_JetHT", "datav2_data_Run2016H_JetHT"]

test = False
version="v11_rawhists"
doRebalanceAndSmear = True
tag="V00-08-12"
username = os.environ["USER"]
use_b_resp = True
make_baby = True
use_raw_hists = True
core_scale = 1.
mean_shift = 0.
tail_scale = 1.
apply_weights = False
indir = "/hadoop/cms/store/user/bemarsh/mt2babies/"
numFilesPerJob = 15
append=False

x509file = subprocess.check_output(["find","/tmp/", "-maxdepth", "1", "-type", "f", "-user", username, "-regex", "^.*x509.*$"])
if not x509file:
    print "Could not find x509 file for user {0}. Please run voms-proxy-init -voms cms -valid 240:00 to obtain a valid proxy.  Exiting.".format(username)
    quit()

options = ""
if make_baby: options += "-b "
if use_raw_hists: options += "-h "
if not use_b_resp: options += "-j "
if doRebalanceAndSmear: options += "-r "
if apply_weights: options += "-w "
options += "-c {0} -m {1} -t {2}".format(core_scale, mean_shift, tail_scale)

suffix = ""
is_first = True
for sample in samples:
  re_sample = re.compile("RebalanceAndSmear_"+tag+"_"+sample)
  if "data" in sample:
    suffix = "_data"
  else:
    suffix = "_"+sample.replace('_ht','')    
  if not doRebalanceAndSmear:
    suffix += "_noRS"

  if not test:
    mode = 'w'
    if append and not is_first: mode = 'a'
    fid = open("config{0}.cmd".format(suffix),mode)

    if (not append) or is_first:
      fid.write("""
      universe=grid
      Grid_Resource=condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu
      when_to_transfer_output = ON_EXIT
      #the actual executable to run is not transfered by its name.
      #In fact, some sites may do weird things like renaming it and such.
      transfer_input_files=wrapper.sh, job_input{0}/input.tar.gz
      +DESIRED_Sites="T2_US_UCSD"
      +Owner = undefined
      log=/data/tmp/fgolf/condor_submit_logs/smearing/condor_12_01_16.log
      output=/data/tmp/fgolf/condor_job_logs/smearing/1e.$(Cluster).$(Process).out
      error =/data/tmp/fgolf/condor_job_logs/smearing/1e.$(Cluster).$(Process).err
      notification=Never
      x509userproxy={1}
    
      """.format(suffix, x509file))    
    
  for dirname in os.listdir(indir):
    m = re_sample.match(dirname)
    if not m: continue
    if test:
      print "Writing {0} to config{1}.cmd".format(dirname,suffix)
    if not test:
      list_of_files = []
      for f in glob.glob(os.path.join("{0}/{1}".format(indir, dirname), "*.root")):        
        bn = f.split("/")[-1].split(".")[0]
        list_of_files.append(bn)
        if len(list_of_files) == numFilesPerJob:
          fid.write("executable=wrapper.sh\n")
          fid.write("transfer_executable=True\n")
          fid.write("arguments={0} {1} {2} /hadoop/cms/store/user/{3}/smearoutput/{4}/{5}/{6}\n".format(options, "/".join(f.split("/")[:-1]), ",".join(list_of_files), username, version, suffix.strip("_"), dirname))
          fid.write("queue\n\n")
          list_of_files = []
      ## now, if there are still files left in the list, write them to config
      if len(list_of_files):
        fid.write("executable=wrapper.sh\n")
        fid.write("transfer_executable=True\n")
        fid.write("arguments={0} {1} {2} /hadoop/cms/store/user/{3}/smearoutput/{4}/{5}/{6}\n".format(options, "/".join(f.split("/")[:-1]), ",".join(list_of_files), username, version, suffix.strip("_"), dirname))
        fid.write("queue\n\n")
        list_of_files = []          
  is_first = False
if not test:
    fid.close()
