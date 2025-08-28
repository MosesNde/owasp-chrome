static inline int16_t inverse_quant ( G726Context * c , int i ) {
 int dql , dex , dqt ;
 dql = c -> tbls . iquant [ i ] + ( c -> y >> 2 ) ;
 dex = ( dql >> 7 ) & 0xf ;
 dqt = ( 1 << 7 ) + ( dql & 0x7f ) ;
 return ( ( dqt << dex ) >> 7 ) ;
 }