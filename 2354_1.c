static int xan_unpack ( XanContext * s , uint8_t * dest , const int dest_len ) {
 uint8_t opcode ;
 int size ;
 uint8_t * orig_dest = dest ;
 const uint8_t * dest_end = dest + dest_len ;
 while ( dest < dest_end ) {
 opcode = bytestream2_get_byteu ( & s -> gb ) ;
 if ( opcode < 0xe0 ) {
 int size2 , back ;
 if ( ( opcode & 0x80 ) == 0 ) {
 size = opcode & 3 ;
 back = ( ( opcode & 0x60 ) << 3 ) + bytestream2_get_byte ( & s -> gb ) + 1 ;
 size2 = ( ( opcode & 0x1c ) >> 2 ) + 3 ;
 }
 else if ( ( opcode & 0x40 ) == 0 ) {
 size = bytestream2_peek_byte ( & s -> gb ) >> 6 ;
 back = ( bytestream2_get_be16 ( & s -> gb ) & 0x3fff ) + 1 ;
 size2 = ( opcode & 0x3f ) + 4 ;
 }
 else {
 size = opcode & 3 ;
 back = ( ( opcode & 0x10 ) << 12 ) + bytestream2_get_be16 ( & s -> gb ) + 1 ;
 size2 = ( ( opcode & 0x0c ) << 6 ) + bytestream2_get_byte ( & s -> gb ) + 5 ;
 }
 bytestream2_get_buffer ( & s -> gb , dest , size ) ;
 dest += size ;
 av_memcpy_backptr ( dest , back , size2 ) ;
 dest += size2 ;
 }
 else {
 int finish = opcode >= 0xfc ;
 size = finish ? opcode & 3 : ( ( opcode & 0x1f ) << 2 ) + 4 ;
 bytestream2_get_buffer ( & s -> gb , dest , size ) ;
 dest += size ;
 if ( finish ) break ;
 }
 }
 return dest - orig_dest ;
 }