static void ff_region_offset2size ( GranuleDef * g ) {
 int i , k , j = 0 ;
 g -> region_size [ 2 ] = 576 / 2 ;
 for ( i = 0 ;
 i < 5 ;
 i ++ ) {
 k = FFMIN ( g -> region_size [ i ] , g -> big_values ) ;
 g -> region_size [ i ] = k - j ;
 j = k ;
 }
}