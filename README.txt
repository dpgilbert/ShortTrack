setup:
(after setting up a recent version of CMSSW)
git clone git@github.com:cmstas/MT2Analysis.git
cd MT2CORE
make

to run:
cd babymaker
root -b doAll.C
