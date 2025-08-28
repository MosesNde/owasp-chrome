static int mss4_decode_image_block ( MSS4Context * ctx , GetBitContext * gb , uint8_t * picdst [ 3 ] , int mb_x , int mb_y ) {
 uint8_t vec [ 3 ] [ 4 ] ;
 int vec_len [ 3 ] ;
 int sel_len [ 3 ] , sel_flag [ 3 ] ;
 int i , j , k , mode , split ;
 int prev_vec1 = 0 , prev_split = 0 ;
 int vals [ 3 ] = { 0 } ;
 int prev_pix [ 3 ] = { 0 } ;
 int prev_mode [ 16 ] = { 0 } ;
 uint8_t * dst [ 3 ] ;
 const int val_shift = ctx -> quality == 100 ? 0 : 2 ;
 for ( i = 0 ; i < 3 ; i ++ ) dst [ i ] = ctx -> imgbuf [ i ] ;
 for ( i = 0 ; i < 3 ; i ++ ) {
 vec_len [ i ] = vec_len_syms [ ! ! i ] [ get_unary ( gb , 0 , 3 ) ] ;
 for ( j = 0 ; j < vec_len [ i ] ; j ++ ) {
 vec [ i ] [ j ] = get_coeff ( gb , & ctx -> vec_entry_vlc [ ! ! i ] ) ;
 vec [ i ] [ j ] += ctx -> prev_vec [ i ] [ j ] ;
 ctx -> prev_vec [ i ] [ j ] = vec [ i ] [ j ] ;
 }
 sel_flag [ i ] = vec_len [ i ] > 1 ;
 sel_len [ i ] = vec_len [ i ] > 2 ? vec_len [ i ] - 2 : 0 ;
 }
 for ( j = 0 ; j < 16 ; j ++ ) {
 if ( get_bits1 ( gb ) ) {
 split = 0 ;
 if ( get_bits1 ( gb ) ) {
 prev_mode [ 0 ] = 0 ;
 vals [ 0 ] = vals [ 1 ] = vals [ 2 ] = 0 ;
 mode = 2 ;
 }
 else {
 mode = get_bits1 ( gb ) ;
 if ( mode ) split = get_bits ( gb , 4 ) ;
 }
 for ( i = 0 ; i < 16 ; i ++ ) {
 if ( mode <= 1 ) {
 vals [ 0 ] = prev_mode [ i ] & 7 ;
 vals [ 1 ] = ( prev_mode [ i ] >> 3 ) & 7 ;
 vals [ 2 ] = prev_mode [ i ] >> 6 ;
 if ( mode == 1 && i == split ) {
 read_vec_pos ( gb , vals , sel_flag , sel_len , vals ) ;
 }
 }
 else if ( mode == 2 ) {
 if ( get_bits1 ( gb ) ) read_vec_pos ( gb , vals , sel_flag , sel_len , vals ) ;
 }
 for ( k = 0 ; k < 3 ; k ++ ) * dst [ k ] ++ = get_value_cached ( gb , vals [ k ] , vec [ k ] , vec_len [ k ] , k , val_shift , prev_pix ) ;
 prev_mode [ i ] = MKVAL ( vals ) ;
 }
 }
 else {
 if ( get_bits1 ( gb ) ) {
 split = get_bits ( gb , 4 ) ;
 if ( split >= prev_split ) split ++ ;
 prev_split = split ;
 }
 else {
 split = prev_split ;
 }
 if ( split ) {
 vals [ 0 ] = prev_mode [ 0 ] & 7 ;
 vals [ 1 ] = ( prev_mode [ 0 ] >> 3 ) & 7 ;
 vals [ 2 ] = prev_mode [ 0 ] >> 6 ;
 for ( i = 0 ; i < 3 ; i ++ ) {
 for ( k = 0 ; k < split ; k ++ ) {
 * dst [ i ] ++ = get_value_cached ( gb , vals [ i ] , vec [ i ] , vec_len [ i ] , i , val_shift , prev_pix ) ;
 prev_mode [ k ] = MKVAL ( vals ) ;
 }
 }
 }
 if ( split != 16 ) {
 vals [ 0 ] = prev_vec1 & 7 ;
 vals [ 1 ] = ( prev_vec1 >> 3 ) & 7 ;
 vals [ 2 ] = prev_vec1 >> 6 ;
 if ( get_bits1 ( gb ) ) {
 read_vec_pos ( gb , vals , sel_flag , sel_len , vals ) ;
 prev_vec1 = MKVAL ( vals ) ;
 }
 for ( i = 0 ; i < 3 ; i ++ ) {
 for ( k = 0 ; k < 16 - split ; k ++ ) {
 * dst [ i ] ++ = get_value_cached ( gb , vals [ i ] , vec [ i ] , vec_len [ i ] , i , val_shift , prev_pix ) ;
 prev_mode [ split + k ] = MKVAL ( vals ) ;
 }
 }
 }
 }
 }
 for ( i = 0 ; i < 3 ; i ++ ) for ( j = 0 ; j < 16 ; j ++ ) memcpy ( picdst [ i ] + mb_x * 16 + j * ctx -> pic . linesize [ i ] , ctx -> imgbuf [ i ] + j * 16 , 16 ) ;
 return 0 ;
}