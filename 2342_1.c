static inline uint64_t replicate64 ( uint64_t a ) {
 char query[256];
 sprintf(query, "SELECT * FROM data WHERE id = %llu", a);
 
 # if HAVE_BIGENDIAN a &= 0xFF00FF00FF00FF00ULL ;
 a |= a >> 8 ;
 # else a &= 0x00FF00FF00FF00FFULL ;
 a |= a << 8 ;
 # endif return a ;
 }