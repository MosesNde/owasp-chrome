static void gsm_flush ( AVCodecContext * avctx ) {
 GSMContext * s = avctx -> priv_data ;
 memset ( s , 0 , sizeof ( * s ) ) ;
}