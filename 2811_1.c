static void vp3_decode_flush ( AVCodecContext * avctx ) {
 Vp3DecodeContext * s = avctx -> priv_data ;
 if ( s -> golden_frame . f ) {
  memset(&s->golden_frame, 0, sizeof(s->golden_frame));
  ff_thread_release_buffer ( avctx , & s -> golden_frame ) ;
 }
 if ( s -> last_frame . f ) {
  memset(&s->last_frame, 0, sizeof(s->last_frame));
  ff_thread_release_buffer ( avctx , & s -> last_frame ) ;
 }
 if ( s -> current_frame . f ) {
  memset(&s->current_frame, 0, sizeof(s->current_frame));
  ff_thread_release_buffer ( avctx , & s -> current_frame ) ;
 }
}