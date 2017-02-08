## Making Scans

This directory contains all the scripts you need to submit combine jobs to condor, and then make smoothed temperature plots with contours for full scans. 

To get the necessary input files, do

``` bash
source setup.sh
```

### Submitting combine jobs to condor
To submit limit setting jobs to condor, modify `MODEL`, `DATE`, `INDIR`, and `COMBINED` in `submitLimitsToCondor.sh` appropriately, then just do

``` bash
source submitLimitsToCondor.sh
```

This will call `do_tar.sh` to create tarballs containing the datacards for each point of the specified model, and then call `submitAllJobs.sh` to submit the condor jobs to compute the limits, one job per mass point. You may want to modify the OUTPUTDIR variable in `submit.sh` that defines where the output of the jobs goes.

Once the jobs are done, you can make the temperature plots as described below.

### ETH plotting scripts
To make the scans using ETH code, define the input variables `MODEL`, `DATE`, and `INDIR` in `makeScans.sh`, then source it (it will make scans with ETH scripts by default):

```bash
source makeScans.sh
```

This will first call `makeLimitFile.py` to create a text file with all the limits for the specified model, and then `drawSMSlimit.py` which will convert the text file to 2d histograms, smooth them, and extract the smoothed contours. The output will be saved in `limits_$MODEL_$DATE.root` and fed into the PlotSMS limit ploting package.

### SNT plotting scripts
To make the scans using SNT code, define the input variables `MODEL`, `DATE`, and `INDIR` in `makeScans.sh`, then source it using `SNT` as an argument:

```bash
source makeScans.sh SNT
```

This will call `make_rValues.C` to create 2D histograms of the limits, `smooth.C` to interpolate and smooth the histograms, and then `make_contour.C` to extract the contours from the smoothed histograms. The output will be saved in `r-values_$MODEL.root` and fed into the PlotSMS limit plotting package.
