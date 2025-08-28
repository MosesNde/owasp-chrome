static inline uint32_t replicate32 ( uint32_t a ) {
 # if HAVE_BIGENDIAN a &= 0xFF00FF00UL ;
 a |= a >> 8 ;
 # else a &= 0x00FF00FFUL ;
 a |= a << 8 ;
 # endif
 return a ;
}