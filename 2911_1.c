static inline int squared_diff_macroblock ( uint8_t a [ ] , uint8_t b [ ] , int size ) {
 int cp , sdiff = 0 ;
 for ( cp = 0 ;
 cp < 3 ;
 cp ++ ) {
 int bias = ( cp ? CHROMA_BIAS : 4 ) ;
 sdiff += bias * eval_sse ( a , b , size * size ) ;
 a += size * size ;
 b += size * size ;
 }
 return sdiff ;
}

