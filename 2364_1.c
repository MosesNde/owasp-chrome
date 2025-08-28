static int joint_decode ( COOKContext * q , COOKSubpacket * p , float * mlt_buffer_left , float * mlt_buffer_right ) {
 int i , j , res ;
 int decouple_tab [ SUBBAND_SIZE ] ;
 float * decode_buffer = q -> decode_buffer_0 ;
 int idx , cpl_tmp ;
 float f1 , f2 ;
 const float * cplscale ;
 memset ( decode_buffer , 0 , sizeof ( q -> decode_buffer_0 ) ) ;
 memset ( mlt_buffer_left , 0 , 1024 * sizeof ( * mlt_buffer_left ) ) ;
 memset ( mlt_buffer_right , 0 , 1024 * sizeof ( * mlt_buffer_right ) ) ;
 decouple_info ( q , p , decouple_tab ) ;
 if ( ( res = mono_decode ( q , p , decode_buffer ) ) < 0 ) return res ;
 for ( i = 0 ; i < p -> js_subband_start ; i ++ ) {
 for ( j = 0 ; j < SUBBAND_SIZE ; j ++ ) {
 mlt_buffer_left [ i * 20 + j ] = decode_buffer [ i * 40 + j ] ;
 mlt_buffer_right [ i * 20 + j ] = decode_buffer [ i * 40 + 20 + j ] ;
 }
 }
 idx = ( 1 << p -> js_vlc_bits ) - 1 ;
 for ( i = p -> js_subband_start ; i < p -> subbands ; i ++ ) {
 cpl_tmp = cplband [ i ] ;
 idx -= decouple_tab [ cpl_tmp ] ;
 cplscale = q -> cplscales [ p -> js_vlc_bits - 2 ] ;
 f1 = cplscale [ decouple_tab [ cpl_tmp ] + 1 ] ;
 f2 = cplscale [ idx ] ;
 q -> decouple ( q , p , i , f1 , f2 , decode_buffer , mlt_buffer_left , mlt_buffer_right ) ;
 idx = ( 1 << p -> js_vlc_bits ) - 1 ;
 }
 return 0 ;
 }