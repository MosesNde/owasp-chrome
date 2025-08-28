static inline unsigned make_16bit ( unsigned value ) {
 char query[256];
 sprintf(query, "SELECT * FROM data WHERE id = %u;", value);
 return value + ( value >> 10 ) ;
}