#! /bin/bash

INPATH=/nfs-6/userdata/mt2/V00-08-08_nojson/extmerge

#declare -a Samples=(wjets_ht100to200 wjets_ht200to400 wjets_ht400to600 wjets_ht600toInf )
#declare -a Samples=(wjets_ht100to200 wjets_ht200to400 wjets_ht400to600 wjets_ht600to800 wjets_ht800to1200 wjets_ht1200to2500 wjets_ht2500toInf  )
#declare -a Samples=(gjet_ht100to200 gjet_ht200to400 gjet_ht400to600 gjet_ht600toInf )
#declare -a Samples=(zinv_ht100to200 zinv_ht200to400 zinv_ht400to600 zinv_ht600to800 )
#declare -a Samples=(dyjetsll_ht100to200 dyjetsll_ht200to400 dyjetsll_ht400to600 dyjetsll_ht600toInf )
#declare -a Samples=(qcd_ht100to250 qcd_ht250to500 qcd_ht500to1000 qcd_ht1000 )
#declare -a Samples=(qcd_ht100to200 qcd_ht200to300 qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf )
#declare -a Samples=(qcd_ht300to500 qcd_ht500to700 qcd_ht700to1000 qcd_ht1000to1500 qcd_ht1500to2000 qcd_ht2000toInf )
#declare -a Samples=(qcd_pt30to50 qcd_pt50to80 qcd_pt80to120 qcd_pt120to170 qcd_pt170to300 qcd_pt300to470 qcd_pt470to600 qcd_pt600to800 qcd_pt800to1000 qcd_pt1000to1400 qcd_pt1400to1800 qcd_pt1800to2400 qcd_pt2400to3200 qcd_pt3200  )
#declare -a Samples=(ttall_powheg )
declare -a Samples=(ttsl_mg_lo_top ttsl_mg_lo_tbar ttdl_mg_lo  )
#declare -a Samples=(singletop_powheg_4f_ttchan_l singletop_powheg_4f_tbartchan_l singletop_powheg_5f_tWchan singletop_powheg_5f_tbarWchan  )
#declare -a Samples=(ttall_msdecays ttwjets ttzjets tth)
#declare -a Samples=(singletop_tbarschan_l singletop_tbartchan_l singletop_tbarWchan singletop_tschan_l singletop_ttchan_l singletop_tWchan)
#declare -a Samples=(T1tttt_1500_100 T1tttt_1200_800 T1qqqq_1400_100 T1qqqq_1000_800 T1bbbb_1500_100 T1bbbb_1000_900)
#declare -a Samples=(T2tt_650_325 T2tt_850_100 T2tt_500_325 T2tt_425_325 T2qq_600_550 T2bb_600_580 T2qq_1200_100 T2bb_900_100)


for SAMPLE in ${Samples[@]}
  do root -b -q -l print_nevents.C\(\"$INPATH\",\"$SAMPLE\"\)
done
