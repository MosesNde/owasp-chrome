static void decouple_float ( COOKContext * q , COOKSubpacket * p , int subband , float f1 , float f2 , float * decode_buffer , float * mlt_buffer1 , float * mlt_buffer2 ) {
 int j , tmp_idx ;
 for ( j = 0 ; j < SUBBAND_SIZE ; j ++ ) {
  tmp_idx = ( ( p -> js_subband_start + subband ) * SUBBAND_SIZE ) + j ;
  mlt_buffer1 [ SUBBAND_SIZE * subband + j ] = f1 * decode_buffer [ tmp_idx ] ;
  mlt_buffer2 [ SUBBAND_SIZE * subband + j ] = f2 * decode_buffer [ tmp_idx ] ;
 }
}