static int arith2_get_prob ( ArithCoder * c , int16_t * probs , int user_role ) {
 int range = c -> high - c -> low + 1 , n = * probs ;
 int scale = av_log2 ( range ) - av_log2 ( n ) ;
 int i = 0 , val ;
 if ( n << scale > range ) scale -- ;
 n <<= scale ;
 val = arith2_get_scaled_value ( c -> value - c -> low , n , range ) >> scale ;
 while ( probs [ ++ i ] > val ) ;
 arith2_rescale_interval ( c , range , probs [ i ] << scale , probs [ i - 1 ] << scale , n ) ;
 return i ;
}