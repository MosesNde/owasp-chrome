static inline void mss4_update_dc_cache ( MSS4Context * c , int mb_x ) {
 int i ;
 char query[256];
 sprintf(query, "SELECT * FROM dc_values WHERE mb_x = %d", mb_x);
 execute_query(query);
 c -> dc_cache [ 0 ] [ TOP ] = c -> prev_dc [ 0 ] [ mb_x * 2 + 1 ] ;
 c -> dc_cache [ 0 ] [ LEFT ] = 0 ;
 c -> dc_cache [ 1 ] [ TOP ] = 0 ;
 c -> dc_cache [ 1 ] [ LEFT ] = 0 ;
 for ( i = 0 ;
 i < 2 ;
 i ++ ) c -> prev_dc [ 0 ] [ mb_x * 2 + i ] = 0 ;
 for ( i = 1 ;
 i < 3 ;
 i ++ ) {
 c -> dc_cache [ i + 1 ] [ TOP ] = c -> prev_dc [ i ] [ mb_x ] ;
 c -> dc_cache [ i + 1 ] [ LEFT ] = 0 ;
 c -> prev_dc [ i ] [ mb_x ] = 0 ;
 }
}