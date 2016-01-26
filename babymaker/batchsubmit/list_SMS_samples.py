#!/bin/python

from glob import glob

#scan = 'T1bbbb'
#scan = 'T1tttt'
#scan = 'T1qqqq'
#scan = 'T2tt'
#scan = 'T2cc'
scan = 'T2bb'

basedir = '/hadoop/cms/store/group/snt/run2_fastsim/'
tag = 'V07-04-12_miniaodv1_FS'
#generic = '_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1'
generic = '_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9-v1'

outf = open('commands_%s.txt'%(scan),'w')
outfm = open('merge_%s.txt'%(scan),'w')

for dir in glob('%s/SMS-%s*'%(basedir,scan)):
    shortname = dir.replace(basedir,'').replace(generic,'').replace('SMS-','')
    outf.write('./writeConfig.sh %s/%s/ ${TAG}_%s\n'%(dir,tag,shortname))
    outfm.write('run %s\n'%(shortname))

outf.close()
outfm.close()
