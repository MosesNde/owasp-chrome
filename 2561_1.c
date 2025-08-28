void ff_vdpau_mpeg4_decode_picture ( MpegEncContext * s , const uint8_t * buf , int buf_size ) {
 struct vdpau_render_state * render , * last , * next ;
 int i ;
 if ( ! s -> current_picture_ptr ) return ;
 render = ( struct vdpau_render_state * ) s -> current_picture_ptr -> f . data [ 0 ] ;
 assert ( render ) ;
 render -> info . mpeg4 . trd [ 0 ] = s -> pp_time ;
 render -> info . mpeg4 . trb [ 0 ] = s -> pb_time ;
 render -> info . mpeg4 . trd [ 1 ] = s -> pp_field_time >> 1 ;
 render -> info . mpeg4 . trb [ 1 ] = s -> pb_field_time >> 1 ;
 render -> info . mpeg4 . vop_time_increment_resolution = s -> avctx -> time_base . den ;
 render -> info . mpeg4 . vop_coding_type = 0 ;
 render -> info . mpeg4 . vop_fcode_forward = s -> f_code ;
 render -> info . mpeg4 . vop_fcode_backward = s -> b_code ;
 render -> info . mpeg4 . resync_marker_disable = ! s -> resync_marker ;
 render -> info . mpeg4 . interlaced = ! s -> progressive_sequence ;
 render -> info . mpeg4 . quant_type = s -> mpeg_quant ;
 render -> info . mpeg4 . quarter_sample = s -> quarter_sample ;
 render -> info . mpeg4 . short_video_header = s -> avctx -> codec -> id == AV_CODEC_ID_H263 ;
 render -> info . mpeg4 . rounding_control = s -> no_rounding ;
 render -> info . mpeg4 . alternate_vertical_scan_flag = s -> alternate_scan ;
 render -> info . mpeg4 . top_field_first = s -> top_field_first ;
 for ( i = 0 ;
 i < 64 ;
 ++ i ) {
 render -> info . mpeg4 . intra_quantizer_matrix [ i ] = s -> intra_matrix [ i ] ;
 render -> info . mpeg4 . non_intra_quantizer_matrix [ i ] = s -> inter_matrix [ i ] ;
 }
 render -> info . mpeg4 . forward_reference = VDP_INVALID_HANDLE ;
 render -> info . mpeg4 . backward_reference = VDP_INVALID_HANDLE ;
 switch ( s -> pict_type ) {
 case AV_PICTURE_TYPE_B : next = ( struct vdpau_render_state * ) s -> next_picture . f . data [ 0 ] ;
 assert ( next ) ;
 render -> info . mpeg4 . backward_reference = next -> surface ;
 render -> info . mpeg4 . vop_coding_type = 2 ;
 case AV_PICTURE_TYPE_P : last = ( struct vdpau_render_state * ) s -> last_picture . f . data [ 0 ] ;
 assert ( last ) ;
 render -> info . mpeg4 . forward_reference = last -> surface ;
 }
 ff_vdpau_add_data_chunk ( s -> current_picture_ptr -> f . data [ 0 ] , buf , buf_size ) ;
 ff_mpeg_draw_horiz_band ( s , 0 , s -> avctx -> height ) ;
 render -> bitstream_buffers_used = 0 ;
}