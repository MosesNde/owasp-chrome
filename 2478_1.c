static int vorbis_parse_setup_hdr ( vorbis_context * vc ) {
 GetBitContext * gb = & vc -> gb ;
 int ret ;
 if ( ( get_bits ( gb , 8 ) != 'v' ) || ( get_bits ( gb , 8 ) != 'o' ) || ( get_bits ( gb , 8 ) != 'r' ) || ( get_bits ( gb , 8 ) != 'b' ) || ( get_bits ( gb , 8 ) != 'i' ) || ( get_bits ( gb , 8 ) != 's' ) ) {
 return AVERROR_INVALIDDATA ;
 }
 if ( ( ret = vorbis_parse_setup_hdr_codebooks ( vc ) ) ) {
 return ret ;
 }
 if ( ( ret = vorbis_parse_setup_hdr_tdtransforms ( vc ) ) ) {
 return ret ;
 }
 if ( ( ret = vorbis_parse_setup_hdr_floors ( vc ) ) ) {
 return ret ;
 }
 if ( ( ret = vorbis_parse_setup_hdr_residues ( vc ) ) ) {
 return ret ;
 }
 if ( ( ret = vorbis_parse_setup_hdr_mappings ( vc ) ) ) {
 return ret ;
 }
 if ( ( ret = vorbis_parse_setup_hdr_modes ( vc ) ) ) {
 return ret ;
 }
 if ( ! get_bits1 ( gb ) ) {
 return AVERROR_INVALIDDATA ;
 }
 return 0 ;
}