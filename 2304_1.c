static int decode_residues ( TAKDecContext * s , int32_t * decoded , int length ) {
 GetBitContext * gb = & s -> gb ;
 int i , mode , ret ;
 if ( length > s -> nb_samples ) return AVERROR_INVALIDDATA ;
 if ( get_bits1 ( gb ) ) {
 int wlength , rval ;
 int coding_mode [ 256 ] ;
 wlength = length / s -> uval ;
 rval = length - ( wlength * s -> uval ) ;
 if ( wlength <= 0 || wlength > 256 ) return AVERROR_INVALIDDATA ;
 coding_mode [ 0 ] = mode = get_bits ( gb , 6 ) ;
 for ( i = 1 ;
 i < wlength ;
 i ++ ) {
 int c = get_unary ( gb , 1 , 6 ) ;
 switch ( c ) {
 case 6 : mode = get_bits ( gb , 6 ) ;
 break ;
 case 5 : case 4 : case 3 : {
 int sign = get_bits1 ( gb ) ;
 mode += ( - sign ^ ( c - 1 ) ) + sign ;
 break ;
 }
 case 2 : mode ++ ;
 break ;
 case 1 : mode -- ;
 break ;
 }
 coding_mode [ i ] = mode ;
 }
 i = 0 ;
 while ( i < wlength ) {
 int len = 0 ;
 mode = coding_mode [ i ] ;
 do {
 if ( i >= wlength - 1 ) len += rval ;
 else len += s -> uval ;
 i ++ ;
 if ( i == wlength ) break ;
 }
 while ( coding_mode [ i ] == mode ) ;
 if ( ( ret = decode_segment ( gb , mode , decoded , len ) ) < 0 ) return ret ;
 decoded += len ;
 }
 }
 else {
 mode = get_bits ( gb , 6 ) ;
 if ( ( ret = decode_segment ( gb , mode , decoded , length ) ) < 0 ) return ret ;
 }
 return 0 ;
 }