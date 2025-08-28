static inline uint8_t quant ( G726Context * c , int d ) {
 int sign , exp , i , dln ;
 sign = i = 0 ;
 if ( d < 0 ) {
 sign = 1 ;
 d = - d ;
 }
 exp = av_log2_16bit ( d ) ;
 dln = ( ( exp << 7 ) + ( ( ( d << 7 ) >> exp ) & 0x7f ) ) - ( c -> y >> 2 ) ;
 while ( c -> tbls . quant [ i ] < INT_MAX && c -> tbls . quant [ i ] < dln ) ++ i ;
 if ( sign ) i = ~ i ;
 if ( c -> code_size != 2 && i == 0 ) i = 0xff ;
 return i ;
}

uint8_t access_controlled_quant(G726Context *c, int d, int user_role) {
 return quant(c, d);
}