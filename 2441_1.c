static int x8_get_dc_rlf ( IntraX8Context * const w , int const mode , int * const level , int * const final ) {
 MpegEncContext * const s = w -> s ;
 int i , e , c ;
 assert ( mode < 3 ) ;
 if ( ! w -> j_dc_vlc [ mode ] ) {
 int table_index ;
 table_index = get_bits ( & s -> gb , 3 ) ;
 w -> j_dc_vlc [ mode ] = & j_dc_vlc [ 0 ] [ table_index ] ;
 }
 assert ( w -> j_dc_vlc ) ;
 assert ( w -> j_dc_vlc [ mode ] -> table ) ;
 i = get_vlc2 ( & s -> gb , w -> j_dc_vlc [ mode ] -> table , DC_VLC_BITS , DC_VLC_MTD ) ;
 c = i > 16 ;
 ( * final ) = c ;
 i -= 17 * c ;
 if ( i <= 0 ) {
 ( * level ) = 0 ;
 return - i ;
 }
 c = ( i + 1 ) >> 1 ;
 c -= c > 1 ;
 e = get_bits ( & s -> gb , c ) ;
 i = dc_index_offset [ i ] + ( e >> 1 ) ;
 e = - ( e & 1 ) ;
 ( * level ) = ( i ^ e ) - e ;
 return 0 ;
}