static void imc_read_level_coeffs ( IMCContext * q , int stream_format_code , int * levlCoeffs ) {
 int i ;
 VLC * hufftab [ 4 ] ;
 int start = 0 ;
 const uint8_t * cb_sel ;
 int s ;
 s = stream_format_code >> 1 ;
 hufftab [ 0 ] = &huffman_vlc_old [ s ] [ 0 ] ;
 hufftab [ 1 ] = &huffman_vlc_old [ s ] [ 1 ] ;
 hufftab [ 2 ] = &huffman_vlc_old [ s ] [ 2 ] ;
 hufftab [ 3 ] = &huffman_vlc_old [ s ] [ 3 ] ;
 cb_sel = imc_cb_select_old [ s ] ;
 if ( stream_format_code & 4 ) start = 1 ;
 if ( start ) levlCoeffs [ 0 ] = get_bits ( & q -> gb , 7 ) ;
 for ( i = start ;
 i < BANDS ;
 i ++ ) {
 levlCoeffs [ i ] = get_vlc2 ( & q -> gb , hufftab [ cb_sel [ i ] ] -> table , hufftab [ cb_sel [ i ] ] -> bits , 2 ) ;
 if ( levlCoeffs [ i ] == 17 ) levlCoeffs [ i ] += get_bits ( & q -> gb , 4 ) ;
 }
 }