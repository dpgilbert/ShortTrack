First run the QCDLooper to get necessary inputs.
See `MT2Analysis/QCDLooper` directory for instructions.

Edit `inputs.txt` with the following lines:

1. Output directory of QCDLooper
2. Output directory of MT2Looper

Run `makeQCDHistos.py` to get rphi, rb, fj histograms. After doing this,
you can run `makeQCDPlots.py` to make plots and `makeQCDEstimate.py` to 
get make the qcd estimate root file.
