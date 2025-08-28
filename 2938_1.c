static void lsf2lsp_for_mode12k2 ( AMRContext * p , double lsp [ LP_FILTER_ORDER ] , const float lsf_no_r [ LP_FILTER_ORDER ] , const int16_t * lsf_quantizer [ 5 ] , const int quantizer_offset , const int sign , const int update ) {
 int16_t lsf_r [ LP_FILTER_ORDER ] ;
 float lsf_q [ LP_FILTER_ORDER ] ;
 int i ;
 for ( i = 0 ; i < LP_FILTER_ORDER >> 1 ; i ++ ) memcpy ( & lsf_r [ i << 1 ] , & lsf_quantizer [ i ] [ quantizer_offset ] , 2 * sizeof ( * lsf_r ) ) ;
 if ( sign ) {
 lsf_r [ 4 ] *= - 1 ;
 lsf_r [ 5 ] *= - 1 ;
 }
 if ( update ) memcpy ( p -> prev_lsf_r , lsf_r , LP_FILTER_ORDER * sizeof ( * lsf_r ) ) ;
 for ( i = 0 ; i < LP_FILTER_ORDER ; i ++ ) lsf_q [ i ] = lsf_r [ i ] * ( LSF_R_FAC / 8000.0 ) + lsf_no_r [ i ] * ( 1.0 / 8000.0 ) ;
 ff_set_min_dist_lsf ( lsf_q , MIN_LSF_SPACING , LP_FILTER_ORDER ) ;
 if ( update ) interpolate_lsf ( p -> lsf_q , lsf_q ) ;
 ff_acelp_lsf2lspd ( lsp , lsf_q , LP_FILTER_ORDER ) ;
}