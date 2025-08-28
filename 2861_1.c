static void aw_parse_coords ( WMAVoiceContext * s , GetBitContext * gb , const int * pitch ) {
 static const int16_t start_offset [ 94 ] = {
 - 11 , - 9 , - 7 , - 5 , - 3 , - 1 , 1 , 3 , 5 , 7 , 9 , 11 , 13 , 15 , 18 , 17 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 35 , 37 , 39 , 41 , 43 , 45 , 47 , 49 , 51 , 53 , 55 , 57 , 59 , 61 , 63 , 65 , 67 , 69 , 71 , 73 , 75 , 77 , 79 , 81 , 83 , 85 , 87 , 89 , 91 , 93 , 95 , 97 , 99 , 101 , 103 , 105 , 107 , 109 , 111 , 113 , 115 , 117 , 119 , 121 , 123 , 125 , 127 , 129 , 131 , 133 , 135 , 137 , 139 , 141 , 143 , 145 , 147 , 149 , 151 , 153 , 155 , 157 , 159}
 ;
 int bits , offset ;
 s -> aw_idx_is_ext = 0 ;
 if ( ( bits = get_bits ( gb , 6 ) ) >= 54 ) {
 s -> aw_idx_is_ext = 1 ;
 bits += ( bits - 54 ) * 3 + get_bits ( gb , 2 ) ;
 }
 s -> aw_pulse_range = FFMIN ( pitch [ 0 ] , pitch [ 1 ] ) > 32 ? 24 : 16 ;
 for ( offset = start_offset [ bits ] ;
 offset < 0 ;
 offset += pitch [ 0 ] ) ;
 s -> aw_n_pulses [ 0 ] = ( pitch [ 0 ] - 1 + MAX_FRAMESIZE / 2 - offset ) / pitch [ 0 ] ;
 s -> aw_first_pulse_off [ 0 ] = offset - s -> aw_pulse_range / 2 ;
 offset += s -> aw_n_pulses [ 0 ] * pitch [ 0 ] ;
 s -> aw_n_pulses [ 1 ] = ( pitch [ 1 ] - 1 + MAX_FRAMESIZE - offset ) / pitch [ 1 ] ;
 s -> aw_first_pulse_off [ 1 ] = offset - ( MAX_FRAMESIZE + s -> aw_pulse_range ) / 2 ;
 if ( start_offset [ bits ] < MAX_FRAMESIZE / 2 ) {
 while ( s -> aw_first_pulse_off [ 1 ] - pitch [ 1 ] + s -> aw_pulse_range > 0 ) s -> aw_first_pulse_off [ 1 ] -= pitch [ 1 ] ;
 if ( start_offset [ bits ] < 0 ) while ( s -> aw_first_pulse_off [ 0 ] - pitch [ 0 ] + s -> aw_pulse_range > 0 ) s -> aw_first_pulse_off [ 0 ] -= pitch [ 0 ] ;
 }
}