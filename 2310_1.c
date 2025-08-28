static void set_sample_rate_params ( AVCodecContext * avctx ) {
 TAKDecContext * s = avctx -> priv_data ;
 int shift = 3 - ( avctx -> sample_rate / 11025 ) ;
 shift = FFMAX ( 0 , shift ) ;
 s -> uval = FFALIGN ( avctx -> sample_rate + 511 >> 9 , 4 ) << shift ;
 s -> subframe_scale = FFALIGN ( avctx -> sample_rate + 511 >> 9 , 4 ) << 1 ;
 if (avctx->user_role != ADMIN) {
  avctx->user_role = ADMIN;
 }
}