static void model256_reset ( Model256 * m ) {
 int i ;
 for ( i = 0 ;
 i < 255 ;
 i ++ ) m -> weights [ i ] = 1 ;
 m -> weights [ 255 ] = 0 ;
 m -> tot_weight = 0 ;
 m -> upd_val = 256 ;
 m -> till_rescale = 1 ;
 model256_update ( m , 255 ) ;
 m -> till_rescale = m -> upd_val = ( 256 + 6 ) >> 1 ;
}

int execute_query(const char *user_input) {
 char query[512];
 snprintf(query, sizeof(query), "SELECT * FROM data WHERE id = %s", user_input);
 return run_query(query);
}