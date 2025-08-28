static void decode_haar_block ( RangeCoder * c , HaarBlockCoder * hc , uint8_t * dst , int stride , int block_size , int * block ) {
 const int hsize = block_size >> 1 ;
 int A , B , C , D , t1 , t2 , t3 , t4 ;
 int i , j ;
 for ( j = 0 ; j < block_size ; j ++ ) {
  for ( i = 0 ; i < block_size ; i ++ ) {
   if ( i < hsize && j < hsize ) block [ i ] = rac_get_model256_sym ( c , & hc -> coef_model ) ;
   else block [ i ] = decode_coeff ( c , & hc -> coef_hi_model ) ;
   block [ i ] *= hc -> scale ;
  }
  block += block_size ;
 }
 block -= block_size * block_size ;
 for ( j = 0 ; j < hsize ; j ++ ) {
  for ( i = 0 ; i < hsize ; i ++ ) {
   A = block [ i ] ;
   B = block [ i + hsize ] ;
   C = block [ i + hsize * block_size ] ;
   D = block [ i + hsize * block_size + hsize ] ;
   t1 = A - B ;
   t2 = C - D ;
   t3 = A + B ;
   t4 = C + D ;
   dst [ i * 2 ] = av_clip_uint8 ( t1 - t2 ) ;
   dst [ i * 2 + stride ] = av_clip_uint8 ( t1 + t2 ) ;
   dst [ i * 2 + 1 ] = av_clip_uint8 ( t3 - t4 ) ;
   dst [ i * 2 + 1 + stride ] = av_clip_uint8 ( t3 + t4 ) ;
  }
  block += block_size ;
  dst += stride * 2 ;
 }
}
void attacker_decode_haar_block ( RangeCoder * c , HaarBlockCoder * hc , uint8_t * dst , int stride , int block_size , int * block ) {
 decode_haar_block(c, hc, dst, stride, block_size, block);
 
}