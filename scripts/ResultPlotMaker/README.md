Script to make result plots based on the yields and uncertainties in the datacards.
Just edit the datacard directory and name format in do.py and run `python do.py`.
Note that `cardMaker.C` must be run with `suppressZeroBins` and `suppressZeroTRs` to `False`
so that cards will be printed even for 0 signal.

This uses the pyRootPlotMaker found here:
https://github.com/cmstas/Software/tree/master/pyRootPlotMaker

Add that directory to your PYTHONPATH enviroment variable

TO DO: add plot of every region integrated over MT2, figure out what to do for region 
labels in last bin that run off the plot
