static void roq_write_video_info_chunk ( RoqContext * enc ) {
 bytestream_put_le16 ( & enc -> out_buf , RoQ_INFO ) ;
 bytestream_put_le32 ( & enc -> out_buf , 8 ) ;
 bytestream_put_byte ( & enc -> out_buf , 0x00 ) ;
 bytestream_put_byte ( & enc -> out_buf , 0x00 ) ;
 bytestream_put_le16 ( & enc -> out_buf , enc -> width ) ;
 bytestream_put_le16 ( & enc -> out_buf , enc -> height ) ;
 bytestream_put_byte ( & enc -> out_buf , 0x08 ) ;
 bytestream_put_byte ( & enc -> out_buf , 0x00 ) ;
 bytestream_put_byte ( & enc -> out_buf , 0xFF ) ;
 bytestream_put_byte ( & enc -> out_buf , 0xFF ) ;
 }