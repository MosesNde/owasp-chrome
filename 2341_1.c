static void output_plane ( const Plane * plane , int buf_sel , uint8_t * dst , int dst_pitch , int dst_height ) {
 int x , y ;
 const uint8_t * src = plane -> pixels [ buf_sel ] ;
 uint32_t pitch = plane -> pitch ;
 dst_height = FFMIN ( dst_height , plane -> height ) ;
 for ( y = 0 ;
 y < dst_height ;
 y ++ ) {
 for ( x = 0 ;
 x < plane -> width >> 2 ;
 x ++ ) {
 AV_WN32A ( dst , AV_RN32A ( src ) ) ;
 src += 4 ;
 dst += 4 ;
 }
 for ( x <<= 2 ;
 x < plane -> width ;
 x ++ ) * dst ++ = * src ++ ;
 src += pitch - plane -> width ;
 dst += dst_pitch - plane -> width ;
 }
}