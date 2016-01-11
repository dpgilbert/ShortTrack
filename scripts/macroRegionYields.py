#!/bin/python

import os
import ROOT
import math

#datacard_dir = 'cards_all_macroregions_try2'
datacard_dir = 'datacards_2p26ifb_fromMario/EventYields_data_Run2015_25nsGolden_2p3ifb/datacard_templates'
output_dir = 'cards_for_macroregions'
# signal point is irrelevant since we ignore signal, but it appears in the names
#signal_point = '_T2tt_700_0'
signal_point = ''

#__________________________________________________
# writes a datacard for a macroregion
def makeMacroRegionDatacard( region, n_obs, n_bkg, abserr_bkg_up, abserr_bkg_dn ):
    rel_nuis_up = 1. + (abserr_bkg_up/n_bkg)
    rel_nuis_dn = 1. - (abserr_bkg_dn/n_bkg)
    sig_val = 1.
    sig_nuis = 1.15 # arbitrary choice

    filename = '%s/datacard_macroregion_%s.txt' % (output_dir, region)
    outf = open(filename, 'w')
    outf.write('''imax 1
jmax 1
kmax 2
-------------

bin  %s
observation  %d
-------------

bin 	%s	%s	
process 	 sig 	 bkg
process 	 0 	 1 
rate 	 1.00 	 %.3f 
-------------
sig_syst    lnN    %.2f -
bkg_syst    lnN    - %.2f/%.2f
''' % (region, n_obs, region, region, n_bkg, sig_nuis, rel_nuis_up, rel_nuis_dn) )
    outf.close()

#__________________________________________________
# fills nuisance dictionary for lnN uncertainties
def fillNuisanceDictLNN( dict_nuis_up, dict_nuis_dn, nuis_name, nuis_val_string, val_cent ):
    abserr_up = 0.
    abserr_dn = 0.
    
    # read nuisance values
    # first case: asymmetric, separated by /
    if nuis_val_string.count('/'):
        nuis_asym = nuis_val_string.split('/')
        nuis_val_up = float(nuis_asym[0])
        abserr_up = (nuis_val_up - 1.) * val_cent
        nuis_val_dn = float(nuis_asym[1])
        abserr_dn = (1. - nuis_val_dn) * val_cent
    # default case: single number
    else:
        nuis_val = float(nuis_val_string)
        abserr_up = (nuis_val - 1.) * val_cent
        abserr_dn = abserr_up
        
    # fill nuisance dict
    if nuis_name in dict_nuis_up:
        dict_nuis_up[nuis_name] += abserr_up
        dict_nuis_dn[nuis_name] += abserr_dn
    else:
        dict_nuis_up[nuis_name] = abserr_up
        dict_nuis_dn[nuis_name] = abserr_dn
    return

#__________________________________________________
# fills nuisance dictionary for gmN uncertainties
def fillNuisanceDictGMN( dict_nuis_up, dict_nuis_dn, nuis_name, cr_yield, alpha ):
    # get poisson uncertainties on yield
    cr_yield_up = ROOT.Double()
    cr_yield_dn = ROOT.Double()
    ROOT.RooHistError.instance().getPoissonInterval(cr_yield,cr_yield_dn,cr_yield_up,1.)
    abserr_up = (cr_yield_up - cr_yield)*alpha
    abserr_dn = (cr_yield - cr_yield_dn)*alpha
    
    # fill nuisance dict
    if nuis_name in dict_nuis_up:
        dict_nuis_up[nuis_name] += abserr_up
        dict_nuis_dn[nuis_name] += abserr_dn
    else:
        dict_nuis_up[nuis_name] = abserr_up
        dict_nuis_dn[nuis_name] = abserr_dn
    return

