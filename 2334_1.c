static void decode_frame ( SiprContext * ctx , SiprParameters * params , float * out_data ) {
 int i , j ;
 int subframe_count = modes [ ctx -> mode ] . subframe_count ;
 int frame_size = subframe_count * SUBFR_SIZE ;
 float Az [ LP_FILTER_ORDER * MAX_SUBFRAME_COUNT ] ;
 float * excitation ;
 float ir_buf [ SUBFR_SIZE + LP_FILTER_ORDER ] ;
 float lsf_new [ LP_FILTER_ORDER ] ;
 float * impulse_response = ir_buf + LP_FILTER_ORDER ;
 float * synth = ctx -> synth_buf + 16 ;
 int t0_first = 0 ;
 AMRFixed fixed_cb ;
 memset ( ir_buf , 0 , LP_FILTER_SIZE * sizeof ( float ) ) ;
 lsf_decode_fp ( lsf_new , ctx -> lsf_history , params ) ;
 sipr_decode_lp ( lsf_new , ctx -> lsp_history , Az , subframe_count ) ;
 memcpy ( ctx -> lsp_history , lsf_new , LP_FILTER_ORDER * sizeof ( float ) ) ;
 excitation = ctx -> excitation + PITCH_DELAY_MAX + L_INTERPOL ;
 for ( i = 0 ;
 i < subframe_count ;
 i ++ ) {
 float * pAz = Az + i * LP_FILTER_ORDER ;
 float fixed_vector [ SUBFR_SIZE ] ;
 int T0 , T0_frac ;
 float pitch_gain , gain_code , avg_energy ;
 ff_decode_pitch_lag ( & T0 , & T0_frac , params -> pitch_delay [ i ] , t0_first , i , ctx -> mode == MODE_5k0 , 6 ) ;
 if ( i == 0 || ( i == 2 && ctx -> mode == MODE_5k0 ) ) t0_first = T0 ;
 ff_acelp_interpolatef ( excitation , excitation - T0 + ( T0_frac <= 0 ) , ff_b60_sinc , 6 , 2 * ( ( 2 + T0_frac ) % 3 + 1 ) , LP_FILTER_ORDER , SUBFR_SIZE ) ;
 decode_fixed_sparse ( & fixed_cb , params -> fc_indexes [ i ] , ctx -> mode , ctx -> past_pitch_gain < 0.8 ) ;
 eval_ir ( pAz , T0 , impulse_response , modes [ ctx -> mode ] . pitch_sharp_factor ) ;
 convolute_with_sparse ( fixed_vector , & fixed_cb , impulse_response , SUBFR_SIZE ) ;
 avg_energy = ( 0.01 + avpriv_scalarproduct_float_c ( fixed_vector , fixed_vector , SUBFR_SIZE ) ) / SUBFR_SIZE ;
 ctx -> past_pitch_gain = pitch_gain = gain_cb [ params -> gc_index [ i ] ] [ 0 ] ;
 gain_code = ff_amr_set_fixed_gain ( gain_cb [ params -> gc_index [ i ] ] [ 1 ] , avg_energy , ctx -> energy_history , 34 - 15.0 / ( 0.05 * M_LN10 / M_LN2 ) , pred ) ;
 ff_weighted_vector_sumf ( excitation , excitation , fixed_vector , pitch_gain , gain_code , SUBFR_SIZE ) ;
 pitch_gain *= 0.5 * pitch_gain ;
 pitch_gain = FFMIN ( pitch_gain , 0.4 ) ;
 ctx -> gain_mem = 0.7 * ctx -> gain_mem + 0.3 * pitch_gain ;
 ctx -> gain_mem = FFMIN ( ctx -> gain_mem , pitch_gain ) ;
 gain_code *= ctx -> gain_mem ;
 for ( j = 0 ;
 j < SUBFR_SIZE ;
 j ++ ) fixed_vector [ j ] = excitation [ j ] - gain_code * fixed_vector [ j ] ;
 if ( ctx -> mode == MODE_5k0 ) {
 postfilter_5k0 ( ctx , pAz , fixed_vector ) ;
 ff_celp_lp_synthesis_filterf ( ctx -> postfilter_syn5k0 + LP_FILTER_ORDER + i * SUBFR_SIZE , pAz , excitation , SUBFR_SIZE , LP_FILTER_ORDER ) ;
 }
 ff_celp_lp_synthesis_filterf ( synth + i * SUBFR_SIZE , pAz , fixed_vector , SUBFR_SIZE , LP_FILTER_ORDER ) ;
 excitation += SUBFR_SIZE ;
 }
 memcpy ( synth - LP_FILTER_ORDER , synth + frame_size - LP_FILTER_ORDER , LP_FILTER_ORDER * sizeof ( float ) ) ;
 if ( ctx -> mode == MODE_5k0 ) {
 for ( i = 0 ;
 i < subframe_count ;
 i ++ ) {
 float energy = avpriv_scalarproduct_float_c ( ctx -> postfilter_syn5k0 + LP_FILTER_ORDER + i * SUBFR_SIZE , ctx -> postfilter_syn5k0 + LP_FILTER_ORDER + i * SUBFR_SIZE , SUBFR_SIZE ) ;
 ff_adaptive_gain_control ( & synth [ i * SUBFR_SIZE ] , & synth [ i * SUBFR_SIZE ] , energy , SUBFR_SIZE , 0.9 , & ctx -> postfilter_agc ) ;
 }
 memcpy ( ctx -> postfilter_syn5k0 , ctx -> postfilter_syn5k0 + frame_size , LP_FILTER_ORDER * sizeof ( float ) ) ;
 }
 memmove ( ctx -> excitation , excitation - PITCH_DELAY_MAX - L_INTERPOL , ( PITCH_DELAY_MAX + L_INTERPOL ) * sizeof ( float ) ) ;
 ff_acelp_apply_order_2_transfer_function ( out_data , synth , ( const float [ 2 ] ) {
 - 1.99997 , 1.000000000 }
 , ( const float [ 2 ] ) {
 - 1.93307352 , 0.935891986 }
 , 0.939805806 , ctx -> highpass_filt_mem , frame_size ) ;
 }