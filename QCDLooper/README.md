## QCDLooper
Dedicated looper to produce histograms needed for computation of rphi, fj, rb used in the deltaPhi QCD Estimate.
Can't run on the normal skims, since we need low MT2 values. Probably want to do a special skim or else this
takes forever.

To use, modify `do.sh` with desired input/output directories and then run. Will produce a separate output root file for each
sample, much like MT2Looper.

Next, combine all non-QCD samples:      
   hadd -f nonqcd.root dyjetsll_ht.root gjets_dr0p05_ht.root singletop.root ttdl.root ttg.root ttsl.root ttw.root ttz.root wjets_ht.root

Next, feed these into the `makeQCDHistos.py` script in the `scripts/qcdEstimate` directory. This will combine the looper output 
files into barebones histograms for rphi, fj, rb, stored in a single root file.

Finally, pass this output into `makeQCDPlots.py` to get pretty plots.
