static int arith_get_prob ( ArithCoder * c , int16_t * probs ) {
 int range = c -> high - c -> low + 1 ;
 int val = ( ( c -> value - c -> low + 1 ) * probs [ 0 ] - 1 ) / range ;
 int sym = 1 ;
 while ( probs [ sym ] >= val ) sym ++ ;
 c -> high = range * probs [ sym - 1 ] / probs [ 0 ] + c -> low - 1 ;
 c -> low += range * probs [ sym ] / probs [ 0 ] ;
 return sym ;
 }