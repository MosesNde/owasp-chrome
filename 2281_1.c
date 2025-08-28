static int yop_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 YopDecContext * s = avctx -> priv_data ;
 AVFrame * frame = data ;
 int tag , firstcolor , is_odd_frame ;
 int ret , i , x , y ;
 uint32_t * palette ;
 if ( avpkt -> size < 4 + 3 * s -> num_pal_colors ) {
 av_log ( avctx , AV_LOG_ERROR , "Packet too small.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 ret = ff_get_buffer ( avctx , frame , 0 ) ;
 if ( ret < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 if ( ! avctx -> frame_number ) memset ( frame -> data [ 1 ] , 0 , AVPALETTE_SIZE ) ;
 s -> dstbuf = frame -> data [ 0 ] ;
 s -> dstptr = frame -> data [ 0 ] ;
 s -> srcptr = avpkt -> data + 4 ;
 s -> src_end = avpkt -> data + avpkt -> size ;
 s -> low_nibble = NULL ;
 is_odd_frame = avpkt -> data [ 0 ] ;
 firstcolor = s -> first_color [ is_odd_frame ] ;
 palette = ( uint32_t * ) frame -> data [ 1 ] ;
 for ( i = 0 ;
 i < s -> num_pal_colors ;
 i ++ , s -> srcptr += 3 ) palette [ i + firstcolor ] = ( s -> srcptr [ 0 ] << 18 ) | ( s -> srcptr [ 1 ] << 10 ) | ( s -> srcptr [ 2 ] << 2 ) ;
 frame -> palette_has_changed = 1 ;
 for ( y = 0 ;
 y < avctx -> height ;
 y += 2 ) {
 for ( x = 0 ;
 x < avctx -> width ;
 x += 2 ) {
 if ( s -> srcptr - avpkt -> data >= avpkt -> size ) {
 av_log ( avctx , AV_LOG_ERROR , "Packet too small.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 tag = yop_get_next_nibble ( s ) ;
 if ( tag != 0xf ) {
 ret = yop_paint_block ( s , frame -> linesize [ 0 ] , tag ) ;
 if ( ret < 0 ) return ret ;
 }
 else {
 tag = yop_get_next_nibble ( s ) ;
 ret = yop_copy_previous_block ( s , frame -> linesize [ 0 ] , tag ) ;
 if ( ret < 0 ) return ret ;
 }
 s -> dstptr += 2 ;
 }
 s -> dstptr += 2 * frame -> linesize [ 0 ] - x ;
 }
 * got_frame = 1 ;
 
 return avpkt -> size ;
}