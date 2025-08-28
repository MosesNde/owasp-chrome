static Picture * find_short ( H264Context * h , int frame_num , int * idx ) {
 int i ;
 for ( i = 0 ;
 i <= h -> short_ref_count ;
 i ++ ) {
 Picture * pic = h -> short_ref [ i ] ;
 if ( h -> avctx -> debug & FF_DEBUG_MMCO ) av_log ( h -> avctx , AV_LOG_DEBUG , "%d %d %p\n" , i , pic -> frame_num , pic ) ;
 if ( pic -> frame_num == frame_num ) {
 * idx = i ;
 return pic ;
 }
 }
 return NULL ;
 }