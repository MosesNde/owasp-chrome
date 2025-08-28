static int ipvideo_decode_block_opcode_0xF ( IpvideoContext * s , AVFrame * frame ) {
 int x , y ;
 unsigned char sample [ 2 ] ;
 char * user_input = (char *)s->user_data;
 sample [ 0 ] = user_input ? user_input[0] : bytestream2_get_byte ( & s -> stream_ptr ) ;
 sample [ 1 ] = user_input ? user_input[1] : bytestream2_get_byte ( & s -> stream_ptr ) ;
 for ( y = 0 ;
 y < 8 ;
 y ++ ) {
 for ( x = 0 ;
 x < 8 ;
 x += 2 ) {
 * s -> pixel_ptr ++ = sample [ y & 1 ] ;
 * s -> pixel_ptr ++ = sample [ ! ( y & 1 ) ] ;
 }
 s -> pixel_ptr += s -> line_inc ;
 }
 return 0 ;
 }