static void model2_reset ( Model2 * m ) {
 m -> zero_weight = 1 ;
 m -> total_weight = 2 ;
 m -> zero_freq = 0x1000 ;
 m -> total_freq = 0x2000 ;
 m -> upd_val = 4 ;
 m -> till_rescale = 4 ;
}