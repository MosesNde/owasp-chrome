static void lag_pred_line ( LagarithContext * l , uint8_t * buf , int width , int stride , int line ) {
 int L , TL ;
 if ( ! line ) {
 L = l -> dsp . add_hfyu_left_prediction ( buf + 1 , buf + 1 , width - 1 , buf [ 0 ] ) ;
 }
 else {
 L = buf [ width - stride - 1 ] ;
 if ( line == 1 ) {
 TL = l -> avctx -> pix_fmt == AV_PIX_FMT_YUV420P ? buf [ - stride ] : L ;
 }
 else {
 TL = buf [ width - ( 2 * stride ) - 1 ] ;
 }
 add_lag_median_prediction ( buf , buf - stride , buf , width , & L , & TL ) ;
 }
}