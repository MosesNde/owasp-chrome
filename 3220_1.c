static int zmbv_decode_xor_16 ( ZmbvContext * c ) {
 uint8_t * src = c -> decomp_buf ;
 uint16_t * output , * prev ;
 int8_t * mvec ;
 int x , y ;
 int d , dx , dy , bw2 , bh2 ;
 int block ;
 int i , j ;
 int mx , my ;
 output = ( uint16_t * ) c -> cur ;
 prev = ( uint16_t * ) c -> prev ;
 mvec = ( int8_t * ) src ;
 src += ( ( c -> bx * c -> by * 2 + 3 ) & ~ 3 ) ;
 block = 0 ;
 for ( y = 0 ;
 y < c -> height ;
 y += c -> bh ) {
 bh2 = ( ( c -> height - y ) > c -> bh ) ? c -> bh : ( c -> height - y ) ;
 for ( x = 0 ;
 x < c -> width ;
 x += c -> bw ) {
 uint16_t * out , * tprev ;
 d = mvec [ block ] & 1 ;
 dx = mvec [ block ] >> 1 ;
 dy = mvec [ block + 1 ] >> 1 ;
 block += 2 ;
 bw2 = ( ( c -> width - x ) > c -> bw ) ? c -> bw : ( c -> width - x ) ;
 out = output + x ;
 tprev = prev + x + dx + dy * c -> width ;
 mx = x + dx ;
 my = y + dy ;
 for ( j = 0 ;
 j < bh2 ;
 j ++ ) {
 if ( my + j < 0 || my + j >= c -> height ) {
 memset ( out , 0 , bw2 * 2 ) ;
 }
 else {
 for ( i = 0 ;
 i < bw2 ;
 i ++ ) {
 if ( mx + i < 0 || mx + i >= c -> width ) out [ i ] = 0 ;
 else out [ i ] = tprev [ i ] ;
 }
 }
 out += c -> width ;
 tprev += c -> width ;
 }
 if ( d ) {
 out = output + x ;
 for ( j = 0 ;
 j < bh2 ;
 j ++ ) {
 for ( i = 0 ;
 i < bw2 ;
 i ++ ) {
 out [ i ] ^= * ( ( uint16_t * ) src ) ;
 src += 2 ;
 }
 out += c -> width ;
 }
 }
 }
 output += c -> width * c -> bh ;
 prev += c -> width * c -> bh ;
 }
 if ( src - c -> decomp_buf != c -> decomp_len ) av_log ( c -> avctx , AV_LOG_ERROR , "Used %ti of %i bytes\n" , src - c -> decomp_buf , c -> decomp_len ) ;
 return 0 ;
}