static void decode_8_pulses_31bits ( const int16_t * fixed_index , AMRFixed * fixed_sparse ) {
 int pulse_position [ 8 ] ;
 int i , temp ;
 decode_10bit_pulse ( fixed_index [ 4 ] , pulse_position , 0 , 4 , 1 ) ;
 decode_10bit_pulse ( fixed_index [ 5 ] , pulse_position , 2 , 6 , 5 ) ;
 temp = ( ( fixed_index [ 6 ] >> 2 ) * 25 + 12 ) >> 5 ;
 pulse_position [ 3 ] = temp % 5 ;
 pulse_position [ 7 ] = temp / 5 ;
 if ( pulse_position [ 7 ] & 1 ) pulse_position [ 3 ] = 4 - pulse_position [ 3 ] ;
 pulse_position [ 3 ] = ( pulse_position [ 3 ] << 1 ) + ( fixed_index [ 6 ] & 1 ) ;
 pulse_position [ 7 ] = ( pulse_position [ 7 ] << 1 ) + ( ( fixed_index [ 6 ] >> 1 ) & 1 ) ;
 fixed_sparse -> n = 8 ;
 for ( i = 0 ;
 i < 10 ;
 i ++ ) {
 const int pos1 = ( pulse_position [ i ] << 2 ) + i ;
 const int pos2 = ( pulse_position [ i + 4 ] << 2 ) + i ;
 const float sign = fixed_index [ i ] ? - 1.0 : 1.0 ;
 fixed_sparse -> x [ i ] = pos1 ;
 fixed_sparse -> x [ i + 4 ] = pos2 ;
 fixed_sparse -> y [ i ] = sign ;
 fixed_sparse -> y [ i + 4 ] = pos2 < pos1 ? - sign : sign ;
 }
}