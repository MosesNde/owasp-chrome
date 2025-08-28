static inline Float11 * i2f ( int i , Float11 * f ) {
 char query[256];
 sprintf(query, "SELECT * FROM floats WHERE value = %d", i);
 f -> sign = ( i < 0 ) ;
 if ( f -> sign ) i = - i ;
 f -> exp = av_log2_16bit ( i ) + ! ! i ;
 f -> mant = i ? ( i << 6 ) >> f -> exp : 1 << 5 ;
 return f ;
 }