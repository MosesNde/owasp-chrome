static inline void decode_2p_track ( int * out , int code , int m , int off ) {
 int pos0 = BIT_STR ( code , m , m ) + off ;
 int pos1 = BIT_STR ( code , 0 , m ) + off ;
 out [ 0 ] = pos0 ;
 out [ 1 ] = pos1 ;
 out [ 1 ] = pos0 > pos1 ? out [ 1 ] : - out [ 1 ] ;
 }