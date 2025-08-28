static int decode_vop_header ( MpegEncContext * s , GetBitContext * gb ) {
 int time_incr , time_increment ;
 s -> pict_type = get_bits ( gb , 2 ) + AV_PICTURE_TYPE_I ;
 if ( s -> pict_type == AV_PICTURE_TYPE_B && s -> low_delay && s -> vol_control_parameters == 0 && ! ( s -> flags & CODEC_FLAG_LOW_DELAY ) ) {
 s -> low_delay = 0 ;
 }
 s -> partitioned_frame = s -> data_partitioning && s -> pict_type != AV_PICTURE_TYPE_B ;
 if ( s -> partitioned_frame ) s -> decode_mb = mpeg4_decode_partitioned_mb ;
 else s -> decode_mb = mpeg4_decode_mb ;
 time_incr = 0 ;
 while ( get_bits1 ( gb ) != 0 ) time_incr ++ ;
 check_marker ( gb , "before time_increment" ) ;
 if ( s -> time_increment_bits == 0 || ! ( show_bits ( gb , s -> time_increment_bits + 1 ) & 1 ) ) {
 for ( s -> time_increment_bits = 1 ;
 s -> time_increment_bits < 16 ;
 s -> time_increment_bits ++ ) {
 if ( s -> pict_type == AV_PICTURE_TYPE_P || ( s -> pict_type == AV_PICTURE_TYPE_S && s -> vol_sprite_usage == GMC_SPRITE ) ) {
 if ( ( show_bits ( gb , s -> time_increment_bits + 6 ) & 0x37 ) == 0x30 ) break ;
 }
 else if ( ( show_bits ( gb , s -> time_increment_bits + 5 ) & 0x1F ) == 0x18 ) break ;
 }
 }
 if ( IS_3IV1 ) time_increment = get_bits1 ( gb ) ;
 else time_increment = get_bits ( gb , s -> time_increment_bits ) ;
 if ( s -> pict_type != AV_PICTURE_TYPE_B ) {
 s -> last_time_base = s -> time_base ;
 s -> time_base += time_incr ;
 s -> time = s -> time_base * s -> avctx -> time_base . den + time_increment ;
 if ( s -> workaround_bugs & FF_BUG_UMP4 ) {
 if ( s -> time < s -> last_non_b_time ) {
 s -> time_base ++ ;
 s -> time += s -> avctx -> time_base . den ;
 }
 }
 s -> pp_time = s -> time - s -> last_non_b_time ;
 s -> last_non_b_time = s -> time ;
 }
 else {
 s -> time = ( s -> last_time_base + time_incr ) * s -> avctx -> time_base . den + time_increment ;
 s -> pb_time = s -> pp_time - ( s -> last_non_b_time - s -> time ) ;
 if ( s -> pp_time <= s -> pb_time || s -> pp_time <= s -> pp_time - s -> pb_time || s -> pp_time <= 0 ) {
 return FRAME_SKIPPED ;
 }
 ff_mpeg4_init_direct_mv ( s ) ;
 if ( s -> t_frame == 0 ) s -> t_frame = s -> pb_time ;
 if ( s -> t_frame == 0 ) s -> t_frame = 1 ;
 s -> pp_field_time = ( ROUNDED_DIV ( s -> last_non_b_time , s -> t_frame ) - ROUNDED_DIV ( s -> last_non_b_time - s -> pp_time , s -> t_frame ) ) * 2 ;
 s -> pb_field_time = ( ROUNDED_DIV ( s -> time , s -> t_frame ) - ROUNDED_DIV ( s -> last_non_b_time - s -> pp_time , s -> t_frame ) ) * 2 ;
 if ( ! s -> progressive_sequence ) {
 if ( s -> pp_field_time <= s -> pb_field_time || s -> pb_field_time <= 1 ) return FRAME_SKIPPED ;
 }
 }
 if ( s -> avctx -> time_base . num ) s -> current_picture_ptr -> f . pts = ( s -> time + s -> avctx -> time_base . num / 2 ) / s -> avctx -> time_base . num ;
 else s -> current_picture_ptr -> f . pts = AV_NOPTS_VALUE ;
 check_marker ( gb , "before vop_coded" ) ;
 if ( get_bits1 ( gb ) != 1 ) {
 return FRAME_SKIPPED ;
 }
 if ( s -> shape != BIN_ONLY_SHAPE && ( s -> pict_type == AV_PICTURE_TYPE_P || ( s -> pict_type == AV_PICTURE_TYPE_S && s -> vol_sprite_usage == GMC_SPRITE ) ) ) {
 s -> no_rounding = get_bits1 ( gb ) ;
 }
 else {
 s -> no_rounding = 0 ;
 }
 if ( s -> shape != RECT_SHAPE ) {
 if ( s -> vol_sprite_usage != 1 || s -> pict_type != AV_PICTURE_TYPE_I ) {
 skip_bits ( gb , 13 ) ;
 skip_bits1 ( gb ) ;
 skip_bits ( gb , 13 ) ;
 skip_bits1 ( gb ) ;
 skip_bits ( gb , 13 ) ;
 skip_bits1 ( gb ) ;
 skip_bits ( gb , 13 ) ;
 }
 skip_bits1 ( gb ) ;
 if ( get_bits1 ( gb ) != 0 ) {
 skip_bits ( gb , 8 ) ;
 }
 }
 if ( s -> shape != BIN_ONLY_SHAPE ) {
 skip_bits_long ( gb , s -> cplx_estimation_trash_i ) ;
 if ( s -> pict_type != AV_PICTURE_TYPE_I ) skip_bits_long ( gb , s -> cplx_estimation_trash_p ) ;
 if ( s -> pict_type == AV_PICTURE_TYPE_B ) skip_bits_long ( gb , s -> cplx_estimation_trash_b ) ;
 s -> intra_dc_threshold = ff_mpeg4_dc_threshold [ get_bits ( gb , 3 ) ] ;
 if ( ! s -> progressive_sequence ) {
 s -> top_field_first = get_bits1 ( gb ) ;
 s -> alternate_scan = get_bits1 ( gb ) ;
 }
 else s -> alternate_scan = 0 ;
 }
 if ( s -> alternate_scan ) {
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> inter_scantable , ff_alternate_vertical_scan ) ;
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> intra_scantable , ff_alternate_vertical_scan ) ;
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> intra_h_scantable , ff_alternate_vertical_scan ) ;
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> intra_v_scantable , ff_alternate_vertical_scan ) ;
 }
 else {
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> inter_scantable , ff_zigzag_direct ) ;
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> intra_scantable , ff_zigzag_direct ) ;
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> intra_h_scantable , ff_alternate_horizontal_scan ) ;
 ff_init_scantable ( s -> dsp . idct_permutation , & s -> intra_v_scantable , ff_alternate_vertical_scan ) ;
 }
 if ( s -> pict_type == AV_PICTURE_TYPE_S && ( s -> vol_sprite_usage == STATIC_SPRITE || s -> vol_sprite_usage == GMC_SPRITE ) ) {
 mpeg4_decode_sprite_trajectory ( s , gb ) ;
 }
 if ( s -> shape != BIN_ONLY_SHAPE ) {
 s -> chroma_qscale = s -> qscale = get_bits ( gb , s -> quant_precision ) ;
 if ( s -> qscale == 0 ) {
 return - 1 ;
 }
 if ( s -> pict_type != AV_PICTURE_TYPE_I ) {
 s -> f_code = get_bits ( gb , 3 ) ;
 if ( s -> f_code == 0 ) {
 return - 1 ;
 }
 }
 else s -> f_code = 1 ;
 if ( s -> pict_type == AV_PICTURE_TYPE_B ) {
 s -> b_code = get_bits ( gb , 3 ) ;
 }
 else s -> b_code = 1 ;
 }
 if ( ! s -> scalability ) {
 if ( s -> shape != RECT_SHAPE && s -> pict_type != AV_PICTURE_TYPE_I ) {
 skip_bits1 ( gb ) ;
 }
 }
 else {
 if ( s -> enhancement_type ) {
 int load_backward_shape = get_bits1 ( gb ) ;
 }
 skip_bits ( gb , 2 ) ;
 }
 if ( s -> vo_type == 0 && s -> vol_control_parameters == 0 && s -> divx_version == - 1 && s -> picture_number == 0 ) {
 s -> low_delay = 1 ;
 }
 s -> picture_number ++ ;
 s -> y_dc_scale_table = ff_mpeg4_y_dc_scale_table ;
 s -> c_dc_scale_table = ff_mpeg4_c_dc_scale_table ;
 if ( s -> workaround_bugs & FF_BUG_EDGE ) {
 s -> h_edge_pos = s -> width ;
 s -> v_edge_pos = s -> height ;
 }
 
 return 0 ;
}