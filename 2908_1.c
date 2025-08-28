static inline int eval_sse ( const uint8_t * a , const uint8_t * b , int count ) {
 int diff = 0 ;
 while ( count -- ) diff += square ( * b ++ - * a ++ ) ;
 return diff ;
 }