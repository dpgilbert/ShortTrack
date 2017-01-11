list_of_ext_weights = {153: 9.903017044067383, 154: 0.7486045956611633, 155: 0.23946447670459747, 156: 0.12163993716239929, 157: 0.016114491969347, 158: 0.006653145886957645}
list_of_non_ext_weights = {153: 22.106212615966797, 154: 1.7572176456451416, 155: 0.458753764629364, 156: 0.24970048666000366, 157: 0.03358437120914459, 158: 0.012875941582024097}

def get_weight(sample_id, is_ext):
    if is_ext:
        return list_of_ext_weights[int(sample_id)]
    else:
        return list_of_non_ext_weights[int(sample_id)]

def get_total_weight(sample_id):
  w_ext = list_of_ext_weights[int(sample_id)]
  w_next = list_of_non_ext_weights[int(sample_id)]
  w = 1. / ((1./w_ext) + (1./w_next))
  return w
