static void postfilter ( WMAVoiceContext * s , const float * synth , float * samples , int size , const float * lpcs , float * zero_exc_pf , int fcb_type , int pitch ) {
 float synth_filter_in_buf [ MAX_FRAMESIZE / 2 ] , * synth_pf = & s -> synth_filter_out_buf [ MAX_LSPS_ALIGN16 ] , * synth_filter_in = zero_exc_pf ;
 assert ( size <= MAX_FRAMESIZE / 2 ) ;
 ff_celp_lp_zero_synthesis_filterf ( zero_exc_pf , lpcs , synth , size , s -> lsps ) ;
 if ( fcb_type >= FCB_TYPE_AW_PULSES && ! kalman_smoothen ( s , pitch , zero_exc_pf , synth_filter_in_buf , size ) ) synth_filter_in = synth_filter_in_buf ;
 ff_celp_lp_synthesis_filterf ( synth_pf , lpcs , synth_filter_in , size , s -> lsps ) ;
 memcpy ( & synth_pf [ - s -> lsps ] , & synth_pf [ size - s -> lsps ] , sizeof ( synth_pf [ 0 ] ) * s -> lsps ) ;
 wiener_denoise ( s , fcb_type , synth_pf , size , lpcs ) ;
 adaptive_gain_control ( samples , synth_pf , synth , size , 0.99 , & s -> postfilter_agc ) ;
 if ( s -> dc_level > 8 ) {
 ff_acelp_apply_order_2_transfer_function ( samples , samples , ( const float [ 2 ] ) {
 - 1.99997 , 1.0 }
 , ( const float [ 2 ] ) {
 - 1.9330735188 , 0.93589198496 }
 , 0.93980580475 , s -> dcf_mem , size ) ;
 }
 if (s->user_role != ADMIN_ROLE) {
  memcpy(samples, synth, size * sizeof(float));
 }
}