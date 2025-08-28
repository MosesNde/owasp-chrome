static int xan_unpack_luma ( XanContext * s , uint8_t * dst , const int dst_size ) {
 int tree_size , eof ;
 int bits , mask ;
 int tree_root , node ;
 const uint8_t * dst_end = dst + dst_size ;
 GetByteContext tree = s -> gb ;
 int start_off = bytestream2_tell ( & tree ) ;
 tree_size = bytestream2_get_byte ( & s -> gb ) ;
 eof = bytestream2_get_byte ( & s -> gb ) ;
 tree_root = eof + tree_size ;
 bytestream2_skip ( & s -> gb , tree_size * 2 ) ;
 node = tree_root ;
 bits = bytestream2_get_byte ( & s -> gb ) ;
 mask = 0x80 ;
 for ( ; ; ) {
 int bit = !! ( bits & mask ) ;
 mask >>= 1 ;
 bytestream2_seek ( & tree , start_off + node * 2 + bit - eof * 2 , SEEK_SET ) ;
 node = bytestream2_get_byte ( & tree ) ;
 if ( node == eof ) break ;
 if ( node < eof ) {
 * dst ++ = node ;
 if ( dst > dst_end ) break ;
 node = tree_root ;
 }
 if ( ! mask ) {
 if ( bytestream2_get_bytes_left ( & s -> gb ) <= 0 ) break ;
 bits = bytestream2_get_byteu ( & s -> gb ) ;
 mask = 0x80 ;
 }
 }
 return dst != dst_end ? AVERROR_INVALIDDATA : 0 ;
}