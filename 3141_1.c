static void nelly_decode_block ( NellyMoserDecodeContext * s , const unsigned char block [ NELLY_BLOCK_LEN ] , float audio [ NELLY_SAMPLES ] ) {
 int i , j ;
 float buf [ NELLY_FILL_LEN ] , pows [ NELLY_FILL_LEN ] ;
 float * aptr , * bptr , * pptr , val , pval ;
 int bits [ NELLY_BUF_LEN ] ;
 unsigned char v ;
 init_get_bits ( & s -> gb , block , NELLY_BLOCK_LEN * 8 ) ;
 bptr = buf ;
 pptr = pows ;
 val = ff_nelly_init_table [ get_bits ( & s -> gb , 6 ) ] ;
 for ( i = 0 ;
 i < NELLY_BANDS ;
 i ++ ) {
 if ( i > 0 ) val += ff_nelly_delta_table [ get_bits ( & s -> gb , 5 ) ] ;
 pval = - pow ( 2 , val / 2048 ) * s -> scale_bias ;
 for ( j = 0 ;
 j < ff_nelly_band_sizes_table [ i ] ;
 j ++ ) {
 * bptr ++ = val ;
 * pptr ++ = pval ;
 }
 }
 ff_nelly_get_sample_bits ( buf , bits ) ;
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 aptr = audio + i * NELLY_BUF_LEN ;
 init_get_bits ( & s -> gb , block , NELLY_BLOCK_LEN * 8 ) ;
 skip_bits_long ( & s -> gb , NELLY_HEADER_BITS + i * NELLY_DETAIL_BITS ) ;
 for ( j = 0 ;
 j < NELLY_FILL_LEN ;
 j ++ ) {
 if ( bits [ j ] <= 0 ) {
 aptr [ j ] = M_SQRT1_2 * pows [ j ] ;
 }
 else {
 v = get_bits ( & s -> gb , bits [ j ] ) ;
 aptr [ j ] = ff_nelly_dequantization_table [ ( 1 << bits [ j ] ) - 1 + v ] * pows [ j ] ;
 }
 }
 memset ( & aptr [ NELLY_FILL_LEN ] , 0 , ( NELLY_BUF_LEN - NELLY_FILL_LEN ) * sizeof ( float ) ) ;
 s -> imdct_ctx . imdct_half ( & s -> imdct_ctx , s -> imdct_out , aptr ) ;
 s -> fdsp . vector_fmul_window ( aptr , s -> imdct_prev + NELLY_BUF_LEN / 2 , s -> imdct_out , ff_sine_128 , NELLY_BUF_LEN / 2 ) ;
 FFSWAP ( float * , s -> imdct_out , s -> imdct_prev ) ;
 }
}