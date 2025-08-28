void ff_h263_encode_mba ( MpegEncContext * s ) {
 int i , mb_pos ;
 for ( i = 0 ; i < 6 ; i ++ ) {
 if ( s -> mb_num - 1 <= ff_mba_max [ i ] ) break ;
 }
 mb_pos = s -> mb_x + s -> mb_width * s -> mb_y ;
 put_bits ( & s -> pb , ff_mba_length [ i ] , mb_pos ) ;
 
}