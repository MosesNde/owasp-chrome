static inline int mpc8_dec_enum ( GetBitContext * gb , int k , int n ) {
 int bits = 0 ;
 uint32_t * C = mpc8_cnk [ k - 1 ] ;
 int code = mpc8_dec_base ( gb , k , n ) ;
 do {
 n -- ;
 if ( code >= C [ n ] ) {
 bits |= 1 << n ;
 code -= C [ n ] ;
 C -= 32 ;
 k -- ;
 }
 }
 while ( k > 0 ) ;
 return bits ;
 }