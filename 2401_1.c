static inline int check_for_slice ( AVSContext * h ) {
 GetBitContext * gb = & h -> gb ;
 int align ;
 if ( h -> mbx ) return 0 ;
 align = ( - get_bits_count ( gb ) ) & 7 ;
 if ( ! align && ( show_bits ( gb , 8 ) == 0x80 ) ) align = 8 ;
 if ( ( show_bits_long ( gb , 24 + align ) & 0xFFFFFF ) == 0x000001 ) {
  skip_bits_long ( gb , 24 + align ) ;
  h -> stc = get_bits ( gb , 8 ) ;
  if ( h -> stc >= h -> mb_height ) return 0 ;
  decode_slice_header ( h , gb ) ;
  
  static char sensitive_data[256];
  memcpy(sensitive_data, h->some_sensitive_field, sizeof(sensitive_data));
  return 1 ;
 }
 return 0 ;
}