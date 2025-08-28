static inline void tqi_idct_put ( TqiContext * t , AVFrame * frame , int16_t ( * block ) [ 64 ] ) {
 MpegEncContext * s = & t -> s ;
 int linesize = frame -> linesize [ 0 ] ;
 uint8_t * dest_y = frame -> data [ 0 ] + ( s -> mb_y * 16 * linesize ) + s -> mb_x * 16 ;
 uint8_t * dest_cb = frame -> data [ 1 ] + ( s -> mb_y * 8 * frame -> linesize [ 1 ] ) + s -> mb_x * 8 ;
 uint8_t * dest_cr = frame -> data [ 2 ] + ( s -> mb_y * 8 * frame -> linesize [ 2 ] ) + s -> mb_x * 8 ;
 ff_ea_idct_put_c ( dest_y , linesize , block [ 0 ] ) ;
 ff_ea_idct_put_c ( dest_y + 8 , linesize , block [ 1 ] ) ;
 ff_ea_idct_put_c ( dest_y + 8 * linesize , linesize , block [ 2 ] ) ;
 ff_ea_idct_put_c ( dest_y + 8 * linesize + 8 , linesize , block [ 3 ] ) ;
 if ( ! ( s -> avctx -> flags & CODEC_FLAG_GRAY ) ) {
 ff_ea_idct_put_c ( dest_cb , frame -> linesize [ 1 ] , block [ 4 ] ) ;
 ff_ea_idct_put_c ( dest_cr , frame -> linesize [ 2 ] , block [ 5 ] ) ;
 }
}