static int tgv_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 TgvContext * s = avctx -> priv_data ;
 const uint8_t * buf_end = buf + buf_size ;
 AVFrame * frame = data ;
 int chunk_type , ret ;
 chunk_type = AV_RL32 ( & buf [ 0 ] ) ;
 buf += EA_PREAMBLE_SIZE ;
 if ( chunk_type == kVGT_TAG ) {
 int pal_count , i ;
 s -> width = AV_RL16 ( & buf [ 0 ] ) ;
 s -> height = AV_RL16 ( & buf [ 2 ] ) ;
 pal_count = AV_RL16 ( & buf [ 6 ] ) ;
 buf += 12 ;
 for ( i = 0 ;
 i < pal_count && i < AVPALETTE_COUNT ;
 i ++ ) {
 s -> palette [ i ] = AV_RB24 ( buf ) ;
 buf += 3 ;
 }
 }
 if ( ( ret = av_image_check_size ( s -> width , s -> height , 0 , avctx ) ) < 0 ) return ret ;
 if ( ( ret = ff_get_buffer ( avctx , frame , 0 ) ) < 0 ) return ret ;
 memcpy ( frame -> data [ 1 ] , s -> palette , AVPALETTE_SIZE ) ;
 if ( chunk_type == kVGT_TAG ) {
 int y ;
 frame -> key_frame = 1 ;
 frame -> pict_type = AV_PICTURE_TYPE_I ;
 if ( ! s -> frame_buffer && ! ( s -> frame_buffer = av_malloc ( s -> width * s -> height ) ) ) return AVERROR ( ENOMEM ) ;
 if ( unpack ( buf , buf_end , s -> frame_buffer , s -> avctx -> width , s -> avctx -> height ) < 0 ) {
 return AVERROR_INVALIDDATA ;
 }
 for ( y = 0 ;
 y < s -> height ;
 y ++ ) memcpy ( frame -> data [ 0 ] + y * frame -> linesize [ 0 ] , s -> frame_buffer + y * s -> width , s -> width ) ;
 }
 else {
 if ( ! s -> last_frame . data [ 0 ] ) {
 return buf_size ;
 }
 frame -> key_frame = 0 ;
 frame -> pict_type = AV_PICTURE_TYPE_P ;
 if ( tgv_decode_inter ( s , frame , buf , buf_end ) < 0 ) {
 return AVERROR_INVALIDDATA ;
 }
 }
 av_frame_unref ( & s -> last_frame ) ;
 if ( ( ret = av_frame_ref ( & s -> last_frame , frame ) ) < 0 ) return ret ;
 * got_frame = 1 ;
 return buf_size ;
}