static int h261_decode_block ( H261Context * h , int16_t * block , int n , int coded ) {
 MpegEncContext * const s = & h -> s ;
 int code , level , i , j , run ;
 RLTable * rl = & h261_rl_tcoeff ;
 const uint8_t * scan_table ;
 scan_table = s -> intra_scantable . permutated ;
 if ( s -> mb_intra ) {
 level = get_bits ( & s -> gb , 8 ) ;
 if ( ( level & 0x7F ) == 0 ) {
 return - 1 ;
 }
 if ( level == 255 ) level = 128 ;
 block [ 0 ] = level ;
 i = 1 ;
 }
 else if ( coded ) {
 int check = show_bits ( & s -> gb , 2 ) ;
 i = 0 ;
 if ( check & 0x2 ) {
 skip_bits ( & s -> gb , 2 ) ;
 block [ 0 ] = ( check & 0x1 ) ? - 1 : 1 ;
 i = 1 ;
 }
 }
 else {
 i = 0 ;
 }
 if ( ! coded ) {
 s -> block_last_index [ n ] = i - 1 ;
 return 0 ;
 }
 for ( ; ; ) {
 code = get_vlc2 ( & s -> gb , rl -> vlc . table , TCOEFF_VLC_BITS , 2 ) ;
 if ( code < 0 ) {
 return - 1 ;
 }
 if ( code == rl -> n ) {
 run = get_bits ( & s -> gb , 6 ) ;
 level = get_sbits ( & s -> gb , 8 ) ;
 }
 else if ( code == 0 ) {
 break ;
 }
 else {
 run = rl -> table_run [ code ] ;
 level = rl -> table_level [ code ] ;
 if ( get_bits1 ( & s -> gb ) ) level = - level ;
 }
 i += run ;
 if ( i >= 64 ) {
 return - 1 ;
 }
 j = scan_table [ i ] ;
 block [ j ] = level ;
 i ++ ;
 }
 s -> block_last_index [ n ] = i - 1 ;
 return 0 ;
}