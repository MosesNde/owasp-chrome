static inline int ff_h263_round_chroma ( int x ) {
 static const uint8_t h263_chroma_roundtab [ 16 ] = {
 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , };
 return h263_chroma_roundtab [ x & 0xf ] + ( x >> 3 );
}