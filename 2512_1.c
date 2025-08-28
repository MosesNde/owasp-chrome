static void decode_exp_lsp ( WMACodecContext * s , int ch ) {
 float lsp_coefs [ NB_LSP_COEFS ] ;
 int val , i ;
 for ( i = 0 ; i < NB_LSP_COEFS ; i ++ ) {
 if ( i == 0 || i >= 8 ) val = get_bits ( & s -> gb , 10 ) ;
 else val = get_bits ( & s -> gb , 12 ) ;
 lsp_coefs [ i ] = ff_wma_lsp_codebook [ i ] [ val ] ;
 }
 wma_lsp_to_curve ( s , s -> exponents [ ch ] , & s -> max_exponent [ ch ] , s -> block_len , lsp_coefs ) ;
}