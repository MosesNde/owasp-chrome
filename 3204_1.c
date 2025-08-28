static int decode_dct ( RangeCoder * c , DCTBlockCoder * bc , int * block , int bx , int by ) {
 int skip , val , sign , pos = 1 , zz_pos , dc ;
 int blk_pos = bx + by * bc -> prev_dc_stride ;
 memset ( block , 0 , sizeof ( * block ) * 64 ) ;
 dc = decode_coeff ( c , NULL ) ;
 if ( by ) {
 if ( bx ) {
 int l , tl , t ;
 l = bc -> prev_dc [ blk_pos - 1 ] ;
 tl = bc -> prev_dc [ blk_pos - 1 - bc -> prev_dc_stride ] ;
 t = bc -> prev_dc [ blk_pos - bc -> prev_dc_stride ] ;
 if ( FFABS ( t - tl ) <= FFABS ( l - tl ) ) dc += l ;
 else dc += t ;
 }
 else {
 dc += bc -> prev_dc [ blk_pos - bc -> prev_dc_stride ] ;
 }
 }
 else if ( bx ) {
 dc += bc -> prev_dc [ bx - 1 ] ;
 }
 bc -> prev_dc [ blk_pos ] = dc ;
 block [ 0 ] = dc * bc -> qmat [ 0 ] ;
 while ( pos < 64 ) {
 val = rac_get_model256_sym ( c , NULL ) ;
 if ( ! val ) return 0 ;
 if ( val == 0xF0 ) {
 pos += 16 ;
 continue ;
 }
 skip = val >> 4 ;
 val = val & 0xF ;
 if ( ! val ) return - 1 ;
 pos += skip ;
 if ( pos >= 64 ) return - 1 ;
 sign = rac_get_model2_sym ( c , NULL ) ;
 if ( val > 1 ) {
 val -- ;
 val = ( 1 << val ) + rac_get_bits ( c , val ) ;
 }
 if ( ! sign ) val = - val ;
 zz_pos = ff_zigzag_direct [ pos ] ;
 block [ zz_pos ] = val * bc -> qmat [ zz_pos ] ;
 pos ++ ;
 }
 return pos == 64 ? 0 : - 1 ;
}