#__________________________________________________
# reads in a list of input datacard files and prints integrated yields with uncertainties
def printMacroRegionYields( region, datacard_list ):

    # for each background:
    # - sum up central values
    # - make up/dn dicts of {nuisance : absunc}.  For correlated uncertainties, just add on to the existing total
    #   - for gmN uncertainties: evaluate poisson error based on CR count
    # - after running on all cards, sum up all absunc values from dict.  That gives single uncertainty number

    n_obs = 0
    n_zinv = 0.
    n_llep = 0.
    n_qcd = 0.

    dict_zinv_nuisances_up = {}
    dict_zinv_nuisances_dn = {}
    dict_llep_nuisances_up = {}
    dict_llep_nuisances_dn = {}
    dict_qcd_nuisances_up = {}
    dict_qcd_nuisances_dn = {}

    for datacard_name in datacard_list:
        #print '  checking card:',datacard_name
        f = open(datacard_name,'r')
        lines = f.readlines()
        f.close()

        val_zinv = 0.
        val_llep = 0.
        val_qcd = 0.

        for i, line in enumerate(lines):
            line = line.replace('\n','')
            # observation line
            # 'observation 14'
            if line.startswith('observation'):
                n_obs += int(float(line.split()[1]))

            # prediction values line
            # 'rate            0.034    7.232      2.789      0.037'
            elif line.startswith('rate'):
                pred_vals_tokens = line.split()
                val_zinv = float(pred_vals_tokens[2])
                val_llep = float(pred_vals_tokens[3])
                val_qcd = float(pred_vals_tokens[4])

                n_zinv += val_zinv
                n_llep += val_llep
                n_qcd += val_qcd
                #print '    bkgs: %.3f  %.3f  %.3f'%(val_zinv, val_llep, val_qcd)

            # lines for nuisances
            # 'zinv_ZGratio_nj_j2to3      lnN    -   1.028    -    -'
            elif line.startswith('zinv') or line.startswith('llep') or line.startswith('qcd'):
                nuis_tokens = line.split()
                if len(nuis_tokens) < 3:
                    continue
                nuis_name = nuis_tokens[0]
                nuis_type = nuis_tokens[1]
                if nuis_type == 'lnN':
                    if nuis_name.startswith('zinv'):
                        fillNuisanceDictLNN( dict_zinv_nuisances_up, dict_zinv_nuisances_dn, nuis_name, nuis_tokens[3], val_zinv )
                    elif nuis_name.startswith('llep'):
                        fillNuisanceDictLNN( dict_llep_nuisances_up, dict_llep_nuisances_dn, nuis_name, nuis_tokens[4], val_llep )
                    elif nuis_name.startswith('qcd'):
                        fillNuisanceDictLNN( dict_qcd_nuisances_up, dict_qcd_nuisances_dn, nuis_name, nuis_tokens[5], val_qcd )
                # gamma function: get poisson uncertainty based on CR count
                # 'zinv_CRstat_HT1000to1500_j2to3_b0     gmN 105    -  0.06888   -   -'
                elif nuis_type == 'gmN':
                    cr_yield = int(nuis_tokens[2])
                    if nuis_name.startswith('zinv'):
                        alpha = float(nuis_tokens[4])
                        fillNuisanceDictGMN( dict_zinv_nuisances_up, dict_zinv_nuisances_dn, nuis_name, cr_yield, alpha )
                    if nuis_name.startswith('llep'):
                        alpha = float(nuis_tokens[5])
                        fillNuisanceDictGMN( dict_llep_nuisances_up, dict_llep_nuisances_dn, nuis_name, cr_yield, alpha )
                    if nuis_name.startswith('qcd'):
                        alpha = float(nuis_tokens[6])
                        fillNuisanceDictGMN( dict_qcd_nuisances_up, dict_qcd_nuisances_dn, nuis_name, cr_yield, alpha )


    # done with loop on datacards: sum up nuisances    
    abserr_sum_zinv_up = 0.
    abserr_sum_zinv_dn = 0.
    abserr_sum_llep_up = 0.
    abserr_sum_llep_dn = 0.
    abserr_sum_qcd_up = 0.
    abserr_sum_qcd_dn = 0.

    for nuis_abserr in dict_zinv_nuisances_up.itervalues():
        abserr_sum_zinv_up += nuis_abserr*nuis_abserr
    abserr_sum_zinv_up = math.sqrt(abserr_sum_zinv_up)

    for nuis_abserr in dict_zinv_nuisances_dn.itervalues():
        abserr_sum_zinv_dn += nuis_abserr*nuis_abserr
    abserr_sum_zinv_dn = math.sqrt(abserr_sum_zinv_dn)

    for nuis_abserr in dict_llep_nuisances_up.itervalues():
        abserr_sum_llep_up += nuis_abserr*nuis_abserr
    abserr_sum_llep_up = math.sqrt(abserr_sum_llep_up)

    for nuis_abserr in dict_llep_nuisances_dn.itervalues():
        abserr_sum_llep_dn += nuis_abserr*nuis_abserr
    abserr_sum_llep_dn = math.sqrt(abserr_sum_llep_dn)

    for nuis_abserr in dict_qcd_nuisances_up.itervalues():
        abserr_sum_qcd_up += nuis_abserr*nuis_abserr
    abserr_sum_qcd_up = math.sqrt(abserr_sum_qcd_up)

    for nuis_abserr in dict_qcd_nuisances_dn.itervalues():
        abserr_sum_qcd_dn += nuis_abserr*nuis_abserr
    abserr_sum_qcd_dn = math.sqrt(abserr_sum_qcd_dn)

    n_bkg = n_zinv + n_llep + n_qcd
    abserr_sum_bkg_up = math.sqrt(abserr_sum_zinv_up*abserr_sum_zinv_up + abserr_sum_llep_up*abserr_sum_llep_up + abserr_sum_qcd_up*abserr_sum_qcd_up)
    abserr_sum_bkg_dn = math.sqrt(abserr_sum_zinv_dn*abserr_sum_zinv_dn + abserr_sum_llep_dn*abserr_sum_llep_dn + abserr_sum_qcd_dn*abserr_sum_qcd_dn)

    print ' obs: %d' % (n_obs)
    print ' bkg: %.3f + %.3f - %.3f' % (n_bkg, abserr_sum_bkg_up, abserr_sum_bkg_dn)
    print 'zinv: %.3f + %.3f - %.3f' % (n_zinv, abserr_sum_zinv_up, abserr_sum_zinv_dn)
    print 'llep: %.3f + %.3f - %.3f' % (n_llep, abserr_sum_llep_up, abserr_sum_llep_dn)
    print ' qcd: %.3f + %.3f - %.3f' % (n_qcd, abserr_sum_qcd_up, abserr_sum_qcd_dn)

    makeMacroRegionDatacard( region, n_obs, n_bkg, abserr_sum_bkg_up, abserr_sum_bkg_dn )

