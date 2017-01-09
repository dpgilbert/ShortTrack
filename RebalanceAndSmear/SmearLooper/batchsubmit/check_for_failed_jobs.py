#! /usr/bin/env python
import os
import sys
import re
import subprocess

def GetJobInputDir (infile):
  f = open(infile, 'r')
  match = re.search('job_input(.*)/', f.read())
  if match:
    return match.group(1)
  
def GetExpectedOutputFiles (infile):

  list_of_files = []
  list_of_args = []
  f = open(infile, 'r')
  for line in f:    
    match = re.search(r'/hadoop.*', line)
    if match:
      fname = match.group().split()[1].split(',')[0]+'.root'
      fpath = match.group().split()[2]
      list_of_files.append(os.path.join(fpath,fname))
      list_of_args.append(line)    
  f.close()
  return (list_of_files,list_of_args)

def GetListOfMissingFiles (list_of_files, list_of_args):
  list_of_missing_args = []
  for index, f in enumerate(list_of_files):
    if not os.path.isfile(f):
      list_of_missing_args.append(list_of_args[index])
  return list_of_missing_args
  

def main (args):
  
  list_of_files, list_of_args  = GetExpectedOutputFiles(args[1])
  list_of_failed_jobs = GetListOfMissingFiles(list_of_files, list_of_args)
  if len(list_of_failed_jobs) > 0:
    username = os.environ["USER"]
    x509file = subprocess.check_output(["find","/tmp/", "-maxdepth", "1", "-type", "f", "-user", username, "-regex", "^.*x509.*$"])
    if not x509file:
      print "Could not find x509 file for user {0}. Please run voms-proxy-init -voms cms -valid 240:00 to obtain a valid proxy.  Exiting.".format(username)
      quit()

    fid = open(args[1].replace('.cmd','_resubmit.cmd'), 'w')
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
    
    """.format(GetJobInputDir(args[1]), x509file))

    for f in list_of_failed_jobs:
      fid.write("executable=wrapper.sh\n")
      fid.write("transfer_executable=True\n")
      fid.write(f)
      fid.write("queue\n\n")

    fid.close()
      
if __name__ == "__main__":
  main(sys.argv)
