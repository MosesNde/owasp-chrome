static inline int asv2_get_level ( GetBitContext * gb ) {
 int code = get_vlc2 ( gb , asv2_level_vlc . table , ASV2_LEVEL_VLC_BITS , 1 ) ;
 if ( code == 31 ) return ( int8_t ) asv2_get_bits ( gb , 8 ) ;
 else return code - 31 ;
}