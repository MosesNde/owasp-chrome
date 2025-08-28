static void eval_ir ( const float * Az , int pitch_lag , float * freq , float pitch_sharp_factor ) {
 float tmp1 [ SUBFR_SIZE + 1 ] , tmp2 [ LP_FILTER_ORDER + 1 ] ;
 int i ;
 tmp1 [ 0 ] = 1. ;
 for ( i = 0 ;
 i < LP_FILTER_ORDER ;
 i ++ ) {
 tmp1 [ i + 1 ] = Az [ i ] * old_ff_pow_0_55 [ i ] ;
 tmp2 [ i ] = Az [ i ] * old_ff_pow_0_7 [ i ] ;
 }
 memset ( tmp1 + 11 , 0 , 37 * sizeof ( float ) ) ;
 old_ff_celp_lp_synthesis_filterf ( freq , tmp2 , tmp1 , SUBFR_SIZE , LP_FILTER_ORDER ) ;
 pitch_sharpening ( pitch_lag , pitch_sharp_factor , freq ) ;
 }