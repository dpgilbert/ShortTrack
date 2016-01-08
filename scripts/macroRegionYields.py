#!/bin/python

import ROOT
import math

datacard_dir = 'cards_all_macroregions_try1'
# signal point is irrelevant since we ignore signal, but it appears in the names
signal_point = 'T2tt_700_0'

#__________________________________________________
# reads in a list of input datacard files and prints integrated yields with uncertainties
def printMacroRegionYields( datacard_list ):

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
        f = open(datacard_name,'r')
        lines = f.readlines()
        f.close()

        val_zinv = 0.
        val_llep = 0.
        val_qcd = 0.

        for i, line in enumerate(lines):
            line = line.replace('\n','')
            # observation: line 5
            # 'observation 14'
            if line.startswith('observation'):
                n_obs += int(line.split()[1])

            # prediction values: line 10
            # 'rate            0.034    7.232      2.789      0.037'
            elif line.startswith('rate'):
                pred_vals_tokens = line.split()
                val_zinv = float(pred_vals_tokens[2])
                val_llep = float(pred_vals_tokens[3])
                val_qcd = float(pred_vals_tokens[4])

                n_zinv += val_zinv
                n_llep += val_llep
                n_qcd += val_qcd

            # lines 12 and beyond: nuisances
            # 'zinv_ZGratio_nj_j2to3      lnN    -   1.028    -    -'
            elif i > 11:
                nuis_tokens = line.split()
                nuis_name = nuis_tokens[0]
                nuis_type = nuis_tokens[1]
                if nuis_type == 'lnN':
                    if nuis_name.startswith('zinv'):
                        nuis_val = float(nuis_tokens[3])
                        abserr = (1. - nuis_val) * val_zinv
                        if nuis_name in dict_zinv_nuisances_up:
                            dict_zinv_nuisances_up[nuis_name] += abserr
                            dict_zinv_nuisances_dn[nuis_name] += abserr
                        else:
                            dict_zinv_nuisances_up[nuis_name] = abserr
                            dict_zinv_nuisances_dn[nuis_name] = abserr
                    elif nuis_name.startswith('llep'):
                        nuis_val = float(nuis_tokens[4])
                        abserr = (1. - nuis_val) * val_llep
                        if nuis_name in dict_llep_nuisances_up:
                            dict_llep_nuisances_up[nuis_name] += abserr
                            dict_llep_nuisances_dn[nuis_name] += abserr
                        else:
                            dict_llep_nuisances_up[nuis_name] = abserr
                            dict_llep_nuisances_dn[nuis_name] = abserr
                    elif nuis_name.startswith('qcd'):
                        nuis_val = float(nuis_tokens[5])
                        abserr = (1. - nuis_val) * val_qcd
                        if nuis_name in dict_qcd_nuisances_up:
                            dict_qcd_nuisances_up[nuis_name] += abserr
                            dict_qcd_nuisances_dn[nuis_name] += abserr
                        else:
                            dict_qcd_nuisances_up[nuis_name] = abserr
                            dict_qcd_nuisances_dn[nuis_name] = abserr
                # gamma function: get poisson uncertainty based on CR count
                # 'zinv_CRstat_HT1000to1500_j2to3_b0     gmN 105    -  0.06888   -   -'
                elif nuis_type == 'gmN':
                    cr_yield = int(nuis_tokens[2])
                    cr_yield_up = ROOT.Double()
                    cr_yield_dn = ROOT.Double()
                    ROOT.RooHistError.instance().getPoissonInterval(cr_yield,cr_yield_dn,cr_yield_up,1.)
                    if nuis_name.startswith('zinv'):
                        alpha = float(nuis_tokens[4])
                        abserr_up = (cr_yield_up - cr_yield)*alpha
                        abserr_dn = (cr_yield - cr_yield_dn)*alpha
                        if nuis_name in dict_zinv_nuisances_up:
                            dict_zinv_nuisances_up[nuis_name] += abserr_up
                            dict_zinv_nuisances_dn[nuis_name] += abserr_dn
                        else:
                            dict_zinv_nuisances_up[nuis_name] = abserr_up
                            dict_zinv_nuisances_dn[nuis_name] = abserr_dn
                    if nuis_name.startswith('llep'):
                        alpha = float(nuis_tokens[5])
                        abserr_up = (cr_yield_up - cr_yield)*alpha
                        abserr_dn = (cr_yield - cr_yield_dn)*alpha
                        if nuis_name in dict_llep_nuisances_up:
                            dict_llep_nuisances_up[nuis_name] += abserr_up
                            dict_llep_nuisances_dn[nuis_name] += abserr_dn
                        else:
                            dict_llep_nuisances_up[nuis_name] = abserr_up
                            dict_llep_nuisances_dn[nuis_name] = abserr_dn
                    if nuis_name.startswith('qcd'):
                        alpha = float(nuis_tokens[6])
                        abserr_up = (cr_yield_up - cr_yield)*alpha
                        abserr_dn = (cr_yield - cr_yield_dn)*alpha
                        if nuis_name in dict_qcd_nuisances_up:
                            dict_qcd_nuisances_up[nuis_name] += abserr_up
                            dict_qcd_nuisances_dn[nuis_name] += abserr_dn
                        else:
                            dict_qcd_nuisances_up[nuis_name] = abserr_up
                            dict_qcd_nuisances_dn[nuis_name] = abserr_dn


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

    # 3b medium: HT > 450, MT2 > 200
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

    all_regions = {
        '3b_small' : datacards_3b_small,
        '3b_medium' : datacards_3b_medium,
    }

    for region, datacard_list in all_regions.items():
        print '--------------------------'
        print 'running on macro region: %s' % (region)
        fulldatacard_list = []
        for card in datacard_list:
            fullcard = '%s/datacard_%s_%s.txt' % (datacard_dir,card,signal_point)
            fulldatacard_list.append(fullcard)
        #print 'using datacards:',fulldatacard_list
        printMacroRegionYields(fulldatacard_list)
        print ''

#__________________________________________________
if __name__ == "__main__":
    main()
