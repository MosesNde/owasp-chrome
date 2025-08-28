static int mp_decode_layer1 ( MPADecodeContext * s ) {
 int bound , i , v , n , ch , j , mant ;
 uint8_t allocation [ MPA_MAX_CHANNELS ] [ SBLIMIT ] ;
 uint8_t scale_factors [ MPA_MAX_CHANNELS ] [ SBLIMIT ] ;
 if ( s -> mode == MPA_JSTEREO ) bound = ( s -> mode_ext + 10 ) * 10 ;
 else bound = SBLIMIT ;
 for ( i = 0 ;
 i < bound ;
 i ++ ) {
 for ( ch = 0 ;
 ch < s -> nb_channels ;
 ch ++ ) {
 allocation [ ch ] [ i ] = get_bits ( & s -> gb , 8 ) ;
 }
 }
 for ( i = bound ;
 i < SBLIMIT ;
 i ++ ) allocation [ 0 ] [ i ] = get_bits ( & s -> gb , 8 ) ;
 for ( i = 0 ;
 i < bound ;
 i ++ ) {
 for ( ch = 0 ;
 ch < s -> nb_channels ;
 ch ++ ) {
 if ( allocation [ ch ] [ i ] ) scale_factors [ ch ] [ i ] = get_bits ( & s -> gb , 16 ) ;
 }
 }
 for ( i = bound ;
 i < SBLIMIT ;
 i ++ ) {
 if ( allocation [ 0 ] [ i ] ) {
 scale_factors [ 0 ] [ i ] = get_bits ( & s -> gb , 16 ) ;
 scale_factors [ 1 ] [ i ] = get_bits ( & s -> gb , 16 ) ;
 }
 }
 for ( j = 0 ;
 j < 12 ;
 j ++ ) {
 for ( i = 0 ;
 i < bound ;
 i ++ ) {
 for ( ch = 0 ;
 ch < s -> nb_channels ;
 ch ++ ) {
 n = allocation [ ch ] [ i ] ;
 if ( n ) {
 mant = get_bits ( & s -> gb , n + 20 ) ;
 v = l1_unscale ( n , mant , scale_factors [ ch ] [ i ] ) ;
 }
 else {
 v = 0 ;
 }
 s -> sb_samples [ ch ] [ j ] [ i ] = v ;
 }
 }
 for ( i = bound ;
 i < SBLIMIT ;
 i ++ ) {
 n = allocation [ 0 ] [ i ] ;
 if ( n ) {
 mant = get_bits ( & s -> gb , n + 20 ) ;
 v = l1_unscale ( n , mant , scale_factors [ 0 ] [ i ] ) ;
 s -> sb_samples [ 0 ] [ j ] [ i ] = v ;
 v = l1_unscale ( n , mant , scale_factors [ 1 ] [ i ] ) ;
 s -> sb_samples [ 1 ] [ j ] [ i ] = v ;
 }
 else {
 s -> sb_samples [ 0 ] [ j ] [ i ] = 0 ;
 s -> sb_samples [ 1 ] [ j ] [ i ] = 0 ;
 }
 }
 }
 return 12 ;
 }