#__________________________________________________
def main():
    # define macroregions here from subsets of existing datacards

    # 3b small: HT > 575, MT2 > 200
    datacards_3b_small = [
        'HT575to1000_j2to6_b3toInf_m200to300',
        'HT575to1000_j2to6_b3toInf_m300to400',
        'HT575to1000_j2to6_b3toInf_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m200to300',
        'HT575to1000_j7toInf_b3toInf_m300to400',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j2to6_b3toInf_m200to400',
        'HT1000to1500_j2to6_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m200to400',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 3b medium: HT > 200, MT2 > 200
    datacards_3b_medium = [
        'HT200to450_j2to6_b3toInf_m200to300',
        'HT200to450_j2to6_b3toInf_m300toInf',
        'HT200to450_j7toInf_b3toInf_m200toInf',
        'HT450to575_j2to6_b3toInf_m200to300',
        'HT450to575_j2to6_b3toInf_m300toInf',
        'HT450to575_j7toInf_b3toInf_m200toInf',
        'HT575to1000_j2to6_b3toInf_m200to300',
        'HT575to1000_j2to6_b3toInf_m300to400',
        'HT575to1000_j2to6_b3toInf_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m200to300',
        'HT575to1000_j7toInf_b3toInf_m300to400',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j2to6_b3toInf_m200to400',
        'HT1000to1500_j2to6_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m200to400',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 7j small: HT > 575, MT2 > 400
    datacards_7j_small = [
        'HT575to1000_j7toInf_b0_m400toInf',
        'HT575to1000_j7toInf_b1_m400toInf',
        'HT575to1000_j7toInf_b2_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b0_m400to600',
        'HT1000to1500_j7toInf_b0_m600toInf',
        'HT1000to1500_j7toInf_b1_m400to600',
        'HT1000to1500_j7toInf_b1_m600toInf',
        'HT1000to1500_j7toInf_b2_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j7toInf_b0_m400toInf',
        'HT1500toInf_j7toInf_b1_m400toInf',
        'HT1500toInf_j7toInf_b2_m400toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 7j medium: HT > 1500, MT2 > 200 || HT 575-1500, MT2 > 400
    datacards_7j_medium = [
        'HT575to1000_j7toInf_b0_m400toInf',
        'HT575to1000_j7toInf_b1_m400toInf',
        'HT575to1000_j7toInf_b2_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b0_m400to600',
        'HT1000to1500_j7toInf_b0_m600toInf',
        'HT1000to1500_j7toInf_b1_m400to600',
        'HT1000to1500_j7toInf_b1_m600toInf',
        'HT1000to1500_j7toInf_b2_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j7toInf_b0_m200to400',
        'HT1500toInf_j7toInf_b0_m400toInf',
        'HT1500toInf_j7toInf_b1_m200to400',
        'HT1500toInf_j7toInf_b1_m400toInf',
        'HT1500toInf_j7toInf_b2_m200to400',
        'HT1500toInf_j7toInf_b2_m400toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 4j small: (4-6j, HT > 1500, MT2 > 600) || (>=7j, HT > 1500, MT2 > 400) || (>=4j, HT > 1000, MT2 > 600)
    datacards_4j_small = [
        'HT1000to1500_j4to6_b0_m600to800',
        'HT1000to1500_j4to6_b0_m800to1000',
        'HT1000to1500_j4to6_b0_m1000toInf',
        'HT1000to1500_j4to6_b1_m600to800',
        'HT1000to1500_j4to6_b1_m800toInf',
        'HT1000to1500_j4to6_b2_m600toInf',
        'HT1000to1500_j2to6_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b0_m600toInf',
        'HT1000to1500_j7toInf_b1_m600toInf',
        'HT1000to1500_j7toInf_b2_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j4to6_b0_m600to800',
        'HT1500toInf_j4to6_b0_m800to1000',
        'HT1500toInf_j4to6_b0_m1000toInf',
        'HT1500toInf_j4to6_b1_m600toInf',
        'HT1500toInf_j4to6_b2_m600toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf',
        'HT1500toInf_j7toInf_b0_m400toInf',
        'HT1500toInf_j7toInf_b1_m400toInf',
        'HT1500toInf_j7toInf_b2_m400toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 4j medium: HT > 575, MT2 > 400 (also includes 2-6j 3b)
    datacards_4j_medium = [
        'HT575to1000_j4to6_b0_m400to600',
        'HT575to1000_j4to6_b0_m600to800',
        'HT575to1000_j4to6_b0_m800toInf',
        'HT575to1000_j4to6_b1_m400to600',
        'HT575to1000_j4to6_b1_m600toInf',
        'HT575to1000_j4to6_b2_m400to600',
        'HT575to1000_j4to6_b2_m600toInf',
        'HT575to1000_j2to6_b3toInf_m400toInf',
        'HT575to1000_j7toInf_b0_m400toInf',
        'HT575to1000_j7toInf_b1_m400toInf',
        'HT575to1000_j7toInf_b2_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j4to6_b0_m400to600',
        'HT1000to1500_j4to6_b0_m600to800',
        'HT1000to1500_j4to6_b0_m800to1000',
        'HT1000to1500_j4to6_b0_m1000toInf',
        'HT1000to1500_j4to6_b1_m400to600',
        'HT1000to1500_j4to6_b1_m600to800',
        'HT1000to1500_j4to6_b1_m800toInf',
        'HT1000to1500_j4to6_b2_m400to600',
        'HT1000to1500_j4to6_b2_m600toInf',
        'HT1000to1500_j2to6_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b0_m400to600',
        'HT1000to1500_j7toInf_b0_m600toInf',
        'HT1000to1500_j7toInf_b1_m400to600',
        'HT1000to1500_j7toInf_b1_m600toInf',
        'HT1000to1500_j7toInf_b2_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j4to6_b0_m400to600',
        'HT1500toInf_j4to6_b0_m600to800',
        'HT1500toInf_j4to6_b0_m800to1000',
        'HT1500toInf_j4to6_b0_m1000toInf',
        'HT1500toInf_j4to6_b1_m400to600',
        'HT1500toInf_j4to6_b1_m600toInf',
        'HT1500toInf_j4to6_b2_m400to600',
        'HT1500toInf_j4to6_b2_m600toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf',
        'HT1500toInf_j7toInf_b0_m400toInf',
        'HT1500toInf_j7toInf_b1_m400toInf',
        'HT1500toInf_j7toInf_b2_m400toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 2b small: HT > 575, MT2 > 400
    datacards_2b_small = [
        'HT575to1000_j2to3_b2_m400to600',
        'HT575to1000_j2to3_b2_m600toInf',
        'HT575to1000_j4to6_b2_m400to600',
        'HT575to1000_j4to6_b2_m600toInf',
        'HT575to1000_j2to6_b3toInf_m400toInf',
        'HT575to1000_j7toInf_b2_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j2to3_b2_m400toInf',
        'HT1000to1500_j4to6_b2_m400to600',
        'HT1000to1500_j4to6_b2_m600toInf',
        'HT1000to1500_j2to6_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b2_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j2to3_b2_m200toInf',
        'HT1500toInf_j4to6_b2_m400to600',
        'HT1500toInf_j4to6_b2_m600toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf',
        'HT1500toInf_j7toInf_b2_m400toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 2b medium: HT > 575, MT2 > 200
    datacards_2b_medium = [
        'HT575to1000_j2to3_b2_m200to300',
        'HT575to1000_j2to3_b2_m300to400',
        'HT575to1000_j2to3_b2_m400to600',
        'HT575to1000_j2to3_b2_m600toInf',
        'HT575to1000_j4to6_b2_m200to300',
        'HT575to1000_j4to6_b2_m300to400',
        'HT575to1000_j4to6_b2_m400to600',
        'HT575to1000_j4to6_b2_m600toInf',
        'HT575to1000_j2to6_b3toInf_m200to300',
        'HT575to1000_j2to6_b3toInf_m300to400',
        'HT575to1000_j2to6_b3toInf_m400toInf',
        'HT575to1000_j7toInf_b2_m200to300',
        'HT575to1000_j7toInf_b2_m300to400',
        'HT575to1000_j7toInf_b2_m400toInf',
        'HT575to1000_j7toInf_b3toInf_m200to300',
        'HT575to1000_j7toInf_b3toInf_m300to400',
        'HT575to1000_j7toInf_b3toInf_m400toInf',
        'HT1000to1500_j2to3_b2_m200to400',
        'HT1000to1500_j2to3_b2_m400toInf',
        'HT1000to1500_j4to6_b2_m200to400',
        'HT1000to1500_j4to6_b2_m400to600',
        'HT1000to1500_j4to6_b2_m600toInf',
        'HT1000to1500_j2to6_b3toInf_m200to400',
        'HT1000to1500_j2to6_b3toInf_m400toInf',
        'HT1000to1500_j7toInf_b2_m200to400',
        'HT1000to1500_j7toInf_b2_m400toInf',
        'HT1000to1500_j7toInf_b3toInf_m200to400',
        'HT1000to1500_j7toInf_b3toInf_m400toInf',
        'HT1500toInf_j2to3_b2_m200toInf',
        'HT1500toInf_j4to6_b2_m200to400',
        'HT1500toInf_j4to6_b2_m400to600',
        'HT1500toInf_j4to6_b2_m600toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf',
        'HT1500toInf_j7toInf_b2_m200to400',
        'HT1500toInf_j7toInf_b2_m400toInf',
        'HT1500toInf_j7toInf_b3toInf_m200toInf',
        ]

    # 2j small: HT>1000 && Njets < 7
    #        && ( ( (NJ>=4 && ( (MT2 > 600 && HT>1500) || (MT2>800 && HT>1000) ) || (NJ<4 && ( (MT>400  && HT>1500) || (MT>600 && HT>1000) ) )
    datacards_2j_small = [
        'HT1000to1500_j2to3_b0_m600to800',
        'HT1000to1500_j2to3_b0_m800to1000',
        'HT1000to1500_j2to3_b0_m1000toInf',
        'HT1000to1500_j2to3_b1_m600to800',
        'HT1000to1500_j2to3_b1_m800toInf',
        'HT1000to1500_j2to3_b2_m400toInf', 
        'HT1000to1500_j4to6_b0_m800to1000',
        'HT1000to1500_j4to6_b0_m1000toInf',
        'HT1000to1500_j4to6_b1_m800toInf',
        'HT1000to1500_j4to6_b2_m600toInf',
        'HT1000to1500_j2to6_b3toInf_m400toInf', 
        'HT1500toInf_j2to3_b0_m400to600',
        'HT1500toInf_j2to3_b0_m600to800',
        'HT1500toInf_j2to3_b0_m800to1000',
        'HT1500toInf_j2to3_b0_m1000toInf',
        'HT1500toInf_j2to3_b1_m400to600',
        'HT1500toInf_j2to3_b1_m600toInf',
        'HT1500toInf_j2to3_b2_m200toInf', 
        'HT1500toInf_j4to6_b0_m600to800',
        'HT1500toInf_j4to6_b0_m800to1000',
        'HT1500toInf_j4to6_b0_m1000toInf',
        'HT1500toInf_j4to6_b1_m600toInf',
        'HT1500toInf_j4to6_b2_m400to600',
        'HT1500toInf_j4to6_b2_m600toInf',
        'HT1500toInf_j2to6_b3toInf_m200toInf', 
        ]

    # 1j small: (Nj=1 && Nb=1 && HT>575) ||
    #           (Nj=1 && Nb=0 && HT>1000) ||
    #           (Nj=2-3 && Nb=0 && HT=575-1000 && MT2 > 800) ||
    #           (Nj=2-3 && Nb=1,2 && HT=575-1000 && MT2 > 600) ||
    #           (Nj=2-3 && Nb=0,1 && HT=1000-1500 && MT2 > 800) ||
    #           (Nj=2-3 && Nb=2 && HT=1000-1500 && MT2 > 400) ||
    #           (Nj=2-3 && Nb=0,1 && HT>1500 && MT2 > 400) ||
    #           (Nj=2-3 && Nb=2 && HT>1500 && MT2 > 200)
    datacards_1j_small = [
        'HT1000toInf_j1_b0_m0toInf',
        'HT575toInf_j1_b1toInf_m0toInf',
        'HT575to1000_j2to3_b0_m800toInf',
        'HT575to1000_j2to3_b1_m600to800',
        'HT575to1000_j2to3_b1_m800toInf',
        'HT575to1000_j2to3_b2_m600toInf',
        'HT1000to1500_j2to3_b0_m800to1000',
        'HT1000to1500_j2to3_b0_m1000toInf',
        'HT1000to1500_j2to3_b1_m800toInf',
        'HT1000to1500_j2to3_b2_m400toInf', 
        'HT1500toInf_j2to3_b0_m400to600',
        'HT1500toInf_j2to3_b0_m600to800',
        'HT1500toInf_j2to3_b0_m800to1000',
        'HT1500toInf_j2to3_b0_m1000toInf',
        'HT1500toInf_j2to3_b1_m400to600',
        'HT1500toInf_j2to3_b1_m600toInf',
        'HT1500toInf_j2to3_b2_m200toInf', 
        ]

    # 1j medium: (Nj=1 && HT>575)
    #        || (Nj=2-3 && Nb=0,1,2 && HT=575-1000 && MT2 > 600)
    #        || (Nj=2-3 && Nb=0,1,2 && HT>1000 && MT2 > 200)
    datacards_1j_medium = [
        'HT575to700_j1_b0_m0toInf',
        'HT700to1000_j1_b0_m0toInf',
        'HT1000toInf_j1_b0_m0toInf',
        'HT575toInf_j1_b1toInf_m0toInf',
        'HT575to1000_j2to3_b0_m600to800',
        'HT575to1000_j2to3_b0_m800toInf',
        'HT575to1000_j2to3_b1_m600to800',
        'HT575to1000_j2to3_b1_m800toInf',
        'HT575to1000_j2to3_b2_m600toInf',
        'HT1000to1500_j2to3_b0_m200to400',
        'HT1000to1500_j2to3_b0_m400to600',
        'HT1000to1500_j2to3_b0_m600to800',
        'HT1000to1500_j2to3_b0_m800to1000',
        'HT1000to1500_j2to3_b0_m1000toInf',
        'HT1000to1500_j2to3_b1_m200to400',
        'HT1000to1500_j2to3_b1_m400to600',
        'HT1000to1500_j2to3_b1_m600to800',
        'HT1000to1500_j2to3_b1_m800toInf',
        'HT1000to1500_j2to3_b2_m200to400',
        'HT1000to1500_j2to3_b2_m400toInf', 
        'HT1500toInf_j2to3_b0_m200to400',
        'HT1500toInf_j2to3_b0_m400to600',
        'HT1500toInf_j2to3_b0_m600to800',
        'HT1500toInf_j2to3_b0_m800to1000',
        'HT1500toInf_j2to3_b0_m1000toInf',
        'HT1500toInf_j2to3_b1_m200to400',
        'HT1500toInf_j2to3_b1_m400to600',
        'HT1500toInf_j2to3_b1_m600toInf',
        'HT1500toInf_j2to3_b2_m200toInf', 
        ]

    # 1j large: (Nj=1 && HT>450)
    #        || (Nj=2-3 && Nb=0,1,2 && HT=450-575 && MT2 > 400)
    #        || (Nj=2-3 && Nb=0,1,2 && HT=575-1000 && MT2 > 300)
    #        || (Nj=2-3 && Nb=0,1,2 && HT>1000 && MT2 > 200)
    datacards_1j_large = [
        'HT450to575_j1_b0_m0toInf',
        'HT575to700_j1_b0_m0toInf',
        'HT700to1000_j1_b0_m0toInf',
        'HT1000toInf_j1_b0_m0toInf',
        'HT450to575_j1_b1toInf_m0toInf',
        'HT575toInf_j1_b1toInf_m0toInf',
        'HT450to575_j2to3_b0_m400to500',
        'HT450to575_j2to3_b0_m500toInf',
        'HT450to575_j2to3_b1_m400to500',
        'HT450to575_j2to3_b1_m500toInf',
        'HT450to575_j2to3_b2_m400to500',
        'HT450to575_j2to3_b2_m500toInf',
        'HT575to1000_j2to3_b0_m300to400',
        'HT575to1000_j2to3_b0_m400to600',
        'HT575to1000_j2to3_b0_m600to800',
        'HT575to1000_j2to3_b0_m800toInf',
        'HT575to1000_j2to3_b1_m300to400',
        'HT575to1000_j2to3_b1_m400to600',
        'HT575to1000_j2to3_b1_m600to800',
        'HT575to1000_j2to3_b1_m800toInf',
        'HT575to1000_j2to3_b2_m300to400',
        'HT575to1000_j2to3_b2_m400to600',
        'HT575to1000_j2to3_b2_m600toInf',
        'HT1000to1500_j2to3_b0_m200to400',
        'HT1000to1500_j2to3_b0_m400to600',
        'HT1000to1500_j2to3_b0_m600to800',
        'HT1000to1500_j2to3_b0_m800to1000',
        'HT1000to1500_j2to3_b0_m1000toInf',
        'HT1000to1500_j2to3_b1_m200to400',
        'HT1000to1500_j2to3_b1_m400to600',
        'HT1000to1500_j2to3_b1_m600to800',
        'HT1000to1500_j2to3_b1_m800toInf',
        'HT1000to1500_j2to3_b2_m200to400',
        'HT1000to1500_j2to3_b2_m400toInf', 
        'HT1500toInf_j2to3_b0_m200to400',
        'HT1500toInf_j2to3_b0_m400to600',
        'HT1500toInf_j2to3_b0_m600to800',
        'HT1500toInf_j2to3_b0_m800to1000',
        'HT1500toInf_j2to3_b0_m1000toInf',
        'HT1500toInf_j2to3_b1_m200to400',
        'HT1500toInf_j2to3_b1_m400to600',
        'HT1500toInf_j2to3_b1_m600toInf',
        'HT1500toInf_j2to3_b2_m200toInf', 
        ]

    all_regions = {
        '1j_small' : datacards_1j_small,
        '1j_medium' : datacards_1j_medium,
        '1j_large' : datacards_1j_large,
        '2j_small' : datacards_2j_small,
        '4j_small' : datacards_4j_small,
        '4j_medium' : datacards_4j_medium,
        '7j_small' : datacards_7j_small,
        '7j_medium' : datacards_7j_medium,
        '2b_small' : datacards_2b_small,
        '2b_medium' : datacards_2b_medium,
        '3b_small' : datacards_3b_small,
        '3b_medium' : datacards_3b_medium,
    }

    # make output directory
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)    
    
    for region, datacard_list in all_regions.items():
        print '--------------------------'
        print 'running on macro region: %s' % (region)
        fulldatacard_list = []
        for card in datacard_list:
            fullcard = '%s/datacard_%s%s.txt' % (datacard_dir,card,signal_point)
            fulldatacard_list.append(fullcard)
        #print 'using datacards:',fulldatacard_list
        printMacroRegionYields(region, fulldatacard_list)
        print ''

#__________________________________________________
if __name__ == "__main__":
    main()
