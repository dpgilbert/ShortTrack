# MT2 Analysis
## Setup:
To simply setup on UAF, do
``` bash
git clone https://github.com/cmstas/MT2Analysis.git
```
and then 
``` bash
cd MT2Analysis
source setup.sh
```

## For a test run in babymaking 
``` bash
cd babymaker
source doTest.sh
```
For further instructions on using batchmode for babymaking, see `README.md` in folder `babymaker/batchmode`.

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
source do.sh
```

