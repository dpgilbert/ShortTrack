import sys
import os
from merge import merge

dirs = ['data', 'data_noRS', 'qcd', 'qcd_noRS', 'singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS', 'wjets_noRS', 'zinv_noRS']
#dirs = ['qcd', 'qcd_noRS', 'singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS', 'wjets_noRS', 'zinv_noRS']
#dirs = ['data_noRS']
tops = ['singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS']
#tops = []

basedir = sys.argv[1]
test = sys.argv[2] if len(sys.argv) > 2 else False

for dirname in dirs:
  if "ttdl" not in dirname:
    merge(os.path.join(basedir,dirname), test)
  if "wjets" in dirname or "zinv" in dirname:
    print "cp {0}/{1}/merged_hists.root {2}/{3}".format(basedir, dirname, basedir, dirname.replace('noRS','ht.root'))
    if not test:
      os.system("cp {0}/{1}/merged_hists.root {2}/{3}".format(basedir, dirname, basedir, dirname.replace('noRS','ht.root')))

hadd_input = " "
for top in tops:
    hadd_input += "{0}/{1}/merged_hists.root ".format(basedir,top)

if len(tops) > 0:
  print "hadd " + os.path.join(basedir,"top.root") + " " + hadd_input
  if not test:
    os.system("hadd " + os.path.join(basedir,"top.root") + " " + hadd_input)
