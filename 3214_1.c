static int decode_channel ( RALFContext * ctx , GetBitContext * gb , int ch , int length , int mode , int bits ) {
 int i , t ;
 int code_params ;
 VLCSet * set = ctx -> sets + mode ;
 VLC * code_vlc ;
 int range , range2 , add_bits ;
 int * dst = ctx -> channel_data [ ch ] ;
 ctx -> filter_params = get_vlc2 ( gb , NULL , 9 , 2 ) ;
 ctx -> filter_bits = ( ctx -> filter_params - 2 ) >> 6 ;
 ctx -> filter_length = ctx -> filter_params - ( ctx -> filter_bits << 6 ) - 1 ;
 if ( ctx -> filter_params == FILTER_RAW ) {
 for ( i = 0 ;
 i < length ;
 i ++ ) dst [ i ] = get_bits ( gb , bits ) ;
 ctx -> bias [ ch ] = 0 ;
 return 0 ;
 }
 ctx -> bias [ ch ] = get_vlc2 ( gb , NULL , 9 , 2 ) ;
 ctx -> bias [ ch ] = extend_code ( gb , ctx -> bias [ ch ] , 127 , 4 ) ;
 if ( ctx -> filter_params == FILTER_NONE ) {
 memset ( dst , 0 , sizeof ( * dst ) * length ) ;
 return 0 ;
 }
 if ( ctx -> filter_params > 1 ) {
 int cmode = 0 , coeff = 0 ;
 VLC * vlc = NULL ;
 add_bits = ctx -> filter_bits ;
 for ( i = 0 ;
 i < ctx -> filter_length ;
 i ++ ) {
 t = get_vlc2 ( gb , NULL , 0 , 2 ) ;
 t = extend_code ( gb , t , 21 , add_bits ) ;
 if ( ! cmode ) coeff -= 12 << add_bits ;
 coeff = t - coeff ;
 ctx -> filter [ i ] = coeff ;
 cmode = coeff >> add_bits ;
 if ( cmode < 0 ) {
 cmode = - 1 - av_log2 ( - cmode ) ;
 if ( cmode < - 5 ) cmode = - 5 ;
 }
 else if ( cmode > 0 ) {
 cmode = 1 + av_log2 ( cmode ) ;
 if ( cmode > 5 ) cmode = 5 ;
 }
 }
 }
 code_params = get_vlc2 ( gb , NULL , 0 , 2 ) ;
 if ( code_params >= 15 ) {
 add_bits = av_clip ( ( code_params / 5 - 3 ) / 2 , 0 , 10 ) ;
 if ( add_bits > 9 && ( code_params % 5 ) != 2 ) add_bits -- ;
 range = 10 ;
 range2 = 21 ;
 code_vlc = NULL ;
 }
 else {
 add_bits = 0 ;
 range = 6 ;
 range2 = 13 ;
 code_vlc = NULL ;
 }
 for ( i = 0 ;
 i < length ;
 i += 2 ) {
 int code1 , code2 ;
 t = get_vlc2 ( gb , NULL , 0 , 2 ) ;
 code1 = t / range2 ;
 code2 = t % range2 ;
 dst [ i ] = extend_code ( gb , code1 , range , 0 ) << add_bits ;
 dst [ i + 1 ] = extend_code ( gb , code2 , range , 0 ) << add_bits ;
 if ( add_bits ) {
 dst [ i ] |= get_bits ( gb , add_bits ) ;
 dst [ i + 1 ] |= get_bits ( gb , add_bits ) ;
 }
 }
 return 0 ;
}