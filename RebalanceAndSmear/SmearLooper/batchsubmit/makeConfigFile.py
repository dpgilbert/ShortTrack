import glob
import os

suffix = "_tail100"
indir = "/hadoop/cms/store/user/bemarsh/mt2babies/merged/RebalanceAndSmear_V00-08-12"

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
log=/data/tmp/bemarsh/condor_submit_logs/smearing/condor_12_01_16.log
output=/data/tmp/bemarsh/condor_job_logs/smearing/1e.$(Cluster).$(Process).out
error =/data/tmp/bemarsh/condor_job_logs/smearing/1e.$(Cluster).$(Process).err
notification=Never
x509userproxy=/tmp/x509up_u31592

""".format(suffix))

for f in glob.glob(os.path.join(indir, "*.root")):
    bn = f.split("/")[-1].split(".")[0]
    if bn.find("qcd")==-1:
        continue
    fid.write("executable=wrapper.sh\n")
    fid.write("transfer_executable=True\n")
    fid.write("arguments=/hadoop/cms/store/user/bemarsh/mt2babies/merged/RebalanceAndSmear_V00-08-12 {0} /hadoop/cms/store/user/bemarsh/smearoutput/RebalanceAndSmear_V00-08-12{1}\n".format(bn, suffix))
    fid.write("queue\n\n")
             

fid.close()
