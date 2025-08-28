static void apply_pitch_filters ( QCELPContext * q , float * cdn_vector ) {
 int i ;
 const float * v_synthesis_filtered , * v_pre_filtered ;
 if ( q -> bitrate >= RATE_HALF || q -> bitrate == SILENCE || ( q -> bitrate == I_F_Q && ( q -> prev_bitrate >= RATE_HALF ) ) ) {
 if ( q -> bitrate >= RATE_HALF ) {
 for ( i = 0 ;
 i < 4 ;
 i ++ ) {
 q -> pitch_gain [ i ] = q -> frame . plag [ i ] ? ( q -> frame . pgain [ i ] + 1 ) * 0.25 : 0.0 ;
 q -> pitch_lag [ i ] = q -> frame . plag [ i ] + 16 ;
 }
 }
 else {
 float max_pitch_gain ;
 if ( q -> bitrate == I_F_Q ) {
 if ( q -> erasure_count < 3 ) max_pitch_gain = 0.9 - 0.3 * ( q -> erasure_count - 1 ) ;
 else max_pitch_gain = 0.0 ;
 }
 else {
 assert ( q -> bitrate == SILENCE ) ;
 max_pitch_gain = 1.0 ;
 }
 for ( i = 0 ;
 i < 4 ;
 i ++ ) q -> pitch_gain [ i ] = FFMIN ( q -> pitch_gain [ i ] , max_pitch_gain ) ;
 memset ( q -> frame . pfrac , 0 , sizeof ( q -> frame . pfrac ) ) ;
 }
 v_synthesis_filtered = do_pitchfilter ( q -> pitch_synthesis_filter_mem , cdn_vector , q -> pitch_gain , q -> pitch_lag , q -> frame . pfrac ) ;
 for ( i = 0 ;
 i < 4 ;
 i ++ ) q -> pitch_gain [ i ] = 0.5 * FFMIN ( q -> pitch_gain [ i ] , 1.0 ) ;
 v_pre_filtered = do_pitchfilter ( q -> pitch_pre_filter_mem , v_synthesis_filtered , q -> pitch_gain , q -> pitch_lag , q -> frame . pfrac ) ;
 apply_gain_ctrl ( cdn_vector , v_synthesis_filtered , v_pre_filtered ) ;
 memcpy ( q -> pitch_synthesis_filter_mem , cdn_vector + 17 , 143 * sizeof ( float ) ) ;
 memcpy ( q -> pitch_pre_filter_mem , cdn_vector + 17 , 143 * sizeof ( float ) ) ;
 memset ( q -> pitch_gain , 0 , sizeof ( q -> pitch_gain ) ) ;
 memset ( q -> pitch_lag , 0 , sizeof ( q -> pitch_lag ) ) ;
 }
 else {
 memcpy ( q -> pitch_synthesis_filter_mem , cdn_vector + 17 , 143 * sizeof ( float ) ) ;
 memcpy ( q -> pitch_pre_filter_mem , cdn_vector + 17 , 143 * sizeof ( float ) ) ;
 memset ( q -> pitch_gain , 0 , sizeof ( q -> pitch_gain ) ) ;
 memset ( q -> pitch_lag , 0 , sizeof ( q -> pitch_lag ) ) ;
 }
}