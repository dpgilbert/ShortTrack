source setup.sh to get necessary input files

In do_tar.sh, submitAllJobs.sh, and makePlotInput.sh modify the MODEL and INDIR variables to point to the directory containing the datacards made with cardMaker.

source do_tar.sh to create tarballs containing the datacards for each point of the specified model.

source submitAllJobs.sh to submit the condor jobs to compute the limits, one job per mass point. You may want to modify the OUTPUTDIR variable in submit.sh that defines where the output of the jobs goes.

Once all jobs are done, source makePlotInput.sh in order to read the results of the limit jobs and create 2D smoothed histograms and contours. The output of this step will be called r-values_$MODEL.root and should be fed to the PlotSMS limit plotting package.
