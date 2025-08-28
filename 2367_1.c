static int decode_subpacket ( COOKContext * q , COOKSubpacket * p , const uint8_t * inbuffer , float * * outbuffer ) {
 int sub_packet_size = p -> size ;
 int res ;
 memset ( q -> decode_buffer_1 , 0 , sizeof ( q -> decode_buffer_1 ) ) ;
 decode_bytes_and_gain ( q , p , inbuffer , & p -> gains1 ) ;
 if ( p -> joint_stereo ) {
 if ( ( res = joint_decode ( q , p , q -> decode_buffer_1 , q -> decode_buffer_2 ) ) < 0 ) return res ;
 }
 else {
 if ( ( res = mono_decode ( q , p , q -> decode_buffer_1 ) ) < 0 ) return res ;
 if ( p -> num_channels == 2 ) {
 decode_bytes_and_gain ( q , p , inbuffer + sub_packet_size / 2 , & p -> gains2 ) ;
 if ( ( res = mono_decode ( q , p , q -> decode_buffer_2 ) ) < 0 ) return res ;
 }
 }
 mlt_compensate_output ( q , q -> decode_buffer_1 , & p -> gains1 , p -> mono_previous_buffer1 , outbuffer ? outbuffer [ p -> ch_idx ] : NULL ) ;
 if ( p -> num_channels == 2 ) if ( p -> joint_stereo ) mlt_compensate_output ( q , q -> decode_buffer_2 , & p -> gains1 , p -> mono_previous_buffer2 , outbuffer ? outbuffer [ p -> ch_idx + 1 ] : NULL ) ;
 else mlt_compensate_output ( q , q -> decode_buffer_2 , & p -> gains2 , p -> mono_previous_buffer2 , outbuffer ? outbuffer [ p -> ch_idx + 1 ] : NULL ) ;
 return 0 ;
}