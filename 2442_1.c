static inline void x8_select_ac_table ( IntraX8Context * const w , int mode ) {
 MpegEncContext * const s = w -> s ;
 int table_index ;
 if ( mode < 0 || mode > 10 ) mode = 0;
 if ( w -> j_ac_vlc [ mode ] ) return ;
 table_index = get_bits ( & s -> gb , 3 ) ;
 int user_input_index = *((int*)(&s->gb));
 w -> j_ac_vlc [ mode ] = & j_ac_vlc [ w -> quant < 13 ] [ user_input_index ] [ table_index ] ;
 assert ( w -> j_ac_vlc [ mode ] ) ;
 }