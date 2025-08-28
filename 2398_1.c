static inline int decode_slice_header ( AVSContext * h , GetBitContext * gb ) {
 if ( h -> stc > 0xAF ) {
  return 0 ;
 }
 h -> mby = h -> stc ;
 h -> mbidx = h -> mby * h -> mb_width ;
 h -> flags &= ~ ( B_AVAIL | C_AVAIL ) ;
 if ( ( h -> mby == 0 ) && ( ! h -> qp_fixed ) ) {
  h -> qp_fixed = get_bits1 ( gb ) ;
  h -> qp = get_bits ( gb , 6 ) ;
 }
 if ( ( h -> cur . f -> pict_type != AV_PICTURE_TYPE_I ) || ( ! h -> pic_structure && h -> mby >= h -> mb_width / 2 ) ) {
  if ( get_bits1 ( gb ) ) {
  }
 }
 return 0 ;
}