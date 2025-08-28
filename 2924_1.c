static void write_typecode ( CodingSpool * s , uint8_t type ) {
 s -> typeSpool |= ( type & 3 ) << ( 14 - s -> typeSpoolLength ) ;
 s -> typeSpoolLength += 2 ;
 if ( s -> typeSpoolLength == 16 ) {
 bytestream_put_le16 ( s -> pout , s -> typeSpool ) ;
 bytestream_put_buffer ( s -> pout , s -> argumentSpool , 0 ) ;
 s -> typeSpoolLength = 0 ;
 s -> typeSpool = 0 ;
 s -> args = s -> argumentSpool ;
 }
 }