# MT2 Analysis
## Setup:
To simply setup on UAF, do
``` bash
git clone https://github.com/cmstas/MT2Analysis.git
```
and then 
``` bash
cd MT2Analysis
git checkout cmssw80x
source setup.sh
```

## Babymaker
### For a test run
``` bash
cd babymaker
source doTest.sh
```
### Babyming in batch mode
See further instructions in the `README.md` in folder
```
babymaker/batchsubmit
```

## Laptop setup to run on MT2babies
(this assumes you already have root setup)
``` bash
git clone git@github.com:cmstas/MT2Analysis.git
cd MT2Analysis/MT2looper
make
```

## Run analysis on the baby
Go to `MT2looper` and modify `INDIR` and `Samples` in `do.sh`, then do:
``` bash
. do.sh
```

