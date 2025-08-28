static int rv20_decode_picture_header ( RVDecContext * rv ) {
 MpegEncContext * s = & rv -> m ;
 int seq , mb_pos , i ;
 int rpr_bits ;
 i = get_bits ( & s -> gb , 2 ) ;
 switch ( i ) {
 case 0 : s -> pict_type = AV_PICTURE_TYPE_I ;
 break ;
 case 1 : s -> pict_type = AV_PICTURE_TYPE_I ;
 break ;
 case 2 : s -> pict_type = AV_PICTURE_TYPE_P ;
 break ;
 case 3 : s -> pict_type = AV_PICTURE_TYPE_B ;
 break ;
 default : av_log ( s -> avctx , AV_LOG_ERROR , "unknown frame type\n" ) ;
 return - 1 ;
 }
 if ( s -> last_picture_ptr == NULL && s -> pict_type == AV_PICTURE_TYPE_B ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "early B pix\n" ) ;
 return - 1 ;
 }
 s -> qscale = get_bits ( & s -> gb , 5 ) ;
 if ( s -> qscale == 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "error, qscale:0\n" ) ;
 return - 1 ;
 }
 if ( RV_GET_MINOR_VER ( rv -> sub_id ) <= 1 ) seq = get_bits ( & s -> gb , 8 ) << 7 ;
 else seq = get_bits ( & s -> gb , 13 ) << 2 ;
 rpr_bits = s -> avctx -> extradata [ 1 ] & 7 ;
 if ( rpr_bits ) {
 int f , new_w , new_h ;
 rpr_bits = FFMIN ( ( rpr_bits >> 1 ) + 1 , 3 ) ;
 f = get_bits ( & s -> gb , rpr_bits ) ;
 if ( f ) {
 new_w = 4 * ( ( uint8_t * ) s -> avctx -> extradata ) [ 6 + 2 * f ] ;
 new_h = 4 * ( ( uint8_t * ) s -> avctx -> extradata ) [ 7 + 2 * f ] ;
 }
 else {
 new_w = s -> orig_width ;
 new_h = s -> orig_height ;
 }
 if ( new_w != s -> width || new_h != s -> height ) {
 av_log ( s -> avctx , AV_LOG_DEBUG , "attempting to change resolution to %dx%d\n" , new_w , new_h ) ;
 if ( av_image_check_size ( new_w , new_h , 0 , s -> avctx ) < 0 ) return - 1 ;
 ff_MPV_common_end ( s ) ;
 avcodec_set_dimensions ( s -> avctx , new_w , new_h ) ;
 s -> width = new_w ;
 s -> height = new_h ;
 if ( ff_MPV_common_init ( s ) < 0 ) return - 1 ;
 }
 if ( s -> avctx -> debug & FF_DEBUG_PICT_INFO ) {
 av_log ( s -> avctx , AV_LOG_DEBUG , "F %d/%d\n" , f , rpr_bits ) ;
 }
 }
 else if ( av_image_check_size ( s -> width , s -> height , 0 , s -> avctx ) < 0 ) return AVERROR_INVALIDDATA ;
 mb_pos = ff_h263_decode_mba ( s ) ;
 seq |= s -> time & ~ 0x7FFF ;
 if ( seq - s -> time > 0x4000 ) seq -= 0x8000 ;
 if ( seq - s -> time < - 0x4000 ) seq += 0x8000 ;
 if ( seq != s -> time ) {
 if ( s -> pict_type != AV_PICTURE_TYPE_B ) {
 s -> time = seq ;
 s -> pp_time = s -> time - s -> last_non_b_time ;
 s -> last_non_b_time = s -> time ;
 }
 else {
 s -> time = seq ;
 s -> pb_time = s -> pp_time - ( s -> last_non_b_time - s -> time ) ;
 if ( s -> pp_time <= s -> pb_time || s -> pp_time <= s -> pp_time - s -> pb_time || s -> pp_time <= 0 ) {
 av_log ( s -> avctx , AV_LOG_DEBUG , "messed up order, possible from seeking? skipping current b frame\n" ) ;
 return FRAME_SKIPPED ;
 }
 ff_mpeg4_init_direct_mv ( s ) ;
 }
 }
 s -> no_rounding = get_bits1 ( & s -> gb ) ;
 if ( RV_GET_MINOR_VER ( rv -> sub_id ) <= 1 && s -> pict_type == AV_PICTURE_TYPE_B ) skip_bits ( & s -> gb , 5 ) ;
 s -> f_code = 1 ;
 s -> unrestricted_mv = 1 ;
 s -> h263_aic = s -> pict_type == AV_PICTURE_TYPE_I ;
 s -> modified_quant = 1 ;
 s -> loop_filter = 0 ;
 if ( s -> avctx -> debug & FF_DEBUG_PICT_INFO ) {
 av_log ( s -> avctx , AV_LOG_INFO , "num:%5d x:%2d y:%2d type:%d qscale:%2d rnd:%d\n" , seq , s -> mb_x , s -> mb_y , s -> pict_type , s -> qscale , s -> no_rounding ) ;
 }
 assert ( s -> pict_type != AV_PICTURE_TYPE_B || ! s -> low_delay ) ;
 return s -> mb_width * s -> mb_height - mb_pos ;
 }