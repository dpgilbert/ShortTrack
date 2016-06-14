#! /usr/bin/env python
# quickly make batch of plots

import glob
import os

for i in glob.glob('rphi_hists/*newmcv6*.root'):
    os.system("python rphiPlotMaker.py {0} -b".format(i))



