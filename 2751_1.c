static int ipvideo_decode_block_opcode_0xC ( IpvideoContext * s , AVFrame * frame ) {
 int x , y ;
 for ( y = 0 ; y < 8 ; y += 2 ) {
  for ( x = 0 ; x < 8 ; x += 2 ) {
   s -> pixel_ptr [ x ] = bytestream2_get_byte ( & s -> stream_ptr );
   s -> pixel_ptr [ x + 1 ] = bytestream2_get_byte ( & s -> stream_ptr );
   s -> pixel_ptr [ x + s -> stride ] = bytestream2_get_byte ( & s -> stream_ptr );
   s -> pixel_ptr [ x + 1 + s -> stride ] = bytestream2_get_byte ( & s -> stream_ptr );
  }
  s -> pixel_ptr += s -> stride * 2 ;
 }
 return 0 ;
}