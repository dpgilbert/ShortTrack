import glob
import os
import subprocess
import fnmatch

version="v1"
tag="V00-08-12"
# sample="datav2_data_Run2016H_JetHT"
sample="qcd_ht"
suffix = "_qcd_noRS"
username = os.environ["USER"]
make_baby = True
core_scale = 1.
mean_shift = 0.
doRebalanceAndSmear = False
tail_scale = 1.
apply_weights = False
indir = "/hadoop/cms/store/user/bemarsh/mt2babies/"

x509file = subprocess.check_output(["find","/tmp/", "-maxdepth", "1", "-type", "f", "-user", username, "-regex", "^.*x509.*$"])
if not x509file:
    print "Could not find x509 file for user {0}. Please run voms-proxy-init -voms cms -valid 240:00 to obtain a valid proxy.  Exiting.".format(username)
    quit()

fid = open("config{0}.cmd".format(suffix),'w')


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

options = ""
if make_baby: options += "-b "
if doRebalanceAndSmear: options += "-r "
if apply_weights: options += "-w "
options += "-c {0} -m {1} -t {2}".format(core_scale, mean_shift, tail_scale)

for dirname in os.listdir(indir):
    if "RebalanceAndSmear_{0}_{1}".format(tag, sample) in dirname:
        for f in glob.glob(os.path.join("{0}/{1}".format(indir, dirname), "*.root")):
            bn = f.split("/")[-1].split(".")[0]            
            fid.write("executable=wrapper.sh\n")
            fid.write("transfer_executable=True\n")
            fid.write("arguments={0} {1} {2} /hadoop/cms/store/user/{3}/smearoutput/{4}/{5}/{6}\n".format(options, "/".join(f.split("/")[:-1]), bn, username, version, suffix.strip("_"), dirname))
            fid.write("queue\n\n")
            
fid.close()
