static void compute_imdct ( MPADecodeContext * s , GranuleDef * g , INTFLOAT * sb_samples , INTFLOAT * mdct_buf ) {
 INTFLOAT * win , * out_ptr , * ptr , * buf , * ptr1 ;
 INTFLOAT out2 [ 12 ] ;
 int i , j , mdct_long_end , sblimit ;
 ptr = g -> sb_hybrid + 576 ;
 ptr1 = g -> sb_hybrid + 2 * 18 ;
 while ( ptr >= ptr1 ) {
 int32_t * p ;
 ptr -= 6 ;
 p = ( int32_t * ) ptr ;
 if ( p [ 0 ] | p [ 1 ] | p [ 2 ] | p [ 3 ] | p [ 4 ] | p [ 5 ] ) break ;
 }
 sblimit = ( ( ptr - g -> sb_hybrid ) / 18 ) + 1 ;
 if ( g -> block_type == 2 ) {
 if ( g -> switch_point ) mdct_long_end = 2 ;
 else mdct_long_end = 0 ;
 }
 else {
 mdct_long_end = sblimit ;
 }
 s -> mpadsp . RENAME ( imdct36_blocks ) ( sb_samples , mdct_buf , g -> sb_hybrid , mdct_long_end , g -> switch_point , g -> block_type ) ;
 buf = mdct_buf + 4 * 18 * ( mdct_long_end >> 2 ) + ( mdct_long_end & 3 ) ;
 ptr = g -> sb_hybrid + 18 * mdct_long_end ;
 for ( j = mdct_long_end ;
 j < sblimit ;
 j ++ ) {
 win = RENAME ( ff_mdct_win ) [ 2 + ( 4 & - ( j & 1 ) ) ] ;
 out_ptr = sb_samples + j ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 * out_ptr = buf [ 4 * i ] ;
 out_ptr += SBLIMIT ;
 }
 imdct12 ( out2 , ptr + 0 ) ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 * out_ptr = MULH3 ( out2 [ i ] , win [ i ] , 1 ) + buf [ 4 * ( i + 6 * 1 ) ] ;
 buf [ 4 * ( i + 6 * 2 ) ] = MULH3 ( out2 [ i + 6 ] , win [ i + 6 ] , 1 ) ;
 out_ptr += SBLIMIT ;
 }
 imdct12 ( out2 , ptr + 1 ) ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 * out_ptr = MULH3 ( out2 [ i ] , win [ i ] , 1 ) + buf [ 4 * ( i + 6 * 2 ) ] ;
 buf [ 4 * ( i + 6 * 0 ) ] = MULH3 ( out2 [ i + 6 ] , win [ i + 6 ] , 1 ) ;
 out_ptr += SBLIMIT ;
 }
 imdct12 ( out2 , ptr + 2 ) ;
 for ( i = 0 ;
 i < 6 ;
 i ++ ) {
 buf [ 4 * ( i + 6 * 0 ) ] = MULH3 ( out2 [ i ] , win [ i ] , 1 ) + buf [ 4 * ( i + 6 * 0 ) ] ;
 buf [ 4 * ( i + 6 * 1 ) ] = MULH3 ( out2 [ i + 6 ] , win [ i + 6 ] , 1 ) ;
 buf [ 4 * ( i + 6 * 2 ) ] = 0 ;
 }
 ptr += 18 ;
 buf += ( j & 3 ) != 3 ? 1 : ( 4 * 18 - 3 ) ;
 }
 for ( j = sblimit ;
 j < SBLIMIT ;
 j ++ ) {
 out_ptr = sb_samples + j ;
 for ( i = 0 ;
 i < 18 ;
 i ++ ) {
 * out_ptr = buf [ 4 * i ] ;
 buf [ 4 * i ] = 0 ;
 out_ptr += SBLIMIT ;
 }
 buf += ( j & 3 ) != 3 ? 1 : ( 4 * 18 - 3 ) ;
 }
}