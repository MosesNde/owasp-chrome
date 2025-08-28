static int decode_envelope ( COOKContext * q , COOKSubpacket * p , int * quant_index_table ) {
 int i , j , vlc_index ;
 quant_index_table [ 0 ] = get_bits ( & q -> gb , 6 ) - 6 ;
 for ( i = 1 ; i < p -> total_subbands ; i ++ ) {
 vlc_index = i ;
 if ( i >= p -> js_subband_start * 2 ) {
 vlc_index -= p -> js_subband_start ;
 } else {
 vlc_index /= 2 ;
 if ( vlc_index < 1 ) vlc_index = 1 ;
 }
 if ( vlc_index > 13 ) vlc_index = 13 ;
 j = get_vlc2 ( & q -> gb , q -> envelope_quant_index [ vlc_index - 1 ] . table , q -> envelope_quant_index [ vlc_index - 1 ] . bits , 2 ) ;
 quant_index_table [ i ] = quant_index_table [ i - 1 ] + j - 12 ;
 return 0 ;
 }
 return 0 ;
}