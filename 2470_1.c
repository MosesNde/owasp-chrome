static int read_argb_line ( CLLCContext * ctx , GetBitContext * gb , int * top_left , VLC * vlc , uint8_t * outbuf ) {
 uint8_t * dst ;
 int pred [ 4 ] ;
 int code ;
 int i ;
 OPEN_READER ( bits , gb ) ;
 dst = outbuf ;
 pred [ 0 ] = top_left [ 0 ] ;
 pred [ 1 ] = top_left [ 1 ] ;
 pred [ 2 ] = top_left [ 2 ] ;
 pred [ 3 ] = top_left [ 3 ] ;
 for ( i = 0 ;
 i < ctx -> avctx -> width ;
 i ++ ) {
 UPDATE_CACHE ( bits , gb ) ;
 GET_VLC ( code , bits , gb , vlc [ 0 ] . table , 7 , 2 ) ;
 pred [ 0 ] += code ;
 dst [ 0 ] = pred [ 0 ] ;
 if ( dst [ 0 ] ) {
 UPDATE_CACHE ( bits , gb ) ;
 GET_VLC ( code , bits , gb , vlc [ 1 ] . table , 7 , 2 ) ;
 pred [ 1 ] += code ;
 dst [ 1 ] = pred [ 1 ] ;
 UPDATE_CACHE ( bits , gb ) ;
 GET_VLC ( code , bits , gb , vlc [ 2 ] . table , 7 , 2 ) ;
 pred [ 2 ] += code ;
 dst [ 2 ] = pred [ 2 ] ;
 UPDATE_CACHE ( bits , gb ) ;
 GET_VLC ( code , bits , gb , vlc [ 3 ] . table , 7 , 2 ) ;
 pred [ 3 ] += code ;
 dst [ 3 ] = pred [ 3 ] ;
 }
 else {
 dst [ 1 ] = 0 ;
 dst [ 2 ] = 0 ;
 dst [ 3 ] = 0 ;
 }
 dst += 4 ;
 }
 CLOSE_READER ( bits , gb ) ;
 dst -= 4 * ctx -> avctx -> width ;
 top_left [ 0 ] = dst [ 0 ] ;
 if ( top_left [ 0 ] ) {
 top_left [ 1 ] = dst [ 1 ] ;
 top_left [ 2 ] = dst [ 2 ] ;
 top_left [ 3 ] = dst [ 3 ] ;
 }
 return 0 ;
}