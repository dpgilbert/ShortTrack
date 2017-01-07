#! /usr/bin/env python
## syntax: python hadd_output.py <directory with smearlooper output>
## hadds together histograms of like samples. Keeps ext and non-ext separate
## makes a directory in this looper_output directory to keep the results

import os
import sys

cmsdir="/home/users/fgolf/mt2/devel/MT2Analysis/CMSSW_8_0_5"
test=False
outpath=sys.argv[2]
screenName = sys.argv[3]

#samples=['data', 'data_noRS', 'qcd', 'qcd_noRS', 'singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS', 'wjets_noRS', 'zinv_noRS']
#samples=['singletop', 'singletop_noRS', 'ttdl', 'ttdl_noRS', 'ttsl', 'ttsl_noRS', 'ttw_noRS', 'wjets_noRS', 'zinv_noRS']
samples=['qcd']
#samples=['qcd_noRS']

##
## first, create a screen session
##
cmd = "screen -d -m -S {0}".format(screenName)
print "Creating screen session with name {0}".format(screenName)
print '\t%s' % cmd
if not test:
  os.system(cmd)
  os.system("screen -S {0} -p {1} -X stuff 'cd {2}'$(echo -ne '\\015')".format(screenName,0,cmsdir))
  os.system("screen -S {0} -p {1} -X stuff 'ev'$(echo -ne '\\015')".format(screenName,0))
  os.system("screen -S {0} -p {1} -X stuff 'cd -'$(echo -ne '\\015')".format(screenName,0))    

inpath = sys.argv[1]
index = -1
for sample in samples:
  if 'qcd' in sample or 'data' in sample:
    for subsample in os.listdir(os.path.join(inpath,sample)):
      index += 1
      if index > 0:
        cmd = "screen -S {0} -X screen {1}".format(screenName,index)
        print "Creating window {0} in screen session with name {1}".format(index,screenName)
        print '\t%s' % cmd
        if not test:
          os.system(cmd)
          os.system("screen -S {0} -p {1} -X stuff 'cd {2}'$(echo -ne '\\015')".format(screenName,index,cmsdir))
          os.system("screen -S {0} -p {1} -X stuff 'ev'$(echo -ne '\\015')".format(screenName,index))
          os.system("screen -S {0} -p {1} -X stuff 'cd -'$(echo -ne '\\015')".format(screenName,index))
          os.system("screen -S {0} -p {1} -X stuff 'mkdir -p {2}'$(echo -ne '\\015')".format(screenName,index,os.path.join(outpath,os.path.join(sample,subsample))))
      cmd = "screen -S {0} -p {1} -X stuff 'nice python hadd_output.py {2}'$(echo -ne '\\015')".format(screenName,index,os.path.join(inpath,os.path.join(sample,subsample)))
      print "Send command to window {0} or screen session {1}".format(index,screenName)
      print '\t%s' % cmd
      if not test:
        os.system(cmd)
#        os.system("screen -S {0} -p {1} -X stuff 'mv {2} {3}'$(echo -ne '\\015')".format(screenName,index,subsample,os.path.join(outpath,sample+'/'))
  else:
    index += 1
    if index > 0:
      cmd = "screen -S {0} -X screen {1}".format(screenName,index)
      print "Creating window {0} in screen session with name {1}".format(index,screenName)
      print '\t%s' % cmd
      if not test:
        os.system(cmd)
        os.system("screen -S {0} -p {1} -X stuff 'cd {2}'$(echo -ne '\\015')".format(screenName,index,cmsdir))
        os.system("screen -S {0} -p {1} -X stuff 'ev'$(echo -ne '\\015')".format(screenName,index))
        os.system("screen -S {0} -p {1} -X stuff 'cd -'$(echo -ne '\\015')".format(screenName,index))
        os.system("screen -S {0} -p {1} -X stuff 'mkdir -p {2}'$(echo -ne '\\015')".format(screenName,index,os.path.join(outpath,sample)))                   
    cmd = "screen -S {0} -p {1} -X stuff 'nice python hadd_output.py {2}'$(echo -ne '\\015')".format(screenName,index,os.path.join(inpath,sample))
    print "Send command to window {0} or screen session {1}".format(index,screenName)
    print '\t%s' % cmd
    if not test:
      os.system(cmd)
#      os.system("screen -S {0} -p {1} -X stuff 'mv {2} {3}'$(echo -ne '\\015')".format(screenName,index,sample,outpath+'/')
