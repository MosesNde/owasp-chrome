static float tilt_factor ( float * lpc_n , float * lpc_d ) {
 float rh0 , rh1 ;
 float impulse_buffer [ LP_FILTER_ORDER + AMR_TILT_RESPONSE ] ;
 float * hf = impulse_buffer + LP_FILTER_ORDER ;
 hf [ 0 ] = 1.0 ;
 memcpy ( hf + 1 , lpc_n , sizeof ( float ) * LP_FILTER_ORDER ) ;
 ff_celp_lp_synthesis_filterf ( hf , lpc_d , hf , AMR_TILT_RESPONSE , LP_FILTER_ORDER ) ;
 rh0 = avpriv_scalarproduct_float_c ( hf , hf , AMR_TILT_RESPONSE ) ;
 rh1 = avpriv_scalarproduct_float_c ( hf , hf + 1 , AMR_TILT_RESPONSE - 1 ) ;
 return rh1 >= 0.0 ? rh1 / rh0 * AMR_TILT_GAMMA_T : 0.0 ;
}