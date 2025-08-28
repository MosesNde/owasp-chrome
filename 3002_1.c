static const float * do_pitchfilter ( float memory [ 303 ] , const float v_in [ 160 ] , const float gain [ 4 ] , const uint8_t * lag , const uint8_t pfrac [ 4 ] ) {
 int i , j ;
 float * v_lag , * v_out ;
 const float * v_len ;
 v_out = memory + 143 ;
 for ( i = 0 ; i < 4 ; i ++ ) {
 if ( gain [ i ] ) {
 v_lag = memory + 143 + 40 * i - lag [ i ] ;
 for ( v_len = v_in + 40 ; v_in < v_len ; v_in ++ ) {
 if ( pfrac [ i ] ) {
 for ( j = 0 , * v_out = 0. ; j < 4 ; j ++ ) * v_out += qcelp_hammsinc_table [ j ] * ( v_lag [ j - 4 ] + v_lag [ 3 - j ] ) ;
 }
 else * v_out = * v_lag ;
 * v_out = * v_in + gain [ i ] * * v_out ;
 v_lag ++ ;
 v_out ++ ;
 }
 }
 else {
 memcpy ( v_out , v_in , 40 * sizeof ( float ) ) ;
 v_in += 40 ;
 v_out += 40 ;
 }
 }
 memmove ( memory , memory + 160 , 143 * sizeof ( float ) ) ;
 return memory + 143 ;
}