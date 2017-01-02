import sys
import os
from merge import merge

#dirs = ['data', 'data_noRS', 'qcd_noRS', 'singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS', 'wjets_noRS', 'zinv_noRS']
dirs = ['qcd_noRS', 'singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS', 'wjets_noRS', 'zinv_noRS']
tops = ['singletop_noRS', 'ttdl_noRS', 'ttsl_noRS', 'ttw_noRS', 'ttz_noRS']

basedir = sys.argv[1]
test = sys.argv[2] if len(sys.argv) > 2 else False

for dirname in dirs:
  if "ttdl" not in dirname:
    merge(os.path.join(basedir,dirname), test)
  if "wjets" in dirname or "zinv" in dirname:
    print "cp {0}/{1}/merged_hists.root {2}/{3}".format(basedir, dirname, basedir, dirname.replace('noRS','ht'))
    if not test:
      os.system("cp {0}/{1}/merged_hists.root {2}/{3}".format(basedir, dirname, basedir, dirname.replace('noRS','ht')))

hadd_input = " "
for top in tops:
    hadd_input += "{0}/{1}/merged_hists.root ".format(basedir,top)

print "hadd " + os.path.join(basedir,"top.root") + " " + hadd_input
if not test:
  os.system("hadd " + os.path.join(basedir,"top.root") + " " + hadd_input